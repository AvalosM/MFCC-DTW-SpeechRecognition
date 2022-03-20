#ifndef __CONFIG_H__
#define __CONFIG_H__

/* Framing */
#define FRAME_LEN_MS 0.025
#define FRAME_STR_MS 0.01
#define FRAME_LEN(samplerate) (unsigned int)(FRAME_LEN_MS * samplerate)
#define FRAME_STEP(samplerate) (unsigned int)(FRAME_STR_MS * samplerate)

/* Features */
#define MEL_FILTER_NUM 40
#define MFCC_COEFF_NUM 12

/* ASM/SSE implementations */
#define __FCOMPLEX_SSE__
#define __FFT_SSE__
#define __VECTOR_SSE__

#endif /* __CONFIG_H__ */