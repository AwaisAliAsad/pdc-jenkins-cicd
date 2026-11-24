# CI/CD Pipeline with GitHub Webhook, Jenkins on AWS EC2, and ngrok

**Course:** Parallel & Distributed Computing (PDC)
**Student:** Awais Ali Asad
**Repository:** https://github.com/AwaisAliAsad/pdc-jenkins-cicd

## 1. Objective
Build a CI/CD pipeline in which every push to a GitHub repository automatically triggers a Jenkins pipeline. Jenkins runs on an AWS EC2 instance, and the pipeline compiles, tests and runs a parallel OpenMP program.

## 2. Architecture

```
Developer --git push--> GitHub --webhook (HTTPS POST)--> ngrok public URL
                                                             |
                                                       (secure tunnel)
                                                             v
                                   AWS EC2 (Ubuntu): Jenkins on port 8080
                                   Checkout -> Build -> Test -> Run/Deploy
```

## 3. Environment
| Item | Value |
|---|---|
| Cloud | AWS EC2, region ap-southeast-2 (Sydney) |
| Instance type | c7i-flex.large (2 vCPU, 4 GB RAM) |
| OS | Ubuntu Server |
| Java | OpenJDK 21 |
| Jenkins | 2.580.2 LTS (official Debian stable repo, jenkins.io-2026 signing key) |
| Compiler | GCC 15.2 with OpenMP (`-fopenmp`) |
| Tunnel | ngrok 3.39 (free plan) |

## 4. Steps performed
1. **EC2 setup.** Connected over SSH with a key pair, and restricted SSH (port 22) in the security group to my IP only. Changed the instance type from t3.micro (1 GB) to c7i-flex.large, because Jenkins needs more memory.
2. **Java and Jenkins.** Installed OpenJDK 21, added the official Jenkins LTS apt repository and its signing key, installed Jenkins, and enabled it with `systemctl enable --now jenkins`.
3. **Jenkins setup.** Opened port 8080 to my IP only, unlocked Jenkins with the initial admin password, installed the suggested plugins (Git, GitHub, Pipeline), and created an admin user.
4. **Build tools.** Installed `build-essential` (GCC with OpenMP support). `nproc` reported 2 cores.
5. **GitHub repository.** Added `pi_omp.c` (the OpenMP program) and a `Jenkinsfile` (a declarative pipeline).
6. **Jenkins job.** Created a Pipeline job using "Pipeline script from SCM" (Git, branch `*/main`, script path `Jenkinsfile`) and enabled **GitHub hook trigger for GITScm polling**.
7. **ngrok.** Installed ngrok on EC2, added the authtoken, and ran `ngrok http 8080`. This gave a public HTTPS URL that forwards to Jenkins.
8. **Webhook.** In GitHub I set Payload URL `https://<ngrok-domain>/github-webhook/`, content type `application/json`, and the push event only. GitHub showed a successful delivery.
9. **Verification.** After a push to `main`, Jenkins started a new build automatically ("Started by GitHub push"), and all four stages passed.

## 5. Pipeline stages
| Stage | What it does |
|---|---|
| Checkout | Pulls the latest commit from GitHub (`checkout scm`) |
| Build | `gcc -O2 -fopenmp pi_omp.c -o pi_omp -lm` |
| Test | Runs a small input. The program exits non-zero if the parallel result is wrong, and that fails the build |
| Run/Deploy | Runs with 200 million steps on 2 threads, saves `output.txt`, and archives the binary and output as artifacts |

## 6. Parallel program (OpenMP)
The program computes pi as the integral of 4/(1+x^2) from 0 to 1, using the midpoint rule. It runs once serially and once with `#pragma omp parallel for reduction(+:sum)`. It then prints both times, the **speedup** (serial time / parallel time) and the **efficiency** (speedup / threads), and checks that the result is correct.

## 7. Why ngrok when EC2 has a public IP?
The EC2 instance already has a public IP, so GitHub could reach Jenkins directly if port 8080 were opened to GitHub's webhook IP ranges. ngrok was used instead because:
- the assignment requires it;
- it gives an **HTTPS** URL without setting up certificates;
- port 8080 can stay open to my IP only, because ngrok makes an **outbound** connection and needs no inbound rule;
- the URL does not change when the EC2 public IP changes after a stop and start.

## 8. Security measures
- SSH (22) and Jenkins (8080) are allowed only from my IP, never from 0.0.0.0/0.
- Jenkins is protected with an admin login. The initial password is invalid once setup is finished.
- Jenkins packages are verified with the official signing key.
- The ngrok authtoken and the key pair should be rotated after submission.

## 9. Result
A push to GitHub automatically triggers the Jenkins pipeline on EC2, which compiles, tests and runs the parallel program with no manual action.
