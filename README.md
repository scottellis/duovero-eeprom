## duovero-eeprom

Read and write to the Gumstix Duovero EEPROM attached to I2C3
at address 0x51 on the Parlor expansion board.

Only the first 256 bytes are accessible with this app, accessed
as 128 16-bit words.

Works at either 100 kHz or 400 kHz.

### Usage

    root@duovero:~/duovero-eeprom # make
    cc -Wall -c main.c
    cc -Wall -c duovero_eeprom.c
    cc -Wall main.o  duovero_eeprom.o  -o eeprom


    root@duovero:~/duovero-eeprom # ./eeprom
    
    Usage: ./eeprom [-h] <address> [<value>]
      address     Address of 16-bit word, range 0-127
      value       Value to write, range 0-65535
      -h          Show this help

    root@duovero:~/duovero-eeprom # ./eeprom 0
    eeprom[0]: 123

    root@duovero:~/duovero-eeprom # ./eeprom 1
    eeprom[1]: 4457

    root@duovero:~/duovero-eeprom # ./eeprom 0 2222

    root@duovero:~/duovero-eeprom # ./eeprom 0
    eeprom[0]: 2222

