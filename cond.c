#include <stdlib.h>
#include <MK64F12.h>
#include "utils.h"
#include "cond.h"


void c_init(lock_t* l, cond_t* c) {
	l->use = 0;
	l->blocked_queue = NULL;
	
}



void c_wait(lock_t* l, cond_t* c) {
	PIT->CHANNEL[0].TCTRL = 0b01;
	
	
	
	
	
	PIT->CHANNEL[0].TCTRL = 0b11;
}



int c_waiting(lock_t* l, cond_t* c) {
	
	
	
	
	
}



void c_signal(lock_t* l, cond_t* c) {
	
	
	
	
	
	
}
