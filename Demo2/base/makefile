all: kernel host.c
	gcc -std=c99 -o host.o host.c `dpu-pkg-config --cflags --libs dpu`

kernel: kernel.c
	dpu-upmem-dpurte-clang -o kernel kernel.c

clean:
	rm *.o kernel host