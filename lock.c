#include <stdlib.h>
#include <MK64F12.h>
#include "utils.h"
#include "lock.h"

/**
 * Initialises the lock structure
 *
 * l pointer to lock to be initialised
 */
void l_init(lock_t* l)
{
	l->use = 0;
  l->blocked_queue = NULL;
}

void l_lock(lock_t* l) // This is an atomic action
{
	PIT->CHANNEL[0].TCTRL = 0b01; // enables Interrupts? i forgot the value
	if ( l->use == 0 )
		l->use = 1;
	else {
		current_process->blocked = 1;
		blocked_enqueue(current_process,l);
		process_blocked();
	}

	PIT->CHANNEL[0].TCTRL = 0b11; // enables Interrupts
}

void l_unlock(lock_t* l) // This is an atomic action
{
  PIT->CHANNEL[0].TCTRL = 0b01;
  l->use = 0;
	
	process_t* temp = blocked_dequeue(l);
  
	if ( temp != NULL ) {
			temp->blocked = 0;
			enqueue(temp);
	}
	PIT->CHANNEL[0].TCTRL = 0b11;
}

void blocked_enqueue(process_t* proc, lock_t* l) 
{
	if ( l->blocked_queue == NULL) { // Case 1, when process_queue is empty, we can make the enqueued process the head of the linked list.
		l->blocked_queue = proc;
		proc->next_process = NULL;
	}

	else {
		process_t* temp = l->blocked_queue; // Case 2, when process_queue is not empty, we must traverse to the end of the linked list.
		while ( temp->next_process != NULL ) {
			temp = temp->next_process;
		}
		temp->next_process = proc;
		proc->next_process = NULL;
	}
}

process_t* blocked_dequeue(lock_t* l)
{
	if (l->blocked_queue == NULL) // If nothing in process queue, we cannot dequeue anything
	  return NULL;
	process_t* temp = l->blocked_queue;
	l->blocked_queue = l->blocked_queue->next_process;
	temp->next_process = NULL; // breaks off a process ( represented by a node in the linked list ) from the process_queue.
	return temp;
}

