/********************************************************************************
* data_memory.h: Contains function declarations and macro definitions for
*                implementation of a 0.5 kB data memory.
********************************************************************************/
#ifndef DATA_MEMORY_H_
#define DATA_MEMORY_H_

/* Include directives: */
#include "cpu.h"

/* Macro definitions: */
#define DATA_MEMORY_DATA_WIDTH    8   /* 8 bits per address. */
#define DATA_MEMORY_ADDRESS_WIDTH 512 /* 512 addresses (0 - 511). */

/********************************************************************************
* data_memory_reset: Clears content of the data memory.
********************************************************************************/
void data_memory_reset(void);

/********************************************************************************
* data_memory_write: Writes value to specified address in data memory and
*                    returns 0 after write is complete. If an invalid address 
*                    is specified (address >= DATA_MEMORY_ADDRESS_WIDTH),
*                    no write is done and error code 1 is returned.
* 
*                    - address: Destination address in data memory.
*                    - value  : Value to write to the destination address.
********************************************************************************/
int data_memory_write(const uint16_t address, 
                      const uint8_t value);

/********************************************************************************
* data_memory_read: Returns value read from specified address. If an invalid
*                   address is specified, no read is done and 0 is returned.
*
*                   - address: Read address in data memory.
********************************************************************************/
uint8_t data_memory_read(const uint16_t address);

#endif /* DATA_MEMORY_H_ */