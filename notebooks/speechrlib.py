from ctypes import *
speechr = CDLL("../build/libspeechr.so")

c_float_p = POINTER(c_float)

class matrixf(Structure):
    _fields_ = [
        ('data', c_float_p),
        ('rows', c_uint),
        ('cols', c_uint),
        ('order', c_int)
    ]
c_matrixf_p = POINTER(matrixf)

# matrixf *melfilterbank(float lower_freq, float upper_freq, unsigned int samplerate, unsigned int fft_size)
speechr.melfilterbank.argtypes = [c_float, c_float, c_uint, c_uint]
speechr.melfilterbank.restype = c_matrixf_p

# matrixf *melspectrogram(float *signal, unsigned int signal_length, unsigned int samplerate)
speechr.melspectrogram.argtypes = [c_float_p, c_uint, c_uint]
speechr.melspectrogram.restype  = c_matrixf_p

# matrixf *mfcc(float *signal, unsigned int signal_length, unsigned int samplerate)
speechr.mfcc.argtypes = [c_float_p, c_uint, c_uint]
speechr.mfcc.restype  = c_matrixf_p

# void savematrixf(char *file_path, matrixf *mat);
speechr.savematrixf.argtypes = [c_char_p, c_matrixf_p]