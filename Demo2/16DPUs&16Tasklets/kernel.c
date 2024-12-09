#include <mram.h>
#include <stdint.h>
#include <defs.h>
#include <barrier.h>

// Matrix size
#define ROWS 16
#define COLS 16
#define NR_TASKLETS 16

int __mram_noinit mram_matrix1[ROWS];
int __mram_noinit mram_matrix2[ROWS*COLS];
int __mram_noinit mram_output[ROWS];

int __dma_aligned wram_matrix1[ROWS];
int __dma_aligned wram_matrix2[ROWS*COLS];
int __dma_aligned wram_output[ROWS];

// Barriers creation
BARRIER_INIT(read_barrier, NR_TASKLETS);
BARRIER_INIT(write_barrier, NR_TASKLETS);

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

    int tasklet_id = me();

    // Tasklet 0 reads data from MRAM to WRAM
    if (tasklet_id == 0) {
        // Read the values from the mram buffers to the wram buffers
        mram_read(mram_matrix1, wram_matrix1, sizeof(mram_matrix1));
        mram_read(mram_matrix2, wram_matrix2, sizeof(mram_matrix2));
    }

    // Synchronize all tasklets before computation
    barrier_wait(&read_barrier);

    // Matrix multiplication (each DPU tasklets is responsible for 1 row & 1 col)
    matrix_matrix_multiply(wram_matrix1, &wram_matrix2[tasklet_id], &wram_output[tasklet_id], 1, 1, 16);

    // Synchronize all tasklets after computation
    barrier_wait(&write_barrier);

    // Tasklet 0 writes the values back to the mram buffers
    if (tasklet_id == 0) mram_write(wram_output, mram_output, sizeof(wram_output));

    return 0;
}