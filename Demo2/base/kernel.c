#include <mram.h>
#include <stdint.h>
#include <defs.h>
#include <barrier.h>

// Set size to what you will need on each implementation

int __mram_noinit mram_matrix1[size];
int __mram_noinit mram_matrix2[size];
int __mram_noinit mram_output[size];

int __dma_aligned wram_matrix1[size];
int __dma_aligned wram_matrix2[size];
int __dma_aligned wram_output[size];

// To create a barrier use BARRIER_INIT(barrier_name, NR_TASKLETS) (do this outside of the main function)
// To use the barrier, put barrier_wait(&barrier_name)

// This function should not be changed!

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

    // The arguments of this function will change on each step
    matrix_matrix_multiply(wram_matrix1,wram_matrix2,wram_output,16,16,16);

    // Write the values back to the mram buffers using mram_write(wram_buffer,mram_buffer,size_of_transaction_in_bytes);

    return 0;
}