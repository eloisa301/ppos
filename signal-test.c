#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include<signal.h>

int parent_count = 5;
int child_count = 5;
pid_t pid; 

void sig_handler_parent(int signum){
  printf("[%d]\tParent : Received a response signal from child\n", parent_count);
  parent_count--;
}

void sig_handler_child(int signum){
  printf("[%d]\tChild : Received a signal from parent\n", child_count);
  child_count--;
  sleep(1);
  printf("[%d]\tChild : Sending a signal to parent\n", child_count);
  kill(getppid(),SIGUSR1);
}

int parent_child() {
  if((pid=fork())<0){
    printf("Fork Failed\n");
    exit(1);
  }
  /* Child Process */
  else if(pid==0){
    signal(SIGUSR1,sig_handler_child); // Register signal handler
    printf("Child: waiting for signal\n");
    pause();
  }
  /* Parent Process */
  else{
    signal(SIGUSR1,sig_handler_parent); // Register signal handler
    sleep(1);
    printf("Parent: sending signal to Child\n");
    while (parent_count != 0) {
        printf("[%d]\tParent : Sending signal to child\n", parent_count);
        kill(pid,SIGUSR1);      
        sleep(1);
    }
    printf("Parent: waiting to terminate\n");
    pause();
  }
  return 0;
}



void sig_handler_usr1(int signum){
  printf("[%d] USR 1 - signal received\n", parent_count);
  printf("[%d] USR 2 - sending signal\n", parent_count);
  raise(SIGUSR2);
}

void sig_handler_usr2(int signum){
  printf("[%d]\t\t\t USR 2 - signal received\n", parent_count);
  child_count = 1;
}

int signal_usr1_self () {
  signal(SIGUSR1,sig_handler_usr1); // Register signal handler
  signal(SIGUSR2,sig_handler_usr2); // Register signal handler
  printf("Inside main function\n");
  for (parent_count = 5; parent_count > 0; parent_count--)
  {
      child_count = 0;
      printf("[%d] USR 1 - sending signal\n", parent_count);
      raise(SIGUSR1);
      printf("[%d]\t\t\t USR 2 - waiting signal 2\n", parent_count);
      while (!child_count) {
          sleep(1);
      }
      printf("[%d, %d] *** next round\n", parent_count, child_count);
  }
  printf("Inside main function\n");
  return 0;
}


int main(){
  return signal_usr1_self();
}