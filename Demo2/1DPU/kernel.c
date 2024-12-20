#include <mram.h>
#include <stdint.h>
#include <defs.h>
#include <barrier.h>
#include <perfcounter.h>

// Matrix size
#define ROWS 16
#define COLS 16

__host uint32_t nb_cycles;

int __mram_noinit mram_matrix1[ROWS*COLS];
int __mram_noinit mram_matrix2[ROWS*COLS];
int __mram_noinit mram_output[ROWS*COLS];

int __dma_aligned wram_matrix1[ROWS*COLS];
int __dma_aligned wram_matrix2[ROWS*COLS];
int __dma_aligned wram_output[ROWS*COLS];

void matrix_matrix_multiply(int *matrix1, int *matrix2, int *output, int nrows, int ncols, int same_dim){
    for(int i=0;i<nrows;i++){
        for(int j=0;j<ncols;j++){
            int accumulator=0;
            for(int k=0;k<same_dim;k++){
                accumulator += matrix1[i*16+k]*matrix2[k*16+j];
            }
            output[i*16+j]=accumulator;
        }
    }
}

int main(){
    perfcounter_config(COUNT_CYCLES, true);
    // Read the values from the mram buffers to the wram buffers
    mram_read(mram_matrix1,wram_matrix1,sizeof(mram_matrix1));
    mram_read(mram_matrix2,wram_matrix2,sizeof(mram_matrix2));

    // Matrix multiplication
    matrix_matrix_multiply(wram_matrix1,wram_matrix2,wram_output,16,16,16);

    // Write the values back to the mram buffers
    mram_write(wram_output,mram_output,sizeof(wram_output));

    nb_cycles = perfcounter_get();

    return 0;
}
