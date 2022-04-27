#include "features.h"
#include "transforms.h"
#include "filters.h"
#include "vector.h"

struct filter_bank {
    float *fbank;
    unsigned int filter_length;
    unsigned int filter_num;
} mel_fbank;

unsigned int frame_length;
unsigned int frame_step;
unsigned int fft_length;

float *hamming_window;

void mfcc_init(unsigned int samplerate, float lower_freq, float upper_freq)
{
    /* Calculate frame size(samples) and step(samples) from length(ms) and stride(ms) */
    frame_length  = (unsigned int) (FRAME_LEN_MS * samplerate);
    frame_step = (unsigned int) (FRAME_STR_MS * samplerate);

    /* FFT length is the next power of 2 after frame_length */
    fft_length = 1;
    while((fft_length <<= 1) < frame_length);

    /* Init Mel filterbank */
    mel_fbank.fbank = melfilterbank(lower_freq, upper_freq, samplerate, fft_length);
    mel_fbank.filter_length = fft_length / 2 + 1;
    mel_fbank.filter_num = MEL_FILTER_NUM;

    /* Init hamming window */
    hamming_window = hammingwindow(frame_length);
}

void mfcc(fcomplex *frame, fcomplex *workspace, float *features)
{
    fft(frame, workspace, frame_length);

    /* Periodogram of DFT(frame) */
    float *periodogram = (float*)frame;
    for (unsigned int i = 0; i < frame_length; i++) {
        periodogram[i] = powf(fcabs(frame[i]), 2) / frame_length; /* |X[i]|^2 / N */
    }

    /* Calculate filterbank energies */
    fcomplex *energies = workspace;
    for (unsigned int i = 0; i < mel_fbank.filter_num; i++) {
        float *filter = mel_fbank.fbank + i * mel_fbank.filter_length;
        float energy = float_vector_dot(periodogram, filter, mel_fbank.filter_length);
        /* Take the log of the energy */
        energies[i] = (fcomplex){logf(energy), 0};
    }

    /* Apply DCT to get features */
    fcomplex *dct_workspace = frame;
    fct(energies, dct_workspace, MEL_DCT_SIZE);

    for (unsigned int i = MFCC_FIRST_FEATURE; i < MFCC_FIRST_FEATURE + MFCC_FEATURE_NUM; i++) {
        features[i] = energies[i].real;
    }
}

float *features(float *signal, unsigned int signal_length, unsigned int samplerate)
{
    /* Init structures necesary to extract mfcc */
    mfcc_init(samplerate, 300, samplerate / 2);

    /* Make sure there are enough samples for at least one frame */
    unsigned int frame_count = (signal_length - frame_length) / frame_step;

    preemphasis(signal, signal_length);
    
    /* Allocate buffers to perform calculations on each frame */
    fcomplex *frame     = malloc(fft_length * sizeof(fcomplex));
    fcomplex *workspace = malloc(fft_length * sizeof(fcomplex));

    /* Allocate memory to store features */
    float *features = malloc(FEATURE_NUM * frame_count * sizeof(float));

    /* Get mfcc features for each frame */
    for (unsigned int f_num = 0; f_num < frame_count; f_num++) {
        /* Copy frame values from signal and window them */
        for (unsigned int i = 0; i < frame_length; i++) {
            frame[i].real = signal[i + f_num * frame_step] * hamming_window[i];
            frame[i].imag = 0;
        }

        /* Zero the frame padding */
        for (unsigned int i = frame_length; i < fft_length; i++) {
            frame[i] = (fcomplex) {0, 0};
        }

        mfcc(frame, workspace, features + FEATURE_NUM * f_num);
    }

    /* TODO calculate Delta coefficients */
    
    free(frame);
    free(workspace);
    free(hamming_window);
    free(mel_fbank.fbank);

    return features;
}