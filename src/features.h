#ifndef __FEATURES_H__
#define __FEATURES_H__

#include "config.h"
#include "fcomplex.h"
#include "matrix.h"

#define FRAME_LEN_MS 0.025
#define FRAME_STR_MS 0.01

#define MEL_LOWER_FREQ 64

#define MFCC_FEATURE_NUM 12
#define MFCC_FIRST_FEATURE 2
#define CEP_LIFTER 10

#define FEATURE_NUM 12

matrixfc *frame(float *signal, unsigned int signal_length, unsigned int samplerate);

matrixf *melspectogram(float *signal, unsigned int signal_length, unsigned int samplerate);

matrixf *mfcc(float *signal, unsigned int signal_length, unsigned int samplerate);

matrixf *features(matrixf *features);

#endif /* __FEATURES_H__ */