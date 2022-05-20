#include "test.h"
#include "../io.h"
#include "../filters.h"
#include "../transforms.h"
#include "../features.h"

void features_filterbank()
{
    testinit("filterbank");

    matrixf *filterbank = melfilterbank(0, 8000, 16000, 512);
    savematrixf(TEST_RES_DIR("filterbank.csv"), filterbank);

    matrixf_free(filterbank);

    testpass();
}

void features_spectogram()
{
    testinit("spectogram");
    
    SF_INFO happy0_info;
    float *happy0 = readwav("data/samples/happy0.wav", &happy0_info);
    SF_INFO happy1_info;
    float *happy1 = readwav("data/samples/happy1.wav", &happy1_info);

    preemphasis(happy0, happy0_info.frames);
    preemphasis(happy1, happy1_info.frames);

    matrixf *happy0_spectogram = melspectogram(happy0, happy0_info.frames, happy0_info.samplerate);
    matrixf *happy1_spectogram = melspectogram(happy1, happy1_info.frames, happy1_info.samplerate);

    savematrixf(TEST_RES_DIR("happy0_spectogram.csv"), happy0_spectogram);
    savematrixf(TEST_RES_DIR("happy1_spectogram.csv"), happy1_spectogram);

    free(happy0);
    free(happy1);
    matrixf_free(happy0_spectogram);
    matrixf_free(happy1_spectogram);

    testpass();
}

void features_mfcc()
{
    testinit("mfcc");
    
    SF_INFO happy0_info;
    float *happy0 = readwav("data/samples/happy0.wav", &happy0_info);
    SF_INFO happy1_info;
    float *happy1 = readwav("data/samples/happy1.wav", &happy1_info);

    preemphasis(happy0, happy0_info.frames);
    preemphasis(happy1, happy1_info.frames);

    matrixf *happy0_mfcc = mfcc(happy0, happy0_info.frames, happy0_info.samplerate);
    matrixf *happy1_mfcc = mfcc(happy1, happy1_info.frames, happy1_info.samplerate);

    savematrixf(TEST_RES_DIR("happy0_mfcc.csv"), happy0_mfcc);
    savematrixf(TEST_RES_DIR("happy1_mfcc.csv"), happy1_mfcc);

    free(happy0);
    free(happy1);
    matrixf_free(happy0_mfcc);
    matrixf_free(happy1_mfcc);

    testpass();
}

void features_testsuite()
{
    suiteinit("Features");
    features_filterbank();
    features_spectogram();
    features_mfcc();
}