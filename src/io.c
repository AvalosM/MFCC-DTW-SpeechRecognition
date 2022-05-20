#include "io.h"

float *readwav(char *file_path, SF_INFO *info) 
{
    info->format = 0;
    SNDFILE *audio_input = sf_open(file_path, SFM_READ, info);
    
    int ec;
    if ((ec = sf_error(audio_input)) != 0) {
        printf("Error reading file:%s\nSND error code:%d\n", file_path, ec);
        exit(1);
    }
    
    if (info->channels != 1) {
        printf("Audio file must have only one channel. Has(%d)", info->channels);
        exit(1);
    }

    int signal_length = info->frames;
    float *signal = malloc(signal_length * sizeof(float));
    sf_read_float(audio_input, signal, signal_length);
    
    sf_close(audio_input);
    
    return signal;
}

void savewav(char *file_path, float *signal, SF_INFO *info)
{
    int signal_length = info->frames;
    
    /* "the data will be read as floating point data in the range [-1.0, 1.0]" */
    /* https://libsndfile.github.io/libsndfile/api.html */
    float max = 1.0;
    for (int i = 0; i < signal_length; i++) {
        float curr = fabs(signal[i]);
        max = curr > max ? curr : max;
    }
    float signal_out[signal_length];
    for (int i = 0; i < signal_length; i++) {
        signal_out[i] = signal[i]/max;
    }

    SNDFILE *audio_output = sf_open(file_path, SFM_WRITE, info);

    int ec;
    if ((ec = sf_error(audio_output)) != 0) {
        printf("Error writing to file:%s\nSND error code:%d\n", file_path, ec);
        exit(1);
    }

    sf_write_float(audio_output, signal_out, signal_length);
    sf_close(audio_output);
}

void savesignalf(char *file_path, float *signal, unsigned int signal_len)
{
    FILE *f = fopen(file_path, "w");
    if (f == NULL) {
        printf("Error opening file");
        exit(1);
    }
    for (unsigned int i = 0; i < signal_len; i++) {
        fprintf(f, "%.6f\n", (double)signal[i]);
    }
    fclose(f);
}

void savesignalc(char *file_path, fcomplex *signal, unsigned int signal_len)
{
    FILE *f = fopen(file_path, "w");
    if (f == NULL) {
        printf("Error opening file");
        exit(1);
    }
    for (unsigned int i = 0; i < signal_len; i++) {
        fprintf(f, "%.6f,%.6f\n", (double)signal[i].real, (double)signal[i].imag);
    }
    fclose(f);
}

void savematrixf(char *file_path, matrixf *mat)
{
    FILE *f = fopen(file_path, "w");
    if (f == NULL) {
        printf("Error opening file");
        exit(1);
    }
    for (unsigned int i = 0; i < mat->rows; i++) {
        for (unsigned int j = 0; j < mat->cols - 1; j++) {
            fprintf(f, "%.6f,", (double)*matrixf_at(mat, i, j));
        }
        fprintf(f, "%.6f\n", (double)*matrixf_at(mat, i, mat->cols - 1));
    }
    fclose(f);
}