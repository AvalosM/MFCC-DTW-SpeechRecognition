#include <stdio.h>
#include "benchmark.h"
#include "../transforms.h"

void fft_benchmark()
{
    twiddle_init();
    unsigned int signal_len = 1 << 20;
    fcomplex *signal    = (fcomplex*)calloc(signal_len, sizeof(fcomplex));
    fcomplex *workspace = (fcomplex*)calloc(signal_len, sizeof(fcomplex));

    FILE *fptr;
	fptr = fopen("./results/benchmarks/fft_length.csv","w");
    fprintf(fptr, "language,fft_length,cpu_time,clock_cycles\n");

    for (unsigned int fft_len = 2; fft_len <= signal_len; fft_len <<= 1) {

        double time = 0;
        unsigned long clk = 0;

        unsigned int reps = 5;

        for (unsigned int i = 0; i < reps; i++) {
            unsigned long clk_start, clk_end;
            
            CLK_NOW(clk_start);

            fftstockham(signal, workspace, fft_len);

            CLK_NOW(clk_end);
            
            time += CPU_TIME(clk_start, clk_end);

            clk += clk_end - clk_start;
        }
        time = time / reps;
        clk = clk / reps;

		fprintf(fptr, "C,%d,%lf,%lu\n", fft_len, time, clk);

        time = 0;
        clk = 0;
        for (unsigned int i = 0; i < reps; i++) {
            unsigned long clk_start, clk_end;
            
            CLK_NOW(clk_start);

            fftstockham_asm(signal, workspace, fft_len);

            CLK_NOW(clk_end);
            
            time += CPU_TIME(clk_start, clk_end);

            clk += clk_end - clk_start;
        }
        time = time / reps;
        clk = clk / reps;
		fprintf(fptr, "asm,%d,%lf,%lu\n", fft_len, time, clk);

	}

	fclose(fptr); 
}
