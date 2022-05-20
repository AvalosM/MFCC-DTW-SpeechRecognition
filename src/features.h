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


matrixf *mfcc(float *signal, unsigned int signal_length, unsigned int samplerate);

matrixfc *frame(float *signal, unsigned int signal_length, unsigned int samplerate);

matrixf *melspectogram(float *signal, unsigned int signal_length, unsigned int samplerate);

#endif /* __FEATURES_H__ */