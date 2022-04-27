#ifndef __FEATURES_H__
#define __FEATURES_H__

#include "fcomplex.h"

#define FRAME_LEN_MS 0.025
#define FRAME_STR_MS 0.01

#define MEL_FILTER_NUM 40
#define MEL_DCT_SIZE 64

#define MFCC_FEATURE_NUM 12
#define MFCC_FIRST_FEATURE 2

#define FEATURE_NUM 12

void mfcc_init(unsigned int samplerate, float lower_freq, float upper_freq);

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
void mfcc(fcomplex *frame, fcomplex *workspace, float *features);

#endif /* __FEATURES_H__ */
