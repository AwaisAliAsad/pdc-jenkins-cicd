# Viva Notes (quick answers)

**What is CI/CD?** CI (Continuous Integration) automatically builds and tests every code change. CD (Continuous Delivery/Deployment) automatically releases or runs the tested build.

**What is Jenkins?** An open-source automation server, written in Java, that runs pipelines (build, test, deploy).

**What is a Jenkinsfile?** A pipeline written as code (Groovy, declarative syntax), stored in the repository with the program. Its sections are `pipeline`, `agent`, `stages`, `steps` and `post`.

**What is a webhook?** An HTTP POST request that GitHub sends to a URL when an event happens, such as a push. It is push-based, not polling.

**Why `/github-webhook/`?** It is the endpoint the Jenkins GitHub plugin listens on.

**Polling vs webhook?** With polling, Jenkins checks GitHub every few minutes, which is wasteful and slow. With a webhook, GitHub notifies Jenkins instantly.

**What is ngrok?** A reverse-tunnel tool. The ngrok agent opens an outbound connection to ngrok's servers and gets a public HTTPS URL. Requests to that URL are forwarded to localhost:8080.

**Why ngrok if EC2 has a public IP?** It is not strictly needed. ngrok was used because it gives HTTPS, keeps port 8080 closed to the public, and keeps the URL stable when the EC2 IP changes.

**Why EC2 and not a laptop?** It runs 24/7 and is reachable from anywhere. This is a distributed setup: the code is on GitHub, the build runs on the cloud, and the developer works from a laptop.

**Security group?** A virtual firewall for the instance. We allowed 22 and 8080 from My IP only.

**Why Java 21?** Current Jenkins LTS needs Java 21 or later.

**Why the signing key?** apt uses it to verify that the Jenkins package is genuine and has not been tampered with.

**What is OpenMP?** A shared-memory parallel programming API for C/C++/Fortran, based on compiler directives (`#pragma omp`).

**`#pragma omp parallel for reduction(+:sum)`?** It splits the loop iterations among threads. Each thread keeps a private `sum`, and the partial sums are added together at the end. This avoids a race condition.

**Race condition?** Two threads update shared data at the same time, so the result is wrong. Reduction, `critical` or `atomic` prevent it.

**Speedup / efficiency?** Speedup = T_serial / T_parallel. Efficiency = Speedup / p (the number of threads).

**Amdahl's law?** Speedup <= 1 / ((1 - f) + f/p), where f is the parallel fraction. The serial part limits the maximum speedup.

**Why is speedup below 2 on 2 vCPUs?** Thread creation overhead, memory bandwidth limits, and the fact that a vCPU may be a hyperthread rather than a full core.

**OpenMP vs MPI?** OpenMP uses shared memory and threads on one machine. MPI uses distributed memory and processes that pass messages across machines.

**How does a failed test stop the pipeline?** The program returns exit code 1, the `sh` step fails, and Jenkins marks the build FAILURE and skips the remaining stages.

**What are artifacts?** Files saved from a build (here `pi_omp` and `output.txt`) that can be downloaded from Jenkins later.
