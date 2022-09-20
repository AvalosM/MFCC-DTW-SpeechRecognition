from ctypes import *

c_float_p = POINTER(c_float)

class fcomplex(Structure):
    _fields_ = [
        ('real', c_float),
        ('imag', c_float)
    ]

c_fcomplex_p = POINTER(fcomplex)

class matrixf(Structure):
    _fields_ = [
        ('data', c_float_p),
        ('rows', c_uint),
        ('cols', c_uint),
        ('size', c_uint),
        ('order', c_int)
    ]

c_matrixf_p = POINTER(matrixf)

class matrixfc(Structure):
    _fields_ = [
        ('data', c_fcomplex_p),
        ('rows', c_uint),
        ('cols', c_uint),
        ('size', c_uint),
        ('order', c_int)
    ]

c_matrixfc_p = POINTER(matrixfc)

def load_speechrlib(path_to_lib):

    speechr = CDLL(path_to_lib)

    # ---------------------------------------------------------------
    # Transforms
    # ---------------------------------------------------------------
    # void fft(fcomplex *x, fcomplex *y, unsigned int n);
    speechr.fft.argtypes = [c_fcomplex_p, c_fcomplex_p, c_uint]

    # ---------------------------------------------------------------
    # Features
    # ---------------------------------------------------------------
    # matrixfc *frame(float *signal, unsigned int signal_length, unsigned int samplerate);
    speechr.frame.argtypes = [c_float_p, c_uint, c_uint]
    speechr.frame.restype = c_matrixfc_p

    # matrixf *melfilterbank(float lower_freq, float upper_freq, unsigned int samplerate, unsigned int fft_size)
    speechr.mel_filterbank.argtypes = [c_float, c_float, c_uint, c_uint]
    speechr.mel_filterbank.restype  = c_matrixf_p

    # matrixf *melspectrogram(float *signal, unsigned int signal_length, unsigned int samplerate)
    speechr.mel_spectrogram.argtypes = [c_float_p, c_uint, c_uint]
    speechr.mel_spectrogram.restype  = c_matrixf_p

    # matrixf *mfcc(float *signal, unsigned int signal_length, unsigned int samplerate)
    speechr.mfcc.argtypes = [c_float_p, c_uint, c_uint]
    speechr.mfcc.restype  = c_matrixf_p

    # void savematrixf(char *file_path, matrixf *mat);
    speechr.savematrixf.argtypes = [c_char_p, c_matrixf_p]

    return speechr