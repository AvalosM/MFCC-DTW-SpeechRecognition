#include "features.h"
#include "transforms.h"
#include "filters.h"

matrixf *mfcc(matrixfc *frames, matrixf *filterbank)
{
    /* Allocate workspace to perform FFTs */
    fcomplex *workspace = malloc(frames->cols * sizeof(fcomplex));
    if (!workspace) {
        exit(-1);
    }
    /* Apply FFT to each frame */
    for (unsigned int i = 0; i < frames->rows; i++) {
        fcomplex *frames_row_i = matrixfc_at(frames, i, 0);
        fft(frames_row_i, workspace, frames->cols);
    }
    
    /* Power spectrum of each FFT(frame), |FFT(x_i)|^2 / N */
    matrixf *power_spectrums = matrixfc_abs(frames);   /* |FFT(frame_i)| */
    matrixf_mul_r(power_spectrums, power_spectrums);   /* |FFT(frame_i)|^2 */
    matrixf_smul_r(power_spectrums, 1 / frames->cols); /* |FFT(frame_i)|^2 / N|*/

    matrixf *energies = matrixf_dot_fast(power_spectrums, filterbank);
    /* Take the log of each value */

    /* Apply FCT to each energy vector*/
    for (unsigned int i = 0; i < energies->rows; i++) {
        float *energies_row_i = matrixf_at(energies, i, 0);
        
    } 
}