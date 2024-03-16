
#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>		//libreria para la funcion dtostrf.
#include <string.h>		//libreria para generar los strings y concatenarlos.
#include <avr/eeprom.h>
#ifndef UART_H_
#define UART_H_


void limpiaruart();
void presentacion();
void Menu0();
void Msj_uart(char *m);
//void Msj_uart(const char m);
void ENVIO_UDR0(int x);

#endif /* UART_H_ */

