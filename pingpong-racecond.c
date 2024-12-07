#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ppos.h"

// operating system check
#if defined(_WIN32) || (!defined(__unix__) && !defined(__unix) && (!defined(__APPLE__) || !defined(__MACH__)))
#warning Este codigo foi planejado para ambientes UNIX (LInux, *BSD, MacOS). A compilacao e execucao em outros ambientes e responsabilidade do usuario.
#endif

#define NUMTASKS 133
#define NUMSTEPS 39373

task_t task[NUMTASKS] ;
semaphore_t  s ;
long int soma = 0 ;
long int soma_correta = 0;

void taskBody(char *id)
{
   int i ;

  printf ("\n[%06d] %s iniciando...\n", systime(), id) ; fflush(stdout);
   for (i=0; i < NUMSTEPS; i++)
   {
      sem_down (&s) ;
      soma += 1 ;

      // espera ocupada para forçar preempção por tempo
      for(int x = (rand()%7+1)*133; x > 0; x--);
      
      sem_up (&s) ;
      if ((soma % 1000) == 0)
      printf("\nProgresso %ld/%ld [%s]", soma, soma_correta, id); fflush(stdout);
   }
   printf("\n%s repetiu %d vezes e terminou!", id, i); fflush(stdout);
   task_exit (0) ;
}

int main (int argc, char *argv[])
{
   int i ;
   
   printf ("[%06d] Main INICIO\n", systime()) ;

   ppos_init () ;

   sem_create (&s, 1) ;
   soma = 0;
   soma_correta = NUMTASKS;
   soma_correta *= NUMSTEPS;

   printf ("\n[%06d] criando %d tarefas...", systime(), NUMTASKS) ;

   for (i=0; i<NUMTASKS; i++) {
     char *task_name = malloc(20);
     memset(task_name, 0, 20);
     sprintf(task_name, "Task %d", i+2); // compatibilizando com o ID das tarefas
     //printf("\nCreating %s", task_name);
     task_create (&task[i], (void (*))taskBody, task_name);
   }
   printf ("\n[%06d] %d tarefas criadas e somando %d vezes cada, aguarde...\n", systime(), NUMTASKS, NUMSTEPS);
   fflush(stdout);

   for (i=0; i<NUMTASKS; i++)
     task_join (&task[i]) ;

   sem_destroy (&s) ;

   printf ("\n\n\n[%06d] Resultado Final: %ld", systime(), soma) ;
   
   if (soma == soma_correta)
     printf ("\n[%06d] SUCESSO!\n", systime(), soma) ;
   else
     printf ("\n[%06d] ERRO: Soma deu %ld, mas deveria ser %ld!\n",
             systime(), soma, soma_correta) ;

   task_exit (0) ;

   exit (0) ;
}
