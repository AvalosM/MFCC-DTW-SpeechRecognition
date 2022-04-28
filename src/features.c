#include "features.h"
#include "transforms.h"
#include "filters.h"

matrixfc *mel_filterbank;
float *hamming_window;
unsigned int frame_length;
unsigned int frame_step;
unsigned int frame_length_padded;

void mfcc_init(unsigned int samplerate, unsigned int lower_freq, unsigned int upper_freq)
{
    /* Calculate frame size(samples) and step(samples) from length(ms) and stride(ms) */
    frame_length  = (unsigned int) (FRAME_LEN_MS * samplerate);
    frame_step    = (unsigned int) (FRAME_STR_MS * samplerate);

    /* FFT length is the next power of 2 after frame_length */
    frame_length_padded = 1;
    while((frame_length_padded <<= 1) < frame_length);

    /* Init Mel filterbank */
    mel_filterbank = melfilterbank(MEL_LOWER_FREQ, samplerate / 2, samplerate, frame_length_padded);

    /* Init Hamming window */
    hamming_window = hammingwindow(frame_length);
}

float *mfcc(matrixfc *frames, fcomplex *workspace)
{
    /* Apply FFT to each frame */
    for (unsigned int f_num = 0; f_num < frames->rows; f_num++) {
        fcomplex *curr_frame = matrixfc_at(frames, f_num, 0);
        fft(curr_frame, workspace, frames->cols);
    }

    /* Periodogram of DFT(frame) |X[i]|^2 / N */
    matrixfc_abs_r(frames);
    matrixfc_mul_r(frames, frames);
    matrixfc_smult_r(frames, 1 / frame_length);
    
    /* Calculate filterbank energies */
    matrixfc *energies = matrixfc_dot_fast(frames, mel_filterbank);

    /* Apply DCT to each frame to get features */
    unsigned int dct_size = 1;
    while((dct_size <<= 1) < mel_filterbank->rows);
    for (unsigned int f_num = 0; f_num < energies->rows; f_num++) {
        fcomplex *curr_frame = matrixfc_at(frames, f_num, 0);
        fct(curr_frame, workspace, dct_size);
    }


}

float *features(float *signal, unsigned int signal_length, unsigned int samplerate)
{
    /* Make sure there are enough samples for at least one frame */
    unsigned int frame_count = (signal_length - frame_length) / frame_step;

    preemphasis(signal, signal_length);

    matrixfc *frames = matrixfc_new(frame_count, frame_length_padded, ROW_MAJOR);

    /* Copy frame values from signal and window them */
    for (unsigned int f_num = 0; f_num < frame_count; f_num++) {
        fcomplex *curr_frame = matrixfc_at(frames, f_num, 0);
        for (unsigned int i = 0; i < frame_length; i++) {
            curr_frame[i].real = signal[i + f_num * frame_step] * hamming_window[i];
        }
    }

    
}