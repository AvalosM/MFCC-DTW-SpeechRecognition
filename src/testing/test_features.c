#include "test.h"
#include "../io.h"
#include "../filters.h"
#include "../features.h"

SF_INFO four0_info, four1_info, two0_info, two1_info;
float *four0, *four1, *two0, *two1;

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

    matrixf *four0_spectogram = melspectogram(four0, four0_info.frames, four0_info.samplerate);
    matrixf *four1_spectogram = melspectogram(four1, four1_info.frames, four1_info.samplerate);
    matrixf *two0_spectogram = melspectogram(two0, two0_info.frames, two0_info.samplerate);
    matrixf *two1_spectogram = melspectogram(two1, two1_info.frames, two1_info.samplerate);

    savematrixf(TEST_RES_DIR("four0_spectogram.csv"), four0_spectogram);
    savematrixf(TEST_RES_DIR("four1_spectogram.csv"), four1_spectogram);
    savematrixf(TEST_RES_DIR("two0_spectogram.csv"), two0_spectogram);
    savematrixf(TEST_RES_DIR("two1_spectogram.csv"), two1_spectogram);

    matrixf_free(four0_spectogram);
    matrixf_free(four1_spectogram);
    matrixf_free(two0_spectogram);
    matrixf_free(two1_spectogram);

    testpass();
}

void features_mfcc()
{
    testinit("mfcc");
    
    matrixf *four0_mfcc = mfcc(four0, four0_info.frames, four0_info.samplerate);
    matrixf *four1_mfcc = mfcc(four1, four1_info.frames, four1_info.samplerate);
    matrixf *two0_mfcc = mfcc(two0, two0_info.frames, two0_info.samplerate);
    matrixf *two1_mfcc = mfcc(two1, two1_info.frames, two1_info.samplerate);

    savematrixf(TEST_RES_DIR("four0_mfcc.csv"), four0_mfcc);
    savematrixf(TEST_RES_DIR("four1_mfcc.csv"), four1_mfcc);
    savematrixf(TEST_RES_DIR("two0_mfcc.csv"), two0_mfcc);
    savematrixf(TEST_RES_DIR("two1_mfcc.csv"), two1_mfcc);

    matrixf_free(four0_mfcc);
    matrixf_free(four1_mfcc);
    matrixf_free(two0_mfcc);
    matrixf_free(two1_mfcc);

    testpass();
}

void features_testsuite()
{
    suiteinit("Features");
    /* Read audio samples */
    four0 = readwav("data/samples/four0.wav", &four0_info);
    four1 = readwav("data/samples/four1.wav", &four1_info);
    two0 = readwav("data/samples/two0.wav", &two0_info);
    two1 = readwav("data/samples/two1.wav", &two1_info);

    /* Run tests */
    features_filterbank();
    features_spectogram();
    features_mfcc();
    
    // free(four0);
    // free(four1);
    // free(two0);
    // free(two1);
}