#include "queue.h"
#include <stdio.h>

void queue_append (queue_t **queue, queue_t *elem) {
    if (queue == NULL) 
        printf("a fila não existe");
    if (elem == NULL)
        printf("o elemento não existe");
    if (elem->prev != NULL || elem->next != NULL)
        printf("o elemnento está em outra fila")
        
    if (*queue == NULL) {
        *queue = elem;
        elem->prev = elem->next = NULL;
    } else {
        queue_t *last = *queue;
        while (last->next != NULL) {
            last = last->next;
        }

        last->next = elem;
        elem->prev = last;
        elem->next = NULL;
    }
    
}