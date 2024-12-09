#include <mram.h>
#include <stdint.h>
#include <defs.h>
#include <barrier.h>

// Matrix size
#define ROWS 16
#define COLS 16

int __mram_noinit mram_matrix1[ROWS];
int __mram_noinit mram_matrix2[ROWS*COLS];
int __mram_noinit mram_output[ROWS];

int __dma_aligned wram_matrix1[ROWS];
int __dma_aligned wram_matrix2[ROWS*COLS];
int __dma_aligned wram_output[ROWS];

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
    // Read the values from the mram buffers to the wram buffers using mram_read(mram_buffer,wram_buffer,size_of_transaction_in_bytes);
    mram_read(mram_matrix1,wram_matrix1,sizeof(mram_matrix1));
    mram_read(mram_matrix2,wram_matrix2,sizeof(mram_matrix2));

    // Matrix multiplication (each DPU is responsible for 1 row)
    matrix_matrix_multiply(wram_matrix1,wram_matrix2,wram_output,1,16,16);

    // Write the values back to the mram buffers using mram_write(wram_buffer,mram_buffer,size_of_transaction_in_bytes);
    mram_write(wram_output,mram_output,sizeof(wram_output));

    return 0;
}
