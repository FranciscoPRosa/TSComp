#include <assert.h>
#include <dpu.h>
#include <dpu_log.h>
#include <stdio.h>

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
    int nr_dpus = 1;

    DPU_ASSERT(dpu_alloc(nr_dpus, NULL, &set));
    DPU_ASSERT(dpu_load(set, DPU_BINARY, NULL));

    // Send the required data to the DPUs
    DPU_FOREACH(set, dpu) {
        DPU_ASSERT(dpu_copy_to(dpu, "mram_matrix1", 0, matrix1, sizeof(matrix1)));
        DPU_ASSERT(dpu_copy_to(dpu, "mram_matrix2", 0, matrix2, sizeof(matrix2)));
    }

    // DPU lauch
    DPU_ASSERT(dpu_launch(set, DPU_SYNCHRONOUS));

    // Receive the required data from the DPUs
    DPU_FOREACH(set, dpu) {
        DPU_ASSERT(dpu_copy_from(dpu, "mram_output", 0, output, sizeof(output)));
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

    dpu_free(set);

}