#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <immintrin.h>

// Perform the operation a[i] += b[i] * c[i];
// if a[i] is >= cutoff
// with n iterations
void do_loop(int n, double *a, double *b, double *c)
{
	// Use a loop upper bound to handle the case when n is not
	// divisible by the vector length (when n is odd and vector is length 2)
	int stride = 4;
	int cutoff = 20;
	int upper_bound = n / stride * stride;
	int i = 0;
	for (; i < upper_bound; i += stride)
	{
		if (a[i] >= cutoff && a[i + 1] >= cutoff && a[i + 2] >= cutoff && a[i + 3] >= cutoff)
		{
			__m256d vec_a = _mm256_loadu_pd(&a[i]);
			__m256d vec_b = _mm256_loadu_pd(&b[i]);
			__m256d vec_c = _mm256_loadu_pd(&c[i]);
			vec_a = _mm256_fmadd_pd(vec_b, vec_c, vec_a);
			_mm256_storeu_pd(&a[i], vec_a);
		}
		else if (!(a[i] >= cutoff) && !(a[i + 1] >= cutoff) && !(a[i + 3] >= cutoff) && !(a[i + 4] >= cutoff))
		{
			// Do nothing
		}
		else
		{
			// Only 1 loop element should be updated
			// Do loop unrolling
			if (a[i] >= cutoff)
			{
				a[i] += b[i] * c[i];
			}
			if (a[i + 1] >= cutoff)
			{
				a[i + 1] += b[i + 1] * c[i + 1];
			}
			if (a[i + 2] >= cutoff)
			{
				a[i + 2] += b[i + 2] * c[i + 2];
			}
			if (a[i + 3] >= cutoff)
			{
				a[i + 3] += b[i + 3] * c[i + 3];
			}
		}
	}
	// Now handle the remainder of n / stride
	for (; i < n; i++)
	{
		a[i] += b[i] * c[i];
	}
}

void print_result(double time_spent, int iterations)
{
	printf("Loop executed %f seconds with %d iterations\n", time_spent, iterations);
}

void print_average(double *times, int num_trials)
{
	double sum = 0;
	for (int i = 0; i < num_trials; i++)
	{
		sum += times[i];
	}
	printf("Average time: %f\n", sum / num_trials);
}

void write_to_file(double *times, int num_trials, const char *exe_path)
{
	// Get file name by argv[0]
	char *outfile_name = strrchr(exe_path, '/') + 1;
	outfile_name = strcat(outfile_name, ".txt");

	FILE *fp;
	fp = fopen(outfile_name, "w");

	for (int i = 0; i < num_trials; i++)
	{
		fprintf(fp, "%f,", times[i]);
	}

	fclose(fp);
}

double trial(int n)
{
	// Set up
	double *a = malloc(sizeof(double) * n);
	double *b = malloc(sizeof(double) * n);
	double *c = malloc(sizeof(double) * n);

	// Insert random numbers into the loop
	time_t t;
	srand((unsigned)time(&t));
	for (int i = 0; i < n; i++)
	{
		a[i] = rand() % 100;
		b[i] = rand() % 100;
		c[i] = rand() % 100;
	}

	// Target loop
	clock_t begin = clock();
	do_loop(n, a, b, c);
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	free(a);
	free(b);
	free(c);
	print_result(time_spent, n);
	return time_spent;
}

int main(int argc, char const *argv[])
{
	// Set up
	int num_trials = 10;
	int n = 100000000;
	if (argc > 1)
	{
		n = atoi(argv[1]);
	}

	double times[num_trials];
	for (int i = 0; i < num_trials; i++)
	{
		times[i] = trial(n);
	}
	print_average(times, num_trials);
	write_to_file(times, num_trials, argv[0]);

	return 0;
}
