all: scalar sse fma

scalar: scalar.c
	gcc scalar.c -o scalar

sse: sse.c
	gcc sse.c -o sse

fma: fma.c
	gcc fma.c -mfma -o fma

clean:
	rm scalar sse fma
