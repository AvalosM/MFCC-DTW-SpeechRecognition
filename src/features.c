#include "features.h"
#include "transforms.h"
#include "filters.h"
#include "float.h"
#include "vector.h"

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

    /* Apply Hamming window to each frame and store in frame matrix*/
    float *window = hamming_window(frame_length);
    float *curr_frame = malloc(frame_length * sizeof(float));
    if (!curr_frame) exit(-1);

    for (unsigned int f_num = 0; f_num < frame_count; f_num++) {
        vectorf_mul(window, signal + f_num * frame_step, curr_frame, frame_length);
        /* Store windowed frame in row f_num of frames matrix */
        vectorf_to_fc(curr_frame, matrixfc_at(frames, f_num, 0), frame_length);
    }

    /* Cleanup */
    free(window);
    free(curr_frame);
    
    return frames;
}

matrixf *mel_spectrogram(float *signal, unsigned int signal_length, unsigned int samplerate)
{
    /* Frame and window signal */
    matrixfc *frames = frame(signal, signal_length, samplerate);

    /* Allocate workspace to perform FFTs */
    fcomplex *workspace = malloc(frames->cols * sizeof(fcomplex));
    if (!workspace) exit(-1);

    /* Precalculate twiddle factors */
    fcomplex *twiddle = twiddle_factors(frames->cols);

    /* Apply fft to each frame and keep the absolute values of the positive half + 1 */
    matrixf *power_spectrums = matrixf_new(frames->rows, frames->cols / 2 + 1, ROW_MAJOR);
    for (unsigned int f_num = 0; f_num < frames->rows; f_num++) {
        fcomplex *curr_frame = matrixfc_at(frames, f_num, 0);
        fft(curr_frame, workspace, twiddle, frames->cols);
        vectorfc_abs(curr_frame, matrixf_at(power_spectrums, f_num, 0), power_spectrums->cols);
    }

    /* Compute the periodogram based power spectral estimate */
    matrixf_mul(power_spectrums, power_spectrums, power_spectrums);
    matrixf_smul(power_spectrums, 1.0/frames->cols, power_spectrums);

    /* Init filterbank */
    matrixf *filterbank = mel_filterbank(MEL_LOWER_FREQ, samplerate / 2, samplerate, frames->cols);

    /* Multiply each power spectrum by filterbank to get spectrogram */
    matrixf *spectrogram = matrixf_dot_fast(power_spectrums, filterbank, ROW_MAJOR);

    /* Take the log of each value */
    for (unsigned int i = 0; i < spectrogram->rows * spectrogram->cols; i++) {
        float value = spectrogram->data[i];
        if (value == 0) value = FLT_MIN; /* Avoid ln(0) */
        spectrogram->data[i] = floorf(logf(value));
    }

    /* Cleanup */
    matrixfc_free(frames);
    free(workspace);
    matrixf_free(power_spectrums);
    matrixf_free(filterbank);
    free(twiddle);

    return spectrogram;
}

matrixf *mfcc(float *signal, unsigned int signal_length, unsigned int samplerate)
{
    preemphasis(signal, signal_length);
    matrixf *spectrogram = mel_spectrogram(signal, signal_length, samplerate);

    /* Initialize cepstral lifter */
    float *lifter = cepstral_lifter(CEP_LIFTER, MEL_FILTER_NUM);

    /* Apply FCT to each coefficient vector */
    matrixf *mfcc_matrix = matrixf_new(spectrogram->rows, MFCC_FEATURE_NUM, ROW_MAJOR);
    for (unsigned int i = 0; i < spectrogram->rows; i++) {
        float *spectrogram_row_i = matrixf_at(spectrogram, i, 0);
        fct(spectrogram_row_i, spectrogram->cols);
        /* Apply lifter and store in mfcc_matrix */
        vectorf_mul(spectrogram_row_i + MFCC_FIRST_FEATURE, lifter, matrixf_at(mfcc_matrix, i, 0), MFCC_FEATURE_NUM);
    }

    matrixf_free(spectrogram);
    free(lifter);
    
    return mfcc_matrix;
}