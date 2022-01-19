#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Perform the operation a[i] += b[i] * c[i];
// if a[i] is >= 50
// with n iterations
void do_loop(int n, double *a, double *b, double *c)
{
	for (int i = 0; i < n; i++)
	{
		if (a[i] >= 50)
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

	return 0;
}
