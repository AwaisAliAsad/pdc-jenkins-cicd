pipeline {
    agent any

    environment {
        OMP_NUM_THREADS = '2'   // use both vCPUs of the EC2 instance
    }

    stages {
        stage('Checkout') {
            steps {
                checkout scm
                sh 'ls -la'
            }
        }

        stage('Build') {
            steps {
                sh 'gcc -O2 -fopenmp pi_omp.c -o pi_omp -lm'
            }
        }

        stage('Test') {
            steps {
                // small run: program exits non-zero if the parallel answer is wrong
                sh './pi_omp 1000000'
            }
        }

        stage('Run/Deploy') {
            steps {
                sh 'nproc'
                sh './pi_omp 200000000 | tee output.txt'
                archiveArtifacts artifacts: 'pi_omp, output.txt', fingerprint: true
            }
        }
    }

    post {
        success { echo 'Pipeline SUCCESS: parallel program built, tested and ran.' }
        failure { echo 'Pipeline FAILED: check the stage logs above.' }
    }
}
