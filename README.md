# Anteckningar 2022-01-31
Implementering av 8-bitars ALU för CPU-emulator samt övningsuppgift med timer- och PCI-avbrott i assembler.
ALU:ns syfte och arbetssätt samt implementering för aritmetiska operationer addition och subtraktion samt 
logiska operationer OR, AND och XOR demonstreras. Statusbitar SNZVC is CPU:ns statusregister uppdateras 
utefter beräknat resultat.

Under nästa lektion ska diverse aritmetiska samt logiska instruktionser såsom ORI, ANDI, ADD och INC implementeras, tillsammans med hoppinstruktioner BREQ, BRNE, BRGE, BRGT, BRLE samt BRLT, som alla använder sig av en eller 
flera statusbitar satta av ALU:n.

Samtliga .c- och .h-filer utgörs av CPU-emulatorn.

Filen "alu.png" utgör en bild som demonstrerar ALU:ns arbetssätt visuellt samt via text.

Filen "alu_emulator.zip" utgör en ALU-emulator, som kan användas för att testa ALU:ns funktion från en terminal. Operation samt operander kan matas in från terminalen, följt av att resultatet skrivs ut både decimalt och binärt, tillsammans med statusbitar SNZVC. Fem exempelfall skrivas ut vid start, som demonstrerar när de olika statusbitarna ettställs.

Filen "toggle_timer0.asm" utgör en övningsuppgift innefattande timer- samt PCI-avbrott i assembler.
