#include <stdio.h>
#include <stdlib.h>
#include "ppos.h"
#include "ppos-core-globals.h"

task_t      a1, a2, b1, b2;
semaphore_t s1, s2 ;

void TaskA (void * arg)
{
   int i ;
   printf("\nIniciando: %s\n", (char*)arg); fflush(stdout);
   for (i=0; i<10; i++)
   {
      sem_down(&s1) ;
      printf ("\n[%06d] %s zig (%d)", systime(), (char *) arg, i) ; fflush(stdout);
      sem_up (&s2) ;
      task_yield();
   }
   task_exit (0) ;
}

void TaskB (void * arg)
{
   int i ;
   printf("\nIniciando: %s\n", (char*)arg); fflush(stdout);
   for (i=0; i<10; i++)
   {
      sem_down (&s2) ;
      printf ("\n[%06d] %s zag (%d)", systime(), (char *) arg, i) ; fflush(stdout);
      sem_up (&s1) ;
      //task_sleep (1) ;
      task_yield();
   }
   task_exit (0) ;
}

int main (int argc, char *argv[])
{
   printf ("Main INICIO\n") ;

   ppos_init();
   
   sem_create (&s1, 1) ;
   sem_create (&s2, 0) ;

   task_create (&a1, TaskA, "A1") ;
   printf ("Task A1 created...\n") ;

   task_create (&a2, TaskA, "    A2") ;
   printf ("Task A2 created...\n") ;
   
   task_create (&b1, TaskB, "                         B1") ;
   printf ("Task B1 created...\n") ;
   
   task_create (&b2, TaskB, "                             B2") ;
   printf ("Task B2 created...\n") ;
   
   printf ("Main waits for A1...\n") ;
   task_join (&a1) ;
   sem_destroy (&s1) ;
   sem_destroy (&s2) ;
   printf ("Main waits for A2...\n") ;
   task_join (&a2) ;
   printf ("Main waits for B1...\n") ;
   task_join (&b1) ;
   printf ("Main waits for B2...\n") ;
   task_join (&b2) ;

   printf ("Main FIM\n") ;
   task_exit (0) ;

   exit (0) ;
}
