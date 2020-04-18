#include "shared_structs.h"
#include <stdlib.h>
#include <MK64F12.h>
#include "utils.h"
#include "3140_concur.h"
#include "process.c"


/**
 * Initialises the lock structure
 *
 * l pointer to lock to be initialised
 */
void l_init(lock_t* l)
{
	l = malloc(sizeof(lock_t));
	l->use = 0;
  l->blocked_queue = NULL;	
}

void l_lock(lock_t* l) // This is an atomic action
{
	PIT->CHANNEL[0].TCTRL = 0b11; // enables Interrupts? i forgot the value
	if ( l->use == 0 )
		l->use = 1;
	else {
		current_process->blocked = 1;
		enqueue(current_process);
		process_blocked();
	}
	
	PIT->CHANNEL[0].TCTRL = 0b10; // enables Interrupts
}

void l_unlock(lock_t* l) // This is an atomic action
{
	if ( l->blocked_queue == NULL )
		l->use = 0;	
	else {
		current_process = dequeue(); // dequeue from blocked_queue
		current_process->blocked = 0;
		
	}	
}
