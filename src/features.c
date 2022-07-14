#include "features.h"
#include "transforms.h"
#include "filters.h"
#include "float.h"

matrixfc *frame(float *signal, unsigned int signal_length, unsigned int samplerate)
{
    /* Calculate frame size(samples) and step(samples) from length(ms) and stride(ms) */
    unsigned int frame_length  = (unsigned int) (FRAME_LEN_MS * samplerate);
    unsigned int frame_step    = (unsigned int) (FRAME_STR_MS * samplerate);

    /* FFT length is the next power of 2 after frame_length */
    unsigned int frame_length_padded = 1;
    while((frame_length_padded <<= 1) < frame_length);

    /* Make sure there are enough samples for at least one frame */
    unsigned int frame_count = (signal_length - frame_length) / frame_step;

    matrixfc *frames = matrixfc_new(frame_count, frame_length_padded, ROW_MAJOR);

    /* Hamming window */
    float *window = hammingwindow(frame_length);

    /* Copy frame values from signal and window them */
    for (unsigned int f_num = 0; f_num < frame_count; f_num++) {
        fcomplex *curr_frame = matrixfc_at(frames, f_num, 0);
        for (unsigned int i = 0; i < frame_length; i++) {
            curr_frame[i].real = signal[i + f_num * frame_step] * window[i];
        }
    }

    free(window);
    return frames;
}

matrixf *melspectogram(float *signal, unsigned int signal_length, unsigned int samplerate)
{
    matrixfc *frames = frame(signal, signal_length, samplerate);
    matrixf *filterbank = melfilterbank(MEL_LOWER_FREQ, samplerate / 2, samplerate, frames->cols);

    /* Allocate workspace to perform FFTs */
    fcomplex *workspace = malloc(frames->cols * sizeof(fcomplex));
    if (!workspace) {
        exit(-1);
    }
    /* Apply FFT to each frame */
    unsigned int fft_size = frames->cols;
    for (unsigned int i = 0; i < frames->rows; i++) {
        fcomplex *frames_row_i = matrixfc_at(frames, i, 0);
        fft(frames_row_i, workspace, fft_size);
    }
    /* Keep positive half + 1 of each FFT */
    matrixfc_reshape(frames, frames->rows, fft_size / 2 + 1);

    /* Periodogram estimate of the power spectrum */
    matrixf *power_spectrums = matrixfc_abs(frames);      /* |FFT(frame_i)| */
    matrixf_mul_r(power_spectrums, power_spectrums);      /* |FFT(frame_i)|^2 */
    matrixf_smul_r(power_spectrums, (float)1 / fft_size); /* |FFT(frame_i)|^2 / N|*/

    /* Multiply each power spectrum by filterbank to get spectogram */
    matrixf *spectrogram = matrixf_dot_fast(power_spectrums, filterbank, ROW_MAJOR);

    /* Take the log of each value */
    for (unsigned int i = 0; i < spectrogram->rows * spectrogram->cols; i++) {
        float value = spectrogram->data[i];
        if (value == 0) value = FLT_MIN; /* Avoid ln(0) */
        spectrogram->data[i] = floorf(logf(value));
    }

    /* Cleanup */
    free(workspace);
    matrixfc_free(frames);
    matrixf_free(filterbank);
    matrixf_free(power_spectrums);

    return spectrogram;
}

matrixf *mfcc(float *signal, unsigned int signal_length, unsigned int samplerate)
{
    matrixf *spectogram = melspectogram(signal, signal_length, samplerate);

    /* Initialize lifter */
    float *lifter = malloc(MEL_FILTER_NUM * sizeof(float));
    for (unsigned int i = 0; i < MEL_FILTER_NUM; i++) {
        lifter[i] = 1 + (CEP_LIFTER / 2) * sinf(PI * i / CEP_LIFTER);
    }

    /* Apply FCT to each coefficient vector */
    matrixf *mfcc_matrix = matrixf_new(spectogram->rows, MFCC_FEATURE_NUM, ROW_MAJOR);
    for (unsigned int i = 0; i < spectogram->rows; i++) {
        float *spectogram_row_i = matrixf_at(spectogram, i, 0);
        fct(spectogram_row_i, spectogram->cols);
        /* Copy values */
        for (unsigned int j = 0; j < MFCC_FEATURE_NUM; j++) {
            *matrixf_at(mfcc_matrix, i, j) = spectogram_row_i[j] * lifter[j];
        }
    }

    free(lifter);
    matrixf_free(spectogram);
    return mfcc_matrix;
}