all: scalar vector

scalar: scalar.c
	gcc scalar.c -o scalar

vector: vector.c
	gcc vector.c -mfma -o vector

clean:
	rm scalar vector

clean-all:
	rm scalar vector *.txt
