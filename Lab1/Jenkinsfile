pipeline {
  agent any
  stages {
    stage('Build') {
      steps {
        sh 'make clean'
        sh 'make btest'
        sh '''#!/bin/bash
        if [[ $(./dlc bits.c | wc -l) -ne 0 ]]; then echo "Failed."; (exit 1); else echo "Great success"; (exit 0) fi
        '''
        sh './btest'
        script {
          grade = sh(script: './btest -g | tail -1', returnStdout: true)
          currentBuild.description = grade
        }
      }
    }
  }
}
