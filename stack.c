/********************************************************************************
* stack.c: Contains function definitions for implementation of a 1 kB stack.
********************************************************************************/
#include "stack.h"

/* Static variables: */
static uint8_t stack[STACK_ADDRESS_WIDTH]; /* 1 kB stack. */
static uint16_t sp;                        /* Stack pointer, points to last added element. */
static bool stack_empty;                   /* Indicates if the stack is empty. */

/********************************************************************************
* stack_reset: Clears content of the stack and sets the stack pointer to the
*              top of the stack.
********************************************************************************/
void stack_reset(void)
{
   for (uint16_t i = 0; i < STACK_ADDRESS_WIDTH; ++i)
   {
      stack[i] = 0x00;
   }

   sp = STACK_ADDRESS_WIDTH - 1;
   stack_empty = true;
   return;
}

/********************************************************************************
* stack_push: Pushes value to the bottom of the stack. If the stack is full,
*             the value isn't pushed and error code 1 is returned. Otherwise
*             return code 0 is returned after successful push and the stack
*             pointer is decremented.
*
*             - value: 8-bit value to be pushed to the stack.
********************************************************************************/
int stack_push(const uint8_t value)
{
   if (sp == 0) return 1;

   if (stack_empty)
   {
      stack[sp] = value;
      stack_empty = false;
   }
   else
   {
      stack[--sp] = value;
   }

   return 0;
}

/********************************************************************************
* stack_pop: Returnes value popped  from the stack and increments the stack
*            pointer if it isn't already pointing to the top of the stack.
*            If the stack is empty, the value 0x00 is returned.
********************************************************************************/
uint8_t stack_pop(void)
{
   const uint8_t value = stack[sp];
   stack[sp] = 0x00;

   if (sp < STACK_ADDRESS_WIDTH - 1)
   {
      sp++;
   }
   else
   {
      stack_empty = true;
   }

   return value;
}

/********************************************************************************
* stack_pointer: Returns the address of the stack pointer.
********************************************************************************/
uint16_t stack_pointer(void)
{
   return sp;
}

/********************************************************************************
* stack_last_added_element: Returns the element last added to the stack.
********************************************************************************/
uint8_t stack_last_added_element(void)
{
  return stack[sp];
}