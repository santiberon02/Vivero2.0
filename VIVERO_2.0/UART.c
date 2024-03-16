#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>		//libreria para la funcion dtostrf.
#include <string.h>		//libreria para generar los strings y concatenarlos.
#include <avr/eeprom.h>
#include "UART.h"

void limpiaruart();
void presentacion();
void Menu0();
//void Msj_uart(const char m);
void Msj_uart(char *m);
void ENVIO_UDR0(int x);

int lengthStrUART = 0;
char mostrarUART[50];
char valoruart[30];
float VTA;

////////////////////////////limpiar uart//////////////////////////////////////
void limpiaruart(){
	ENVIO_UDR0(12);						//limpio uart

}
///////////////////////////// Saludo /////////////////////////////////////////
void presentacion(){
	Msj_uart("Bienvenidos, VIVERO 2.0");
	ENVIO_UDR0(13);
}
///////////////////////////////////////
void Menu0(){
	Msj_uart("Accion a realizar");
	ENVIO_UDR0(13);									//salto renglon
	Msj_uart("1 - Carga parametros");
	ENVIO_UDR0(13);									//salto renglon
	Msj_uart("2 - Datos actuales");
}

////////////////////////////////////////////////////////////////////////
// void Msj_uart(const char m){
// 	char c;
// 	const char *ptr=m;
// 	
// 	while ((c = pgm_read_byte(*ptr++)) != '\0') {
// 		printf("%c", c);
// 
// }

void Msj_uart(char *m){
	 	lengthStrUART=  strlen(m);			//calculamos el tamaño que tiene este vector para recorrerlo luego
	 	for(int i=0; i<lengthStrUART; i++)
	 	{
	 		UDR0=m[i];
	 		while(!(UCSR0A&(1<<UDRE0)));
		}
}

void ENVIO_UDR0(int x){
	UDR0=x;
	while(!(UCSR0A&(1<<UDRE0)));
}

