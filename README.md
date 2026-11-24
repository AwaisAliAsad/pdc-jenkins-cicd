# PDC CI/CD Pipeline: GitHub → Jenkins on AWS EC2

When code is pushed to this repo, a GitHub webhook (through ngrok) triggers a Jenkins pipeline on an AWS EC2 instance. The pipeline builds, tests and runs an OpenMP parallel program.

**Pipeline stages:** Checkout → Build → Test → Run/Deploy

**Program:** `pi_omp.c` computes pi serially and in parallel with OpenMP (`reduction(+:sum)`), then prints the speedup and efficiency.

**Build locally:**
```bash
gcc -O2 -fopenmp pi_omp.c -o pi_omp -lm
OMP_NUM_THREADS=2 ./pi_omp 200000000
```

See [REPORT.md](REPORT.md) and [VIVA_NOTES.md](VIVA_NOTES.md).
