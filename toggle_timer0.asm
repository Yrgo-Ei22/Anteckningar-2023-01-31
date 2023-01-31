;********************************************************************************
; toggle_timer0.asm: Ansluter lysdiod LED1 till pin 8 (PORTB0) samt tryckknapp
;                    BUTTON1 till pin 13 (PORTB5) och initierar timerkrets
;                    Timer 0 i Normal Mode s� att overflow-avbrott sker var 
;                    16.384:e ms (vid uppr�kning till 256) i aktiverat tillst�nd.
;                    Timer 0 togglas via nedtryckning av BUTTON1. N�r Timer 0
;                    �r aktiverad togglar denna LED1 var 100:e ms, allts� var 
;                    sj�tte overflow-avbrott. N�r Timer 0 �r inaktiverad h�lls
;                    LED1 sl�ckt.
;
;                    - Notering: R16 lagrar v�rdet (1 << LED1) = 0000 0001,
;                                skriv inte �ver detta v�rde!                   
;********************************************************************************

;********************************************************************************
; Makrodefinitioner:
;********************************************************************************
.EQU LED1    = PORTB0       ; Lysdiod 1 ansluten till pin 8 (PORTB0).
.EQU BUTTON1 = PORTB5       ; Tryckknapp 1 ansluten till pin 13 (PORTB5).

.EQU RESET_vect      = 0x00 ; Reset-vektor, utg�r programmets startpunkt.
.EQU PCINT0_vect     = 0x06 ; Avbrottsvektor f�r PCI-avbrott p� I/O-port B.
.EQU TIMER0_OVF_vect = 0x20 ; Avbrottsvektor f�r Timer 0 i Normal Mode.

.EQU TIMER0_MAX_COUNT = 6   ; Uppr�kning f�r 100 ms f�rdr�jning.

;********************************************************************************
; .DSEG: Dataminnet, h�r lagras statiska variabler.
;********************************************************************************
.DSEG
counter0: .byte 1 ; static uint8_t counter0 = 0;

;********************************************************************************
; .CSEG: Programminnet, h�r lagras maskinkoden.
;********************************************************************************
.CSEG

;********************************************************************************
; RESET_vect: Programmets startpunkt. Programhopp sker till subrutinen main
;             f�r att starta programmet. 
;********************************************************************************
.ORG RESET_vect
   RJMP main

;********************************************************************************
; PCINT0_vect: Avbrottsvektor f�r PCI-avbrott p� I/O-port B, som programmet 
;              hoppar till vid nedtryckning eller uppsl�ppning av BUTTON1.
;              Hopp sker till motsvarande avbrottsrutin ISR_PCINT0 f�r att
;              hantera avbrottet.
;********************************************************************************
.ORG PCINT0_vect
   RJMP ISR_PCINT0

;********************************************************************************
; TIMER0_OVF_vect: Avbrottsvektor f�r Timer 0 i Normal Mode, som programmet
;                  hoppar till var 16.384:e millisekund n�r Timer 0 �r aktiverad. 
;                  Programhopp sker till motsvarande avbrottsrutin 
;                  ISR_TIMER0_OVF f�r att hantera avbrottet.
;********************************************************************************
.ORG TIMER0_OVF_vect
   RJMP ISR_TIMER0_OVF

;********************************************************************************
; ISR_PCINT0: Avbrottsrutin f�r PCI-avbrott p� I/O-port B, som programmet 
;             hoppar till vid nedtryckning eller uppsl�ppning av BUTTON1.
;             Vid nedtryckning av BUTTON1 togglas Timer 0.
;********************************************************************************
ISR_PCINT0:
   IN R24, PINB
   ANDI R24, (1 << BUTTON1)
   BREQ ISR_PCINT0_end
   RCALL timer0_toggle
ISR_PCINT0_end:
   RETI

;********************************************************************************
; ISR_TIMER0_OVF: Avbrottsrutin f�r Timer 0 i Normal Mode, som programmet hoppar 
;                 till var 16.384:e millisekund n�r Timer 0 �r aktiverad. 
;                 Var sj�tte avbrott (ca var 100:e ms) togglas LED1.
;********************************************************************************
ISR_TIMER0_OVF:
   LDS R24, counter0
   INC R24
   CPI R24, TIMER0_MAX_COUNT
   BRLO ISR_TIMER0_OVF_end
   OUT PINB, R16
   CLR R24
ISR_TIMER0_OVF_end:
   STS counter0, R24
   RETI

;********************************************************************************
; main: Initierar systemet vid start. Programmet h�lls sedan ig�ng s� l�nge
;       matningssp�nning tillf�rs.
;********************************************************************************
main:

;********************************************************************************
; init_ports: S�tter lysdiodens pin till utport och aktiverar den interna
;             pullup-resistorn p� tryckknappens pin.
;********************************************************************************
init_ports:
   LDI R16, (1 << LED1)
   OUT DDRB, R16
   LDI R17, (1 << BUTTON1)
   OUT PORTB, R17

;********************************************************************************
; init_interrupts: Aktiverar PCI-avbrott p� tryckknappens pin och konfigurerar
;                  Timer 0 f�r overflow-avbrott var 16.384:e ms i Normal Mode.
;********************************************************************************
init_interrupts:
   STS PCICR, R16
   STS PCMSK0, R17
   LDI R18, (1 << CS02) | (1 << CS00)
   OUT TCCR0B, R18
   SEI

;********************************************************************************
; main_loop: H�ller ig�ng programmet s� l�nge matningssp�nning tillf�rs.
;********************************************************************************
main_loop:
   RJMP main_loop

;********************************************************************************
; timer0_toggle: Togglar aktivering av Timer 0. Ifall Timer 0 inaktiveras
;                sl�cks LED1 omedelbart.
;********************************************************************************
timer0_toggle:
   LDS R24, TIMSK0
   ANDI R24, (1 << TOIE0)
   BREQ timer0_toggle_enable
timer0_toggle_disable:
   IN R24, PORTB
   ANDI R24, ~(1 << LED1)
   OUT PORTB, R24
   CLR R24
   RJMP timer0_toggle_end
timer0_toggle_enable:
   LDI R24, (1 << TOIE0)
timer0_toggle_end:
   STS TIMSK0, R24
   RET
   
