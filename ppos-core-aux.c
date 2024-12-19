#include "ppos.h"
// #include "ppos-core-globals.h"
#include "ppos-disk-manager.h"

// ****************************************************************************
// Coloque as suas modificações aqui,
// p.ex. includes, defines variáveis, // estruturas e funções
task_t *taskExec;   // Ponteiro para a TCB da tarefa em execucao
task_t *readyQueue; // Ponteiro para a fila de tarefas prontas
task_t *sleepQueue;

void queue_add(task_t **queue, task_t *elem);
void queue_remove(task_t **queue, task_t *elem);

int sem_create(semaphore_t *s, int value)
{
    if (s == NULL)
    {
        return -1;
    }
    // printf("teste\n");
    s->counter = value;
    s->queue = NULL;
    return 0;
}

int sem_down(semaphore_t *s)
{
    s->counter = s->counter - 1;
    if (s->counter < 0)
    {
        queue_add(&s->queue, taskExec);
        task_suspend(taskExec, &(s->queue));
        return 0;
    }
    return 0;
}

int sem_up(semaphore_t *s)
{
    // printf("teste");
    s->counter = s->counter + 1;
    if (s->counter <= 0)
    {
        task_resume(s->queue);
    }
    return 0;
}

int sem_destroy(semaphore_t *s)
{
    /*while (s->queue != NULL)
    {
        task_resume(s->queue); // Devolve a Tarefa a fila de prontas, implementar
    }
    return 0;*/
}

int task_create(task_t *task, void (*start_func)(void *), void *arg)
{
    if (readyQueue == NULL)
    {
        task_t *aux;
        readyQueue = aux;
        taskExec = task;
        task->state = PPOS_TASK_STATE_EXECUTING;
        return 1;
    }
    // void (*start_func);
    task->state = PPOS_TASK_STATE_READY;
    queue_add(&readyQueue, task);
    return 1;
}

void task_yield()
{
}

void task_exit(int exitCode)
{
}

int task_join(task_t *task)
{
    if (task == NULL || task->state == PPOS_TASK_STATE_TERMINATED)
    {
        return task->exitCode;
    }
    task_suspend(readyQueue, &readyQueue);
    task_resume(task);
    return task->exitCode;
}
void task_suspend(task_t *task, task_t **queue)
{
    if (task->state == PPOS_TASK_STATE_READY)
    {
        task->state = PPOS_TASK_STATE_SUSPENDED;
        queue_remove(&readyQueue, task);
    }
    queue_add(&sleepQueue, task);
    task->state = PPOS_TASK_STATE_SUSPENDED;
}
void task_resume(task_t *task)
{
    if (task->state == PPOS_TASK_STATE_SUSPENDED)
    {
        queue_remove(&sleepQueue, task);
    }
    task->state = PPOS_TASK_STATE_READY;
    queue_add(&readyQueue, task);
}
unsigned int systime()
{
}

void ppos_init()
{
    setvbuf(stdout, 0, _IONBF, 0);
    // printf("teste\n");
    taskExec = NULL;
    readyQueue = NULL;
    sleepQueue = NULL;
}

void queue_add(task_t **queue, task_t *elem)
{
    if (queue == NULL)
        printf("a fila não existe\n");
    if (elem == NULL)
        printf("o elemento não existe\n");
    if (elem->prev != NULL || elem->next != NULL)
        printf("o elemento está em outra fila\n");

    if (*queue == NULL)
    {
        *queue = elem;
        elem->prev = elem->next = NULL;
    }
    else
    {
        task_t *last = *queue;
        while (last->next != NULL)
        {
            last = last->next;
        }

        last->next = elem;
        elem->prev = last;
        elem->next = NULL;
    }
    // printf("teste");
}

void queue_remove(task_t **queue, task_t *elem)
{
    if (queue == NULL)
        printf("a fila não existe\n");
    if (elem == NULL)
        printf("o elemento não existe\n");

    if (*queue == NULL)
    {
        *queue = elem;
        elem->prev = elem->next = NULL;
    }
    else
    {
        task_t *last = *queue;
        while (last != elem && last != NULL)
        {
            last = last->next;
        }
        if (last == NULL)
        {
            printf("Elemento nao encontrado");
            return;
        }
        if (last->next == NULL)
        {
            last->prev->next = NULL;
            last->prev = NULL;
            return;
        }
        last->prev->next = last->next;
        last->prev = NULL;
        last->next = NULL;
    }
    // printf("teste");
}

void before_ppos_init()
{
    // put your customization here
#ifdef DEBUG
    printf("\ninit - BEFORE");
#endif
}

void after_ppos_init()
{
    // put your customization here
#ifdef DEBUG
    printf("\ninit - AFTER");
#endif
}

void before_task_create(task_t *task)
{
    // put your customization here
#ifdef DEBUG
    printf("\ntask_create - BEFORE - [%d]", task->id);
#endif
}

void after_task_create(task_t *task)
{
    // put your customization here
#ifdef DEBUG
    printf("\ntask_create - AFTER - [%d]", task->id);
#endif
}

void before_task_exit()
{
    // put your customization here
#ifdef DEBUG
    printf("\ntask_exit - BEFORE - [%d]", taskExec->id);
#endif
}

void after_task_exit()
{
    // put your customization here
#ifdef DEBUG
    printf("\ntask_exit - AFTER- [%d]", taskExec->id);
#endif
}

void before_task_switch(task_t *task)
{
    // put your customization here
#ifdef DEBUG
    printf("\ntask_switch - BEFORE - [%d -> %d]", taskExec->id, task->id);
#endif
}

void after_task_switch(task_t *task)
{
    // put your customization here
#ifdef DEBUG
    printf("\ntask_switch - AFTER - [%d -> %d]", taskExec->id, task->id);
#endif
}

void before_task_yield()
{
    // put your customization here
#ifdef DEBUG
    printf("\ntask_yield - BEFORE - [%d]", taskExec->id);
#endif
}
void after_task_yield()
{
    // put your customization here
#ifdef DEBUG
    printf("\ntask_yield - AFTER - [%d]", taskExec->id);
#endif
}

void before_task_suspend(task_t *task)
{
    // put your customization here
#ifdef DEBUG
    printf("\ntask_suspend - BEFORE - [%d]", task->id);
#endif
}

void after_task_suspend(task_t *task)
{
    // put your customization here
#ifdef DEBUG
    printf("\ntask_suspend - AFTER - [%d]", task->id);
#endif
}

void before_task_resume(task_t *task)
{
    // put your customization here
#ifdef DEBUG
    printf("\ntask_resume - BEFORE - [%d]", task->id);
#endif
}

void after_task_resume(task_t *task)
{
    // put your customization here
#ifdef DEBUG
    printf("\ntask_resume - AFTER - [%d]", task->id);
#endif
}

void before_task_sleep()
{
    // put your customization here
#ifdef DEBUG
    printf("\ntask_sleep - BEFORE - [%d]", taskExec->id);
#endif
}

void after_task_sleep()
{
    // put your customization here
#ifdef DEBUG
    printf("\ntask_sleep - AFTER - [%d]", taskExec->id);
#endif
}

int before_task_join(task_t *task)
{
    // put your customization here
#ifdef DEBUG
    printf("\ntask_join - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_task_join(task_t *task)
{
    // put your customization here
#ifdef DEBUG
    printf("\ntask_join - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_sem_create(semaphore_t *s, int value)
{
    // put your customization here
#ifdef DEBUG
    printf("\nsem_create - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_sem_create(semaphore_t *s, int value)
{
    // put your customization here
#ifdef DEBUG
    printf("\nsem_create - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_sem_down(semaphore_t *s)
{
    // put your customization here
#ifdef DEBUG
    printf("\nsem_down - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_sem_down(semaphore_t *s)
{
    // put your customization here
#ifdef DEBUG
    printf("\nsem_down - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_sem_up(semaphore_t *s)
{
    // put your customization here
#ifdef DEBUG
    printf("\nsem_up - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_sem_up(semaphore_t *s)
{
    // put your customization here
#ifdef DEBUG
    printf("\nsem_up - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_sem_destroy(semaphore_t *s)
{
    // put your customization here
#ifdef DEBUG
    printf("\nsem_destroy - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_sem_destroy(semaphore_t *s)
{
    // put your customization here
#ifdef DEBUG
    printf("\nsem_destroy - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_mutex_create(mutex_t *m)
{
    // put your customization here
#ifdef DEBUG
    printf("\nmutex_create - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_mutex_create(mutex_t *m)
{
    // put your customization here
#ifdef DEBUG
    printf("\nmutex_create - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_mutex_lock(mutex_t *m)
{
    // put your customization here
#ifdef DEBUG
    printf("\nmutex_lock - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_mutex_lock(mutex_t *m)
{
    // put your customization here
#ifdef DEBUG
    printf("\nmutex_lock - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_mutex_unlock(mutex_t *m)
{
    // put your customization here
#ifdef DEBUG
    printf("\nmutex_unlock - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_mutex_unlock(mutex_t *m)
{
    // put your customization here
#ifdef DEBUG
    printf("\nmutex_unlock - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_mutex_destroy(mutex_t *m)
{
    // put your customization here
#ifdef DEBUG
    printf("\nmutex_destroy - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_mutex_destroy(mutex_t *m)
{
    // put your customization here
#ifdef DEBUG
    printf("\nmutex_destroy - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_barrier_create(barrier_t *b, int N)
{
    // put your customization here
#ifdef DEBUG
    printf("\nbarrier_create - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_barrier_create(barrier_t *b, int N)
{
    // put your customization here
#ifdef DEBUG
    printf("\nbarrier_create - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_barrier_join(barrier_t *b)
{
    // put your customization here
#ifdef DEBUG
    printf("\nbarrier_join - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_barrier_join(barrier_t *b)
{
    // put your customization here
#ifdef DEBUG
    printf("\nbarrier_join - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_barrier_destroy(barrier_t *b)
{
    // put your customization here
#ifdef DEBUG
    printf("\nbarrier_destroy - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_barrier_destroy(barrier_t *b)
{
    // put your customization here
#ifdef DEBUG
    printf("\nbarrier_destroy - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_mqueue_create(mqueue_t *queue, int max, int size)
{
    // put your customization here
#ifdef DEBUG
    printf("\nmqueue_create - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_mqueue_create(mqueue_t *queue, int max, int size)
{
    // put your customization here
#ifdef DEBUG
    printf("\nmqueue_create - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_mqueue_send(mqueue_t *queue, void *msg)
{
    // put your customization here
#ifdef DEBUG
    printf("\nmqueue_send - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_mqueue_send(mqueue_t *queue, void *msg)
{
    // put your customization here
#ifdef DEBUG
    printf("\nmqueue_send - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_mqueue_recv(mqueue_t *queue, void *msg)
{
    // put your customization here
#ifdef DEBUG
    printf("\nmqueue_recv - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_mqueue_recv(mqueue_t *queue, void *msg)
{
    // put your customization here
#ifdef DEBUG
    printf("\nmqueue_recv - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_mqueue_destroy(mqueue_t *queue)
{
    // put your customization here
#ifdef DEBUG
    printf("\nmqueue_destroy - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_mqueue_destroy(mqueue_t *queue)
{
    // put your customization here
#ifdef DEBUG
    printf("\nmqueue_destroy - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_mqueue_msgs(mqueue_t *queue)
{
    // put your customization here
#ifdef DEBUG
    printf("\nmqueue_msgs - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_mqueue_msgs(mqueue_t *queue)
{
    // put your customization here
#ifdef DEBUG
    printf("\nmqueue_msgs - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

// Essa função implemeneta o escalonador de requisicoes de
// leitura/scrita do disco usado pelo gerenciador do disco
// A função implementa a política FCFS.
/*diskrequest_t* disk_scheduler(diskrequest_t* queue) {
     // FCFS scheduler
    if ( queue != NULL ) {
        PPOS_PREEMPT_DISABLE
        diskrequest_t* request = queue;
        PPOS_PREEMPT_ENABLE
        return request;
    }
    return NULL;
}
*/
