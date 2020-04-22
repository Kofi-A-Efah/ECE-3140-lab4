#ifndef __SHARED_STRUCTS_H__
#define __SHARED_STRUCTS_H__

/** Implement your structs here */

/**
 * This structure holds the process structure information
 */
typedef struct process_state // Our PCB is represented as a node in a linked list. A node contains the following variables.
{
	unsigned int* sp;
	unsigned int size;
	unsigned int* origsp;
	struct process_state* next_process;
	int blocked;
} process_t;

/**
 * This defines the lock structure
 */
typedef struct lock_state {
	int use; // 1 if lock in use, 0 if lock is not in use
	process_t* blocked_queue;
} lock_t;

/**
 * This defines the conditional variable structure
 */
typedef struct cond_var {

} cond_t;

void blocked_enqueue(process_t* proc, lock_t* l);
void enqueue(process_t* proc);
process_t* blocked_dequeue(lock_t* l);
process_t* dequeue();

#endif
