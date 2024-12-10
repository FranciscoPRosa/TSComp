all: kernel host.c
	gcc -std=c99 -o host host.c `dpu-pkg-config --cflags --libs dpu`

kernel: kernel.c
	dpu-upmem-dpurte-clang -DNR_TASKLETS=16 -o kernel kernel.c

clean:
	rm host kernel