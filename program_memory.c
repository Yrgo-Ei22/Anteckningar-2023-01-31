/********************************************************************************
* program_memory.c: Contains function definitions and macro definitions for
*                   implementation of a 0.75 kB program memory, capable of 
*                   storing up to 256 24-bit instructions. Since C doesn't
*                   support unsigned 24-bit integers (without using structs or 
*                   unions), the program memory is set to 32 bits data width, 
*                   but only 24 bits are used.
********************************************************************************/
#include "program_memory.h"

/* Macro definitions: */
#define main           8  /* Start address for subroutine main. */
#define main_loop      9  /* Start address for loop in subroutine main. */
#define led_blink      11 /* Start address for subroutine led_blink. */
#define setup          16 /* Start address for subroutine setup. */
#define init_ports     19 /* Start address for subroutine init_ports. */
#define init_registers 22 /* Start address for subroutine init_registers. */
#define end            26 /* End address for current program. */

#define LED1 PORTB0 /* LED 1 connected to pin 8 (PORTB0). */
#define LED2 PORTB1 /* LED 2 connected to pin 9 (PORTB1). */
#define LED3 PORTB2 /* LED 3 connected to pin 10 (PORTB2). */

/* Static functions: */
static inline uint32_t assemble(const uint8_t op_code,
                                const uint8_t op1,
                                const uint8_t op2);

/* Static variables: */
static uint32_t data[PROGRAM_MEMORY_ADDRESS_WIDTH]; /* 0.75 kB program memory. */

/********************************************************************************
* program_memory_write: Writes machine code to the program memory. This function
*                       should be called once when the program starts.
********************************************************************************/
void program_memory_write(void)
{
   static bool program_memory_initialized = false;
   if (program_memory_initialized) return;

   /********************************************************************************
   * RESET_vect: Reset vector and start address for the program. A jump is made
   *             to the main subroutine in order to start the program.
   ********************************************************************************/
   data[0]  = assemble(JMP, main, 0x00); 
   data[1]  = assemble(NOP, 0x00, 0x00); 
   data[2]  = assemble(NOP, 0x00, 0x00); 
   data[3]  = assemble(NOP, 0x00, 0x00); 
   data[4]  = assemble(NOP, 0x00, 0x00); 
   data[5]  = assemble(NOP, 0x00, 0x00);
   data[6]  = assemble(NOP, 0x00, 0x00); 
   data[7]  = assemble(NOP, 0x00, 0x00); 

   /********************************************************************************
   * main: Initiates the system at start. The program is kept running as long
   *       as voltage is supplied. The leds connected to PORTB0 - PORTB2 are
   *       blinkning continuously. Values for enabling each LED is stored in
   *       CPU registers R16 - R18 for direct write to data register PORTB.
   ********************************************************************************/
   data[8] = assemble(CALL, setup, 0x00);

   /********************************************************************************
   * main_loop: Blinks the leds in a loop continuously.
   ********************************************************************************/
   data[9]  = assemble(CALL, led_blink, 0x00);
   data[10] = assemble(JMP, main_loop, 0x00);

   /********************************************************************************
   * led_blink: Blinks the leds in a loop continuously.
   ********************************************************************************/
   data[11] = assemble(OUT, PORTB, R16);
   data[12] = assemble(OUT, PORTB, R17);
   data[13] = assemble(OUT, PORTB, R18);
   data[14] = assemble(OUT, PORTB, R19);
   data[15] = assemble(RET, 0x00, 0x00);

   /********************************************************************************
   * setup: Initiates the system in terms of I/O ports and CPU registers.
   ********************************************************************************/
   data[16] = assemble(CALL, init_ports, 0x00);
   data[17] = assemble(CALL, init_registers, 0x00);
   data[18] = assemble(RET, 0x00, 0x00);

   /********************************************************************************
   * init_ports: Sets leds to outputs by setting corresponding bits in data
   *             direction register DDRB.
   ********************************************************************************/
   data[19] = assemble(LDI, R16, (1 << LED1) | (1 << LED2) | (1 << LED3));
   data[20] = assemble(OUT, DDRB, R16);
   data[21] = assemble(RET, 0x00, 0x00);

   /********************************************************************************
   * init_registers: Storing values for enabling each led LED1 - LED3 in CPU
   *                 registers R16 - R18 for direct write to data register PORTB.
   ********************************************************************************/
   data[22] = assemble(LDI, R16, (1 << LED1));
   data[23] = assemble(LDI, R17, (1 << LED2));
   data[24] = assemble(LDI, R18, (1 << LED3));
   data[25] = assemble(RET, 0x00, 0x00);

   program_memory_initialized = true;
   return;
}

/********************************************************************************
* program_memory_read: Returns the instruction at specified address. If an
*                      invalid address is specified (should be impossible as
*                      long as the program memory address width isn't increased)
*                      no operation (0x00) is returned.
*
*                      - address: Address to instruction in program memory.
********************************************************************************/
uint32_t program_memory_read(const uint8_t address)
{
   if (address < PROGRAM_MEMORY_ADDRESS_WIDTH)
   {
      return data[address];
   }
   else
   {
      return 0x00;
   }
}

/********************************************************************************
* program_memory_subroutine_name: Returns the name of the subroutine at
*                                 specified address.
*
*                                 - address: Address within the subroutine.
********************************************************************************/
const char* program_memory_subroutine_name(const uint8_t address)
{
   if (address >= RESET_vect && address < main)                return "RESET_vect";
   else if (address >= main && address < led_blink)            return "main";
   else if (address >= led_blink && address < setup)           return "led_blink";
   else if (address >= setup && address < init_ports)          return "setup";
   else if (address >= init_ports && address < init_registers) return "init_ports";
   else if (address >= init_registers && address < end)        return "init_registers";
   else                                                        return "Unknown";
}

/********************************************************************************
* assemble: Returns instruction assembled to machine code.
********************************************************************************/
static inline uint32_t assemble(const uint8_t op_code,
                                const uint8_t op1,
                                const uint8_t op2)
{
   const uint32_t instruction = (op_code << 16) | (op1 << 8) | op2;
   return instruction;
}