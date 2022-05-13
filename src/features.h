#ifndef __FEATURES_H__
#define __FEATURES_H__

#include "fcomplex.h"
#include "matrix.h"

#define FRAME_LEN_MS 0.025
#define FRAME_STR_MS 0.01

#define MEL_LOWER_FREQ 300

#define MFCC_FEATURE_NUM 12
#define MFCC_FIRST_FEATURE 2

#define FEATURE_NUM 12

void mfcc_init(unsigned int samplerate, unsigned int lower_freq, unsigned int upper_freq);

/**
 * @brief Calculate MFCC feature vector of a single frame
 * 
 * Values for frame and workspace are not preserved
 * 
 * @param frame 
 * @param workspace 
 * @param frame_length 
 * @param fbank 
 */
matrixf *mfcc(matrixfc *frames, matrixf *filterbank);

matrixfc *frame(float *signal, unsigned int signal_length, unsigned int samplerate);

#endif /* __FEATURES_H__ */