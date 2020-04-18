#include "shared_structs.h"
#include "3140_concur.h"
#include <stdlib.h>
#include <MK64F12.h>
#include "utils.h"

process_t* current_process = NULL;
process_t* process_queue = NULL;

void enqueue(process_t* proc) 
{
	if ( process_queue == NULL) { // Case 1, when process_queue is empty, we can make the enqueued process the head of the linked list.
		process_queue = proc;
		proc->next_process = NULL;
	}

	else {
		process_t* temp = process_queue; // Case 2, when process_queue is not empty, we must traverse to the end of the linked list.
		while ( temp->next_process != NULL ) {
			temp = temp->next_process;
		}
		temp->next_process = proc;
		proc->next_process = NULL;
	}
}


process_t* dequeue()
{
	if (process_queue == NULL) // If nothing in process queue, we cannot dequeue anything
	  return NULL;
	process_t* temp = process_queue;
	process_queue = process_queue->next_process;
	temp->next_process = NULL; // breaks off a process ( represented by a node in the linked list ) from the process_queue.
	return temp;
}


int process_create( void (*f)(void), int n)
{	
	process_t* new_process = malloc(sizeof(process_t));

	if ( new_process == NULL )
	  return -1; // Allocation failed 

	new_process->sp = process_stack_init(f, n);
	new_process->size = n;
	new_process->blocked = 0;

	if ( new_process->sp == NULL ) {
	  return -1; //Allocation failed.
	}
	new_process->origsp = new_process->sp;

	enqueue(new_process);
	return 0;
} //What we just did is that we created a new process and appended it to the end of the process_queue.

void process_start() 
{
	NVIC_EnableIRQ(PIT0_IRQn); // Enable IRQHandler
	SIM->SCGC6 = SIM_SCGC6_PIT_MASK; //Enables clock to PIT module
	PIT->MCR = 0; //Enable the whole PIT module
	PIT->CHANNEL[0].TCTRL = 0b10; // enables Interrupts
	PIT->CHANNEL[0].LDVAL = 0x13FFA10;
	process_begin();
}


unsigned int* process_select(unsigned int* cursp) {
	if (current_process->blocked != 1 ) {
		if (current_process == NULL && cursp == NULL) { // when no process is currently running, dequeue
			current_process = dequeue();
		if (current_process == NULL ) 
			return NULL;
		else
		 return current_process->sp;
			}

		else if ( current_process != NULL && cursp == NULL ) { //when a process is finished running, free memory & dequeue
			process_stack_free(current_process->origsp, current_process->size);
			free(current_process);
			current_process = dequeue();
			return current_process->sp;
			}

		else if ( current_process != NULL && cursp != NULL ) { // process is in the middle of running, enqueue then dequeue
			current_process->sp = cursp;
			enqueue(current_process);
			current_process = dequeue();
			return current_process->sp;
			}
		}	
}