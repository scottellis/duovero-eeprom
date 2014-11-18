# Makefile for Duovero eeprom app  

CC = cc
CFLAGS = -Wall

OBJS = main.o \
       duovero_eeprom.o

TARGET = eeprom 

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<


.PHONY: clean

clean:
	rm -f $(OBJS) $(TARGET)


