#include <assert.h>
#include <dpu.h>
#include <dpu_log.h>
#include <stdio.h>

#include <time.h>
#define _POSIX_C_SOURCE 199309L

#ifndef DPU_BINARY
#define DPU_BINARY "./kernel"
#endif

int main(){
    struct dpu_set_t set, dpu;
    int matrix1[16*16];
    int matrix2[16*16];
    int output[16*16];
    int cpu_output[16*16];
    for(int i=0;i<16*16;i++){
        matrix1[i] = rand()%13;
        if(matrix1[i]>6){
            matrix1[i]-=12;
        }
    }
    for(int i=0;i<16*16;i++){
        matrix2[i] = rand()%13;
        if(matrix2[i]>6){
            matrix2[i]-=12;
        }
    }
    printf("Initial matrices:\n");
    printf("Matrix1:\n");
    for(int i=0;i<16;i++){
        for(int j=0;j<16;j++){
            printf("%d ",matrix1[i*16+j]);
        }
        printf("\n");
    }
    printf("Matrix2:\n");
    for(int i=0;i<16;i++){
        for(int j=0;j<16;j++){
            printf("%d ",matrix2[i*16+j]);
        }
        printf("\n");
    }
    printf("\n");

    // Number of DPUs
    int nr_dpus = 16;

    DPU_ASSERT(dpu_alloc(nr_dpus, NULL, &set));
    DPU_ASSERT(dpu_load(set, DPU_BINARY, NULL));

    // Send the required data to the DPUs
    // Send 1 matrix row to each DPU
    int dpu_id = 0;
    DPU_FOREACH(set, dpu) {
        DPU_ASSERT(dpu_prepare_xfer(dpu, &matrix1[dpu_id*nr_dpus]));
        dpu_id++;
    }
    DPU_ASSERT(dpu_push_xfer(set, DPU_XFER_TO_DPU, "mram_matrix1", 0, sizeof(matrix1)/nr_dpus, DPU_XFER_DEFAULT));

    // Send the entire matrix for all DPUs
    DPU_ASSERT(dpu_broadcast_to(set, "mram_matrix2", 0, matrix2, sizeof(matrix2), DPU_XFER_DEFAULT));

    // DPU lauch
    DPU_ASSERT(dpu_launch(set, DPU_SYNCHRONOUS));

    // Receive the required data from the DPUs (1 row per DPU)
    dpu_id = 0;
    DPU_FOREACH(set, dpu) {
        DPU_ASSERT(dpu_prepare_xfer(dpu, &output[dpu_id*nr_dpus]));
        dpu_id++;
    }
    DPU_ASSERT(dpu_push_xfer(set, DPU_XFER_FROM_DPU, "mram_output", 0, sizeof(matrix1)/nr_dpus, DPU_XFER_DEFAULT));

    // Getting the times
    uint32_t nb_cycles, clocks_per_sec;
    DPU_FOREACH(set, dpu) {
        DPU_ASSERT(dpu_copy_from(dpu, "nb_cycles", 0, &nb_cycles, sizeof(nb_cycles)));
        DPU_ASSERT(dpu_copy_from(dpu, "CLOCKS_PER_SEC", 0, &clocks_per_sec, sizeof(clocks_per_sec)));
    }

    for(int i=0;i<16;i++){
        for(int j=0;j<16;j++){
            int accumulator=0;
            for(int k=0;k<16;k++){
                accumulator += matrix1[i*16+k]*matrix2[k*16+j];
            }
            cpu_output[i*16+j]=accumulator;
        }
    }
    int errors=0;

    printf("CPU Output:\n");
    for(int i=0;i<16;i++){
        for(int j=0; j<16;j++){
            printf("%d ",cpu_output[i*16+j]);
        }
        printf("\n");
    }
    printf("\n");

    printf("UPMEM PIM Output:\n");
    for(int i=0;i<16;i++){
        for(int j=0; j<16;j++){
            printf("%d ",output[i*16+j]);
            if(output[i*16+j]!=cpu_output[i*16+j]){
                errors++;
            }
        }
        printf("\n");
    }
    printf("\n");

    printf("Errors:%d out of 256\n",errors);

    printf("DPU cycles: %u\n", nb_cycles);
    printf("DPU time: %.2e secs.\n", (double)nb_cycles / clocks_per_sec);

    dpu_free(set);

}