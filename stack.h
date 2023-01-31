/********************************************************************************
* stack.h: Contains function declarations and macro definitions for 
*          implementation of a 1 kB stack.
********************************************************************************/
#ifndef STACK_H_
#define STACK_H_

/* Include directives: */
#include "cpu.h"

/* Macro definitions: */
#define STACK_ADDRESS_WIDTH 1024 /* 1024 unique addresses. */
#define STACK_DATA_WIDTH    8    /* 8 bits storage capacity per address. */

/********************************************************************************
* stack_reset: Clears content of the stack and sets the stack pointer to the
*              top of the stack.
********************************************************************************/
void stack_reset(void);

/********************************************************************************
* stack_push: Pushes value to the bottom of the stack. If the stack is full,
*             the value isn't pushed and error code 1 is returned. Otherwise
*             return code 0 is returned after successful push and the stack
*             pointer is decremented.
* 
*             - value: 8-bit value to be pushed to the stack.
********************************************************************************/
int stack_push(const uint8_t value);

/********************************************************************************
* stack_pop: Returned value popped  from the stack and increments the stack 
*            pointer if it isn't already pointing to the top of the stack. 
*            If the stack is empty, the value 0x00 is returned.
********************************************************************************/
uint8_t stack_pop(void);

/********************************************************************************
* stack_pointer: Returns the address of the stack pointer.
********************************************************************************/
uint16_t stack_pointer(void);

/********************************************************************************
* stack_last_added_element: Returns the element last added to the stack. 
********************************************************************************/
uint8_t stack_last_added_element(void);

#endif /* STACK_H_ */