#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Perform the operation a[i] += b[i] * c[i];
// with n iterations
void do_loop(int n, double *a, double *b, double *c)
{
	for (int i = 0; i < n; i++)
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
	struct timespec start, end;
	clock_gettime(CLOCK_REALTIME, &start);
	do_loop(n, a, b, c);
	clock_gettime(CLOCK_REALTIME, &end);
	double time_spent = (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

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
