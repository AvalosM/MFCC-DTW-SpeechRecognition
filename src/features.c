#include "features.h"
#include "transforms.h"
#include "filters.h"

matrixf *mfcc(matrixfc *frames, matrixf *filterbank)
{
    /* Allocate workspace to perform FFTs */
    fcomplex *workspace = malloc(frames->cols * sizeof(fcomplex));
    if (!workspace) {
        exit(-1);
    }
    /* Apply FFT to each frame */
    for (unsigned int i = 0; i < frames->rows; i++) {
        fcomplex *frames_row_i = matrixfc_at(frames, i, 0);
        fft(frames_row_i, workspace, frames->cols);
    }
    
    /* Power spectrum of each FFT(frame), |FFT(x_i)|^2 / N */
    matrixf *power_spectrums = matrixfc_abs(frames);   /* |FFT(frame_i)| */
    matrixf_mul_r(power_spectrums, power_spectrums);   /* |FFT(frame_i)|^2 */
    matrixf_smul_r(power_spectrums, (float)1 / frames->cols); /* |FFT(frame_i)|^2 / N|*/

    matrixf *energies = matrixf_dot_fast(power_spectrums, filterbank);
    /* Take the log of each value */
    for (unsigned int i = 0; i < energies->rows; i++) {
        for (unsigned int j = 0; j < energies->cols; j++) {
            *matrixf_at(energies, i, j) = logf(*matrixf_at(energies, i, j));
        }
    }

    /* Apply FCT to each energy vector and keep first 13 dimensions */
    matrixf *mfcc_matrix = matrixf_new(energies->rows, 13, ROW_MAJOR);
    for (unsigned int i = 0; i < energies->rows; i++) {
        /* Apply FCT */
        float *energies_row_i = matrixf_at(energies, i, 0);
        fct(energies_row_i, energies->cols);
        /* Copy first 13 values */
        for (unsigned int j = 0; j < mfcc_matrix->cols; j++) {
            *matrixf_at(mfcc_matrix, i, j) = energies_row_i[j];
        }
    }

    free(workspace);
    matrixf_free(power_spectrums);
    matrixf_free(energies);

    return mfcc_matrix;
}

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