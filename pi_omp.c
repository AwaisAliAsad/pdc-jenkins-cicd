#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define PI_REF 3.14159265358979323846

/* Serial version: one thread does all the work */
static double pi_serial(long n) {
    double h = 1.0 / n, sum = 0.0;
    for (long i = 0; i < n; i++) {
        double x = (i + 0.5) * h;
        sum += 4.0 / (1.0 + x * x);
    }
    return sum * h;
}

/* Parallel version: loop split across threads, partial sums combined by reduction */
static double pi_parallel(long n) {
    double h = 1.0 / n, sum = 0.0;
    #pragma omp parallel for reduction(+:sum)
    for (long i = 0; i < n; i++) {
        double x = (i + 0.5) * h;
        sum += 4.0 / (1.0 + x * x);
    }
    return sum * h;
}

int main(int argc, char **argv) {
    long n = (argc > 1) ? atol(argv[1]) : 100000000L;
    int threads = omp_get_max_threads();

    double t0 = omp_get_wtime();
    double ps = pi_serial(n);
    double t1 = omp_get_wtime();
    double pp = pi_parallel(n);
    double t2 = omp_get_wtime();

    double ts = t1 - t0, tp = t2 - t1;
    printf("Steps (n)       : %ld\n", n);
    printf("Threads         : %d\n", threads);
    printf("Serial   pi     : %.12f  time %.4f s\n", ps, ts);
    printf("Parallel pi     : %.12f  time %.4f s\n", pp, tp);
    printf("Speedup         : %.2fx\n", ts / tp);
    printf("Efficiency      : %.1f%%\n", 100.0 * (ts / tp) / threads);

    int ok = fabs(pp - PI_REF) < 1e-6 && fabs(pp - ps) < 1e-9;
    printf("RESULT          : %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;   /* non-zero exit code makes the Jenkins stage fail */
}
