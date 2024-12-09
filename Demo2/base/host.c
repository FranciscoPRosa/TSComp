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

    // Set nr_dpus to the number of DPUs will need on each step

    DPU_ASSERT(dpu_alloc(nr_dpus, NULL, &set));
    DPU_ASSERT(dpu_load(set, DPU_BINARY, NULL));

    // Send the required data to the DPUs using one of the following methods:

    // DPU_FOREACH(set, dpu) {
    //     dpu_copy_to(dpu, "variable_name_in_kernel", 0, pointer_to_buffer_start, size_of_transaction_in_bytes);
    // }

    // dpu_broadcast_to(set, "variable_name_in_kernel", 0, pointer_to_buffer_start, size_of_transaction_in_bytes, DPU_XFER_DEFAULT);

    // DPU_FOREACH(set, dpu, each_dpu) {
    //     DPU_ASSERT(dpu_prepare_xfer(dpu, &pointer_to_buffer_start));
    // }

    // DPU_ASSERT(dpu_push_xfer(set, DPU_XFER_TO_DPU, "variable_name_in_kernel", 0, size_of_transaction_in_bytes, DPU_XFER_DEFAULT));

    DPU_ASSERT(dpu_launch(set, DPU_SYNCHRONOUS));

    // Receive the required data from the DPUs using one of the following methods:

    // DPU_FOREACH(set, dpu) {
    //     dpu_copy_from(dpu, "variable_name_in_kernel", 0, pointer_to_buffer_start, size_of_transaction_in_bytes);
    // }

    // DPU_FOREACH(set, dpu, each_dpu) {
    //     DPU_ASSERT(dpu_prepare_xfer(dpu, &pointer_to_buffer_start));
    // }

    // DPU_ASSERT(dpu_push_xfer(set, DPU_XFER_FROM_DPU, "variable_name_in_kernel", 0, size_of_transaction_in_bytes, DPU_XFER_DEFAULT));

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