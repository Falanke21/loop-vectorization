#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <xmmintrin.h>
#include <immintrin.h>

// Perform the operation a[i] += b[i] * c[i];
// if a[i] is > 49
// with n iterations
void do_loop(int n, double *a, double *b, double *c)
{
	// Use a loop upper bound to handle the case when n is not
	// divisible by the vector length (when n is odd and vector is length 2)
	int upper_bound = n / 2 * 2;
	int i = 0;
	for (; i < upper_bound; i += 2)
	{
		if (a[i] > 49 && a[i + 1] > 49)
		{
			// Do vectorization
			__m128d vec_a = _mm_loadu_pd(&a[i]);
			__m128d vec_b = _mm_loadu_pd(&b[i]);
			__m128d vec_c = _mm_loadu_pd(&c[i]);
			vec_a = _mm_fmadd_pd(vec_b, vec_c, vec_a);
			_mm_store_pd(&a[i], vec_a);
		}
		else if (!(a[i] > 49) && !(a[i + 1] > 49))
		{
			// Do nothing
		}
		else
		{
			// Only 1 loop element should be updated
			// Do a loop unroll
			if (a[i] > 49)
			{
				a[i] += b[i] * c[i];
			}
			else
			{
				a[i + 1] += b[i + 1] * c[i + 1];
			}
		}
	}
	// Now handle the remainder of n / 2
	for (; i < n; i++)
	{
		if (a[i] > 49)
		{
			a[i] += b[i] * c[i];
		}
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
