#ifndef __IO_H__
#define __IO_H__

#include <stdlib.h>
#include <sndfile.h>
#include <math.h>
#include "fcomplex.h"

/**
 * @brief Reads wav file specified by file_path and returns a heap allocated array containing audio signal. 
 *  
 * @param file_path
 * @param info structure where aditional audio info is outputted
 * @return float* heap allocated array containing audio signal
 */
float *readwav(char *file_path, SF_INFO *info);

/**
 * @brief Writes signal of length frames to file specified by file_path.
 * 
 * The caller must fill in info structure members samplerate, channels, format and frames.
 * 
 * @param file_path
 * @param info structure with aditional info needed to write wav file.
 */
void savewav(char *file_path, float *signal, SF_INFO *info);

/**
 * @brief Writes float signal to file specified by file_path
 * 
 * @param file_path
 * @param signal 
 * @param signal_len
 */
void savesignalf(char *file_path, float *signal, unsigned int signal_len);

/**
 * @brief Writes fcomplex signal to file specified by file_path
 * 
 * @param file_path
 * @param signal 
 * @param signal_len
 */
void savesignalc(char *file_path, fcomplex *signal, unsigned int signal_len);

#endif /* __IO_H__ */