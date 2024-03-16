/*
 * VIVERO_2.0.c
 *
 * Created: 12/12/2023 17:15:38
 * Author : Ale_Martinez
 */ 

#define  F_CPU 16000000
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>

#include "LCD.h"
#include "I2C.h"
#include "frases.h"
#include "Fraseslcd.h"
#include "UART.h"
#include "sensores.h"
void Menu1(unsigned char x);
void Menu2(unsigned char x);
void MenuA(unsigned char a);
void MenuI(unsigned char i);
void lcdT();
void lcdt();
void lcdk();
void lcdH();
void lcdh();
void lcdS();
void lcds();
void lcdV();
void lcdv();
void lcdE();
void lcde();
void lcdVIV();
void init_Int0();
void cargardato(int limh);
void Msj_numero(int x);
void datosmenui();
void paraelse();
void EEPROM_write(unsigned int uiAddress, unsigned char ucData);
unsigned char EEPROM_read(unsigned int uiAddress);


int ContadorMuestroLCD;
unsigned char Datouart, valoreeprom;
int llamado, variable, opcancelada=0;
int VTA, HTA,GPTA, HAI, HHAI, HSI, HHSI, VMAXV, VMINV, IMAX, IMIN;
int contador=0, temp, c,d,u,num, t, var, cont=0, con;

int main(void)
{
		// Configuracion UART
		UCSR0A=0;
		UCSR0B=(1<<TXEN0)|(1<<RXEN0)|(1<<RXCIE0);
		UCSR0C=(1<<UCSZ01)|(1<<UCSZ00);
		UBRR0=103; //9600,8,N,1
		I2C_Init();
		I2C_startCond();
		presentacion();
		Menu0();
		Seleccion_I2C(0x70);
		Inicio_LCD();
		Direccion(0x02); //direccion para empezar a escribir en lcd
		LCD_Mensaje(Presentlcd);
	sei();
	

    while (1) 
    {
		//FDC1004();
		BMP180();
	if (llamado==1){
			switch(Datouart){
				case '1':
					llamado=0; //para poder seguir ingresando datos
					Menu1(Datouart);
					while (llamado==0); //en este while 
					Menu1(Datouart);
				break;
				
				case '2':
					limpiaruart();
					Envio_Com(0x01);
					Direccion(0x02);
					LCD_Mensaje(Presentlcd);
					
					llamado=0;
					Msj_uart(frmen2);
					ENVIO_UDR0(13); //salto renglon
					datosmenui();
					llamado=0;
					Menu2(Datouart);
					while (llamado==0);
					Menu2(Datouart);
					break;
			}
		}
	}
}


ISR(USART_RX_vect){
	_delay_ms(10);
		Datouart=UDR0;
	while(!(UCSR0A&(1<<UDRE0))); //espero el recibimiento correcto del dato
		llamado=1;
	}

void Menu1(unsigned char x){
	limpiaruart();
	Msj_uart("Carga valores de sensores");
		ENVIO_UDR0(13);									//salto renglon
	Msj_uart("A - Todos los valores");
		ENVIO_UDR0(13);									//salto renglon
	Msj_uart("I - Valor especifico");
		ENVIO_UDR0(13);									//salto renglon
	Msj_uart(frmin);
	
if (llamado==1) {
	switch(x){
		case 'a':
		case 'A':
		limpiaruart();
		llamado=0;
		MenuA(Datouart);
		while (llamado==0);
		MenuA(Datouart);
		break;
		
		case 'i':
		case 'I':
		limpiaruart();		//limpio la uart
		llamado=0;
		Msj_uart("Seleccione parametro a cargar");
		ENVIO_UDR0(13); //salto renglon
		datosmenui();  //cargo los datos para saber q tecla se apreta para cada variable
		MenuI(Datouart);
		while(llamado==0);
		MenuI(Datouart);
		break;
		
		case 'q': //vuelvo al inicio
		limpiaruart();
		presentacion();
		Menu0();
		llamado=0;
		break;
		
		default:{
			llamado=0;
			Menu1(Datouart);
			while (llamado==0);
			Menu1(Datouart);
		}
	}
}
}

void Menu2(unsigned char x){
	
	if (llamado==1){
		switch(x){
			case 'T':		//temperatura interna 
				cont=0;
				limpiaruart();
				Msj_uart(frT2); //imprimo frase 
				Msj_numero(VTA); //envio a uart el valor cargado en vta
				Msj_uart(frgrado);
				ENVIO_UDR0(13); //renglon nuevo
				Msj_uart(frmedac); 
				Msj_uart(tempint);
				Msj_uart(frgrado);
				lcdT();
				ENVIO_UDR0(13);
				Msj_uart(frmant);
				while (cont!=1){//hasta q no se presione z no sale
					_delay_ms(10);
					if (Datouart==122){
						cont=1;
					}
				}
				limpiaruart();
				llamado=0;
				Msj_uart(frmen2);
				ENVIO_UDR0(13); //salto renglon
				datosmenui();
				llamado=0;
				Menu2(Datouart);
				while (llamado==0);
				Menu2(Datouart);
			break;
			
			case 't':		//histeresis de la temperatura interna
				cont=0;
				limpiaruart();
				Msj_uart(frt2);
				Msj_numero(HTA);
				Msj_uart(frgrado);
				ENVIO_UDR0(13);
				Msj_uart(frmant);
				while (cont!=1){
					_delay_ms(10);
					if (Datouart==122){
						cont=1;
					}
				}
				limpiaruart();
				llamado=0;
				Msj_uart(frmen2);
				ENVIO_UDR0(13); //salto renglon
				datosmenui();
				llamado=0;
				Menu2(Datouart);
				while (llamado==0);
				Menu2(Datouart);
				
			break;
			
			case 'k':		//ganancia de la temperatura interna
				cont=0;
				limpiaruart();
				Msj_uart(frk2);
				Msj_numero(GPTA);
				Msj_uart(frgrado);
				ENVIO_UDR0(13);
				Msj_uart(frmant);
				while (cont!=1){
					_delay_ms(10);
					if (Datouart==122){
						cont=1;
					}
				}
				limpiaruart();
				llamado=0;
				Msj_uart(frmen2);
				ENVIO_UDR0(13); //salto renglon
				datosmenui();
				llamado=0;
				Menu2(Datouart);
				while (llamado==0);
				Menu2(Datouart);
				
			break;
			
			case 'H':		//humedad ambiental interna
				cont=0;
				limpiaruart();
				Msj_uart(frH2);
				Msj_numero(HAI);
				Msj_uart(frporcen);
				ENVIO_UDR0(13);
				Msj_uart(frmedac);
				Msj_uart(humint);
				Msj_uart(frporcen);
				lcdH();
				ENVIO_UDR0(13);
				Msj_uart(frmant);
				while (cont!=1){
					_delay_ms(10);
					if (Datouart==122){
						cont=1;
					}
				}
				Envio_Com(0x01);
				limpiaruart();
				llamado=0;
				Msj_uart(frmen2);
				ENVIO_UDR0(13); //salto renglon
				datosmenui();
				Direccion(0x02);
				LCD_Mensaje(Presentlcd);
				llamado=0;
				Menu2(Datouart);
				while (llamado==0);
				Menu2(Datouart);
			break;
			
			case 'h':
				cont=0;
				limpiaruart();
				Msj_uart(frh2);
				Msj_numero(HHAI);
				Msj_uart(frporcen);
				ENVIO_UDR0(13);
				Msj_uart(frmedac);
				//algun void para que cada 1 seg se actualize estando en este menu
				Msj_uart(frporcen);
				lcdh();
				ENVIO_UDR0(13);
				Msj_uart(frmant);
				while (cont!=1){
					_delay_ms(10);
					if (Datouart==122){
						cont=1;
					}
				}
				Envio_Com(0x01);
				limpiaruart();
				llamado=0;
				Msj_uart(frmen2);
				ENVIO_UDR0(13); //salto renglon
				datosmenui();
				Direccion(0x02);
				LCD_Mensaje(Presentlcd);
				llamado=0;
				Menu2(Datouart);
				while (llamado==0);
				Menu2(Datouart);
			break;
			
			case 'S':
				cont=0;
				limpiaruart();
				Msj_uart(frS2);
				Msj_numero(HSI);
				Msj_uart(frporcen);
				ENVIO_UDR0(13);
				Msj_uart(frmedac);
				//algun void para que cada 1 seg se actualize estando en este menu
				Msj_uart(frporcen);
				lcdS();
				ENVIO_UDR0(13);
				Msj_uart(frmant);
				while (cont!=1){
					_delay_ms(10);
					if (Datouart==122){
						cont=1;
					}
				}
				Envio_Com(0x01);
				limpiaruart();
				llamado=0;
				Msj_uart(frmen2);
				ENVIO_UDR0(13); //salto renglon
				datosmenui();
				Direccion(0x02);
				LCD_Mensaje(Presentlcd);
				llamado=0;
				Menu2(Datouart);
				while (llamado==0);
				Menu2(Datouart);
			break;
			
			case 's':
				cont=0;
				limpiaruart();
				Msj_uart(frs2);
				Msj_numero(HHSI);
				Msj_uart(frporcen);
				ENVIO_UDR0(13);
				Msj_uart(frmedac);
				//algun void para que cada 1 seg se actualize estando en este menu
				Msj_uart(frporcen);
				lcds();
				ENVIO_UDR0(13);
				Msj_uart(frmant);
				while (cont!=1){
					_delay_ms(10);
					if (Datouart==122){
						cont=1;
					}
				}
				Envio_Com(0x01);
				limpiaruart();
				llamado=0;
				Msj_uart(frmen2);
				ENVIO_UDR0(13); //salto renglon
				datosmenui();
				Direccion(0x02);
				LCD_Mensaje(Presentlcd);
				llamado=0;
				Menu2(Datouart);
				while (llamado==0);
				Menu2(Datouart);
			break;
			
			case 'V':
				cont=0;
				limpiaruart();
				Msj_uart(frV2);
				Msj_numero(VMAXV);
				Msj_uart(frm7s);
				ENVIO_UDR0(13);
				Msj_uart(frmedac);
				//algun void para que cada 1 seg se actualize estando en este menu
				Msj_uart(frm7s);
				lcdV();
				ENVIO_UDR0(13);
				Msj_uart(frmant);
				while (cont!=1){
					_delay_ms(10);
					if (Datouart==122){
						cont=1;
					}
				}
				Envio_Com(0x01);
				limpiaruart();
				llamado=0;
				Msj_uart(frmen2);
				ENVIO_UDR0(13); //salto renglon
				datosmenui();
				Direccion(0x02);
				LCD_Mensaje(Presentlcd);
				llamado=0;
				Menu2(Datouart);
				while (llamado==0);
				Menu2(Datouart);
			break;
			
			case 'v':
				cont=0;
				limpiaruart();
				Msj_uart(frv2);
				Msj_numero(VMINV);
				Msj_uart(frm7s);
				ENVIO_UDR0(13);
				Msj_uart(frmedac);
				//algun void para que cada 1 seg se actualize estando en este menu
				Msj_uart(frm7s);
				lcdv();
				ENVIO_UDR0(13);
				Msj_uart(frmant);
				while (cont!=1){
					_delay_ms(10);
					if (Datouart==122){
						cont=1;
					}
				}
				Envio_Com(0x01);
				limpiaruart();
				llamado=0;
				Msj_uart(frmen2);
				ENVIO_UDR0(13); //salto renglon
				datosmenui();
				Direccion(0x02);
				LCD_Mensaje(Presentlcd);
				llamado=0;
				Menu2(Datouart);
				while (llamado==0);
				Menu2(Datouart);
			break;
			
			case 'E':
				cont=0;
				limpiaruart();
				Msj_uart(frE2);
				Msj_numero(IMAX);
				Msj_uart(frlux);
				ENVIO_UDR0(13);
				Msj_uart(frmedac);
				//algun void para que cada 1 seg se actualize estando en este menu
				Msj_uart(frlux);
				lcdE();
				ENVIO_UDR0(13);
				Msj_uart(frmant);
				while (cont!=1){
					_delay_ms(10);
					if (Datouart==122){
						cont=1;
					}
				}
				Envio_Com(0x01);
				limpiaruart();
				llamado=0;
				Msj_uart(frmen2);
				ENVIO_UDR0(13); //salto renglon
				datosmenui();
				Direccion(0x02);
				LCD_Mensaje(Presentlcd);
				llamado=0;
				Menu2(Datouart);
				while (llamado==0);
				Menu2(Datouart);
			break;
			
			case 'e':
				cont=0;
				limpiaruart();
				Msj_uart(fre2);
				Msj_numero(IMIN);
				Msj_uart(frlux);
				ENVIO_UDR0(13);
				Msj_uart(frmedac);
				//algun void para que cada 1 seg se actualize estando en este menu
				Msj_uart(frlux);
				lcde();
				ENVIO_UDR0(13);
				Msj_uart(frmant);
				while (cont!=1){
					_delay_ms(10);
					if (Datouart==122){
						cont=1;
					}
				}
				Envio_Com(0x01);
				limpiaruart();
				llamado=0;
				Msj_uart(frmen2);
				ENVIO_UDR0(13); //salto renglon
				datosmenui();
				Direccion(0x02);
				LCD_Mensaje(Presentlcd);
				llamado=0;
				Menu2(Datouart);
				while (llamado==0);
				Menu2(Datouart);
			break;
			
			case 'z':
				limpiaruart();
				Envio_Com(0x01);
				Direccion(0x02);
				LCD_Mensaje(Presentlcd);
				presentacion();
				Menu0();
				llamado=0;
			break;
			
			default:
				limpiaruart();
				Msj_uart(frin);
				ENVIO_UDR0(13);
				Envio_Com(0x01);
				Direccion(0x02);
				LCD_Mensaje(Presentlcd);
				llamado=0;
				Msj_uart(frmen2);
				ENVIO_UDR0(13); //salto renglon
				datosmenui();
				Menu2(Datouart);
				while(llamado==0);
				Menu2(Datouart);
		}
	}
}

void MenuA(unsigned char a){
	limpiaruart();
	Msj_uart("Carga de todos los valores");
	ENVIO_UDR0(13);
			Msj_uart(frT);
			ENVIO_UDR0(13);
			if(Datouart!=122){
			cargardato(100); //cargo los datos con un valor maximo de 100
			con=0;
			while(con==0){
			if(opcancelada==0){
				VTA=variable;//mando a vta el valor cargado
				valoreeprom=EEPROM_read(0); //leo la eeprom en la direccion de memoria de la temperatura
				if (valoreeprom!=VTA){ //si el valor es distinto lo guardo
					EEPROM_write(0, VTA);
					ENVIO_UDR0(13);
					Msj_uart(fraceep);
				}
				con=1;
				ENVIO_UDR0(13);
				Msj_uart(frT2);
				Msj_numero(VTA);
				Msj_uart(" ºC");
				ENVIO_UDR0(13);
			}
			else{
				Msj_uart(frrein);
				ENVIO_UDR0(13);
				cargardato(100);
			}
			}
			con=0;
			}
	ENVIO_UDR0(13);
	Msj_uart(frt);
				ENVIO_UDR0(13);
				cargardato(100);
				con=0;
				while(con==0){
				if(opcancelada==0){
					HTA=variable;
					valoreeprom=EEPROM_read(2);
					if (valoreeprom!=HTA){
						EEPROM_write(2, HTA);
						ENVIO_UDR0(13);
						Msj_uart(fraceep);
					}
					con=1;
					ENVIO_UDR0(13);
					Msj_uart(frt2);
					Msj_numero(HTA);
					Msj_uart(" ºC");
					ENVIO_UDR0(13);
				}
				else{
					Msj_uart(frrein);
					ENVIO_UDR0(13);
					cargardato(100);
				}
				}
				con=0;
	ENVIO_UDR0(13);
	Msj_uart(frk);
				ENVIO_UDR0(13);
				cargardato(100);
				con=0;
				while(con==0){
				if(opcancelada==0){
					GPTA=variable;
					valoreeprom=EEPROM_read(4);
					if (valoreeprom!=GPTA){
						EEPROM_write(4, GPTA);
						ENVIO_UDR0(13);
						Msj_uart(fraceep);
					}
					con=1;
					ENVIO_UDR0(13);
					Msj_uart(frk2);
					Msj_numero(GPTA);
					Msj_uart(" %");
					ENVIO_UDR0(13);
				}
				else{
					Msj_uart(frrein);
					ENVIO_UDR0(13);
					cargardato(100);
				}
				}
			con=0;
	ENVIO_UDR0(13);
	Msj_uart(frH);
				ENVIO_UDR0(13);
				cargardato(95);
				con=0;
				while(con==0){
				if(opcancelada==0){
					HAI=variable;
					valoreeprom=EEPROM_read(6);
					if (valoreeprom!=HAI){
						EEPROM_write(6, HAI);
						ENVIO_UDR0(13);
						Msj_uart(fraceep);
					}
					con=1;
					ENVIO_UDR0(13);
					Msj_uart(frH2);
					Msj_numero(HAI);
					Msj_uart(" %");
					ENVIO_UDR0(13);
				}
				else{
					Msj_uart(frrein);
					ENVIO_UDR0(13);
					cargardato(100);
				}
				}
				con=0;
	ENVIO_UDR0(13);
	Msj_uart(frh);
				ENVIO_UDR0(13);
				cargardato(95);
				con=0;
				while(con==0){
				if(opcancelada==0){
					HHAI=variable;
					valoreeprom=EEPROM_read(8);
					if (valoreeprom!=HHAI){
						EEPROM_write(8, HHAI);
						ENVIO_UDR0(13);
						Msj_uart(fraceep);
					}
					con=1;
					ENVIO_UDR0(13);
					Msj_uart(frh2);
					Msj_numero(HHAI);
					Msj_uart(" %");
					ENVIO_UDR0(13);
				}
				else{
					Msj_uart(frrein);
					ENVIO_UDR0(13);
					cargardato(100);
				}
				}
				con=0;
	ENVIO_UDR0(13);
	Msj_uart(frS);
				ENVIO_UDR0(13);
				cargardato(100);
				con=0;
				while(con==0){
				if(opcancelada==0){
					HSI=variable;
					valoreeprom=EEPROM_read(10);
					if (valoreeprom!=HSI){
						EEPROM_write(10, HSI);
						ENVIO_UDR0(13);
						Msj_uart(fraceep);
					}
					con=1;
					ENVIO_UDR0(13);
					Msj_uart(frS2);
					Msj_numero(HSI);
					Msj_uart(" %");
					ENVIO_UDR0(13);
				}
				else{
					Msj_uart(frrein);
					ENVIO_UDR0(13);
					cargardato(100);
				}
				}
				con=0;
	ENVIO_UDR0(13);
	Msj_uart(frs);
				ENVIO_UDR0(13);
				cargardato(100);
				con=0;
				while(con==0){
				if(opcancelada==0){
					HHSI=variable;
					valoreeprom=EEPROM_read(12);
					if (valoreeprom!=HHSI){
						EEPROM_write(12, HHSI);
						ENVIO_UDR0(13);
						Msj_uart(fraceep);
					}
					con=1;
					ENVIO_UDR0(13);
					Msj_uart(frs2);
					Msj_numero(HHSI);
					Msj_uart(" %");
					ENVIO_UDR0(13);
				}
				else{
					Msj_uart(frrein);
					ENVIO_UDR0(13);
					cargardato(100);
				}
				}
				con=0;
	ENVIO_UDR0(13);
	Msj_uart(frV);
				ENVIO_UDR0(13);
				cargardato(100);
				con=0;
				while(con==0){
				if(opcancelada==0){
					VMAXV=variable;
					valoreeprom=EEPROM_read(14);
					if (valoreeprom!=VMAXV){
						EEPROM_write(14, VMAXV);
						ENVIO_UDR0(13);
						Msj_uart(fraceep);
					}
					con=1;
					ENVIO_UDR0(13);
					Msj_uart(frV2);
					Msj_numero(VMAXV);
					Msj_uart(" km/h");
					ENVIO_UDR0(13);
				}
				else{
					Msj_uart(frrein);
					ENVIO_UDR0(13);
					cargardato(100);
				}
				}
				con=0;
	ENVIO_UDR0(13);
	Msj_uart(frv);
				ENVIO_UDR0(13);
				cargardato(100);
				con=0;
				while(con==0){
				if(opcancelada==0){
					VMINV=variable;
					valoreeprom=EEPROM_read(16);
					if (valoreeprom!=VMINV){
						EEPROM_write(16, VMINV);
						ENVIO_UDR0(13);
						Msj_uart(fraceep);
					}
					con=1;
					ENVIO_UDR0(13);
					Msj_uart(frv2);
					Msj_numero(VMINV);
					Msj_uart(" km/h");
					ENVIO_UDR0(13);
				}
				else{
					Msj_uart(frrein);
					ENVIO_UDR0(13);
					cargardato(100);
				}
				}
				con=0;
	ENVIO_UDR0(13);
	Msj_uart(frE);
				ENVIO_UDR0(13);
				cargardato(100);
				con=0;
				while(con==0){
				if(opcancelada==0){
					IMAX=variable;
					valoreeprom=EEPROM_read(18);
					if (valoreeprom!=IMAX){
						EEPROM_write(18, IMAX);
						ENVIO_UDR0(13);
						Msj_uart(fraceep);
					}
					con=1;
					ENVIO_UDR0(13);
					Msj_uart(frE2);
					Msj_numero(IMAX);
					Msj_uart(" LUX");
					ENVIO_UDR0(13);
				}
				else{
					Msj_uart(frrein);
					ENVIO_UDR0(13);
					cargardato(100);
				}
				}
				con=0;
	ENVIO_UDR0(13);
	Msj_uart(fre);
				ENVIO_UDR0(13);
				cargardato(100);
				con=0;
				while(con==0){
				if(opcancelada==0){
					IMIN=variable;
					valoreeprom=EEPROM_read(20);
					if (valoreeprom!=IMIN){
						EEPROM_write(20, IMIN);
						ENVIO_UDR0(13);
						Msj_uart(fraceep);
					}
					con=1;
					ENVIO_UDR0(13);
					Msj_uart(fre2);
					Msj_numero(IMIN);
					Msj_uart(" LUX");
					ENVIO_UDR0(13);
				}
				else{
					Msj_uart(frrein);
					ENVIO_UDR0(13);
					cargardato(100);
				}
				}
				con=0;
	ENVIO_UDR0(13);
				cont=0;
	Msj_uart(frmant);
				while (cont!=1){
					_delay_ms(10);
					if (Datouart==122){
						cont=1;
					}
				}
			Menu1(Datouart);
}

void MenuI(unsigned char i){
	if (llamado==1){
		switch(i){
			case 'T': //temp interna deseada
			limpiaruart();
			Msj_uart(frT1);
			ENVIO_UDR0(13);
			cargardato(100);
 			if(opcancelada==0){
			VTA=variable;
			valoreeprom=EEPROM_read(0);
			if (valoreeprom!=VTA){
				EEPROM_write(0, VTA);
				ENVIO_UDR0(13);
				Msj_uart(fraceep);
			}
			cont=0;
			ENVIO_UDR0(13);
			Msj_uart(frT2);
			Msj_numero(VTA);
			Msj_uart(" ºC");
			ENVIO_UDR0(13);
			Msj_uart(frmant);
				while (cont!=1){
					_delay_ms(10);
					if (Datouart==122){
						cont=1;
					}
				}
				limpiaruart();
				llamado=0;
				Msj_uart(frcpar);
				ENVIO_UDR0(13); //salto renglon
				datosmenui();
				MenuI(Datouart);
				while(llamado==0);
				MenuI(Datouart);
			}
			else{
				paraelse();
			}
			break;
			case 't': //histeresis para el control de temperatura
			limpiaruart();
			Msj_uart(frt1);
			ENVIO_UDR0(13);
			cargardato(100);
			if(opcancelada==0){
				HTA=variable;
				valoreeprom=EEPROM_read(2);
				if (valoreeprom!=HTA){
					EEPROM_write(2, HTA);
					ENVIO_UDR0(13);
					Msj_uart(fraceep);
				}
				cont=0;
				ENVIO_UDR0(13);
				Msj_uart(frt2);
				Msj_numero(HTA);
				Msj_uart(" ºC");
				ENVIO_UDR0(13);
				Msj_uart(frmant);
				while (cont!=1){
					_delay_ms(10);
					if (Datouart==122){
						cont=1;
					}
				}
				limpiaruart();
				llamado=0;
				Msj_uart(frcpar);
				ENVIO_UDR0(13); //salto renglon
				datosmenui();
				MenuI(Datouart);
				while(llamado==0);
				MenuI(Datouart);
			}
			else{
				paraelse();
			}
			break;
			case 'k': //ganancia proporcional para control de temperatura
			limpiaruart();
			Msj_uart(frk1);
			ENVIO_UDR0(13);
			cargardato(100);
			if(opcancelada==0){
				GPTA=variable;
				valoreeprom=EEPROM_read(4);
				if (valoreeprom!=GPTA){
					EEPROM_write(4, GPTA);
					ENVIO_UDR0(13);
					Msj_uart(fraceep);
				}
				cont=0;
				ENVIO_UDR0(13);
				Msj_uart(frk2);
				Msj_numero(GPTA);
				Msj_uart(" %");
				ENVIO_UDR0(13);
				Msj_uart(frmant);
				
				while (cont==0){
					_delay_ms(10);
					if (Datouart==122){
						cont=1;
					}
				}
				limpiaruart();
				llamado=0;
				Msj_uart(frcpar);
				ENVIO_UDR0(13); //salto renglon
				datosmenui();
				MenuI(Datouart);
				while(llamado==0);
				MenuI(Datouart);
			}
			else{
				paraelse();
 			}
			break;
			case 'H': //humedad ambiental interna deseada
			limpiaruart();
			Msj_uart(frH1);
			ENVIO_UDR0(13);
			cargardato(95);
			if(opcancelada==0){
				HAI=variable;
				valoreeprom=EEPROM_read(6);
				if (valoreeprom!=HAI){
					EEPROM_write(6, HAI);
					ENVIO_UDR0(13);
					Msj_uart(fraceep);
				}
				cont=0;
				ENVIO_UDR0(13);
				Msj_uart(frH2);
				Msj_numero(HAI);
				Msj_uart(" %");
				ENVIO_UDR0(13);
				Msj_uart(frmant);
				
				while (cont==0){
					_delay_ms(10);
					if (Datouart==122){
						cont=1;
					}
				}
				limpiaruart();
				llamado=0;
				Msj_uart(frcpar);
				ENVIO_UDR0(13); //salto renglon
				datosmenui();
				MenuI(Datouart);
				while(llamado==0);
				MenuI(Datouart);
			}
			else{
				paraelse();
			}
			break;
			case 'h': //histeresis para el control de humedad interna
			limpiaruart();
			Msj_uart(frh1);
			ENVIO_UDR0(13);
			cargardato(95);
			if(opcancelada==0){
				HHAI=variable;
				valoreeprom=EEPROM_read(8);
				if (valoreeprom!=HHAI){
					EEPROM_write(8, HHAI);
					ENVIO_UDR0(13);
					Msj_uart(fraceep);
				}
				cont=0;
				ENVIO_UDR0(13);
				Msj_uart(frh2);
				Msj_numero(HHAI);
				Msj_uart(" %");
				ENVIO_UDR0(13);
				Msj_uart(frmant);
				
				while (cont==0){
					_delay_ms(10);
					if (Datouart==122){
						cont=1;
					}
				}
				limpiaruart();
				llamado=0;
				Msj_uart(frcpar);
				ENVIO_UDR0(13); //salto renglon
				datosmenui();
				MenuI(Datouart);
				while(llamado==0);
				MenuI(Datouart);
			}
			else{
				paraelse();
			}
			break;
			case'S': //humedad del suelo interno deseada
			limpiaruart();
			Msj_uart(frS1);
			ENVIO_UDR0(13);
			cargardato(100);
			if(opcancelada==0){
				HSI=variable;
				valoreeprom=EEPROM_read(10);
				if (valoreeprom!=HSI){
					EEPROM_write(10, HSI);
					ENVIO_UDR0(13);
					Msj_uart(fraceep);
				}
				cont=0;
				ENVIO_UDR0(13);
				Msj_uart(frS2);
				Msj_numero(HSI);
				Msj_uart(" %");
				ENVIO_UDR0(13);
				Msj_uart(frmant);
				
				while (cont==0){
					_delay_ms(10);
					if (Datouart==122){
						cont=1;
					}
				}
				limpiaruart();
				llamado=0;
				Msj_uart(frcpar);
				ENVIO_UDR0(13); //salto renglon
				datosmenui();
				MenuI(Datouart);
				while(llamado==0);
				MenuI(Datouart);
			}
			else{
				paraelse();
			}
			break;
			case 's': //histeresis de humedad del suelo interno
			limpiaruart();
			Msj_uart(frs1);
			ENVIO_UDR0(13);
			cargardato(100);
			if(opcancelada==0){
				HHSI=variable;
				valoreeprom=EEPROM_read(12);
				if (valoreeprom!=HHSI){
					EEPROM_write(12, HHSI);
					ENVIO_UDR0(13);
					Msj_uart(fraceep);
				}
				cont=0;
				ENVIO_UDR0(13);
				Msj_uart(frs2);
				Msj_numero(HHSI);
				Msj_uart(" %");
				ENVIO_UDR0(13);
				Msj_uart(frmant);
				
				while (cont==0){
					_delay_ms(10);
					if (Datouart==122){
						cont=1;
					}
				}
				limpiaruart();
				llamado=0;
				Msj_uart(frcpar);
				ENVIO_UDR0(13); //salto renglon
				datosmenui();
				MenuI(Datouart);
				while(llamado==0);
				MenuI(Datouart);
			}
			else{
				paraelse();
			}
			break;
			case 'V': //velocidad maxima del viento
			limpiaruart();
			Msj_uart(frV1);
			ENVIO_UDR0(13);
			cargardato(100);
			if(opcancelada==0){
				VMAXV=variable;
				valoreeprom=EEPROM_read(14);
				if (valoreeprom!=VMAXV){
					EEPROM_write(14, VMAXV);
					ENVIO_UDR0(13);
					Msj_uart(fraceep);
				}
				cont=0;
				ENVIO_UDR0(13);
				Msj_uart(frV2);
				Msj_numero(VMAXV);
				Msj_uart(" km/h");
				ENVIO_UDR0(13);
				Msj_uart(frmant);
				
				while (cont==0){
					_delay_ms(10);
					if (Datouart==122){
						cont=1;
					}
				}
				limpiaruart();
				llamado=0;
				Msj_uart(frcpar);
				ENVIO_UDR0(13); //salto renglon
				datosmenui();
				MenuI(Datouart);
				while(llamado==0);
				MenuI(Datouart);
			}
			else{
				paraelse();
			}
			break;
			case 'v': //velocidad minima del viento
			limpiaruart();
			Msj_uart(frv1);
			ENVIO_UDR0(13);
			cargardato(100);
			if(opcancelada==0){
				VMINV=variable;
				valoreeprom=EEPROM_read(16);
				if (valoreeprom!=VMINV){
					EEPROM_write(16, VMINV);
					ENVIO_UDR0(13);
					Msj_uart(fraceep);
				}
				cont=0;
				ENVIO_UDR0(13);
				Msj_uart(frv2);
				Msj_numero(VMINV);
				Msj_uart(" km/h");
				ENVIO_UDR0(13);
				Msj_uart(frmant);
				
				while (cont==0){
					_delay_ms(10);
					if (Datouart==122){
						cont=1;
					}
				}
				limpiaruart();
				llamado=0;
				Msj_uart(frcpar);
				ENVIO_UDR0(13); //salto renglon
				datosmenui();
				MenuI(Datouart);
				while(llamado==0);
				MenuI(Datouart);
			}
			else{
				paraelse();
			}
			break;
			case'E': //luminosidad maxima
			limpiaruart();
			Msj_uart(frE1);
			ENVIO_UDR0(13);
			cargardato(100);
			if(opcancelada==0){
				IMAX=variable;
				valoreeprom=EEPROM_read(18);
				if (valoreeprom!=IMAX){
					EEPROM_write(18, IMAX);
					ENVIO_UDR0(13);
					Msj_uart(fraceep);
				}
				cont=0;
				ENVIO_UDR0(13);
				Msj_uart(frE2);
				Msj_numero(IMAX);
				Msj_uart(" LUX");
				ENVIO_UDR0(13);
				Msj_uart(frmant);
				
				while (cont==0){
					_delay_ms(10);
					if (Datouart==122){
						cont=1;
					}
				}
				limpiaruart();
				llamado=0;
				Msj_uart(frcpar);
				ENVIO_UDR0(13); //salto renglon
				datosmenui();
				MenuI(Datouart);
				while(llamado==0);
				MenuI(Datouart);
			}
			else{
				paraelse();
			}
			break;
			case 'e': //luminosidad minima
			cont=0;
			limpiaruart();
			Msj_uart(fre1);
			ENVIO_UDR0(13);
			cargardato(100);
			if(opcancelada==0){
				IMIN=variable;
				valoreeprom=EEPROM_read(20);
				if (valoreeprom!=IMIN){
					EEPROM_write(20, IMIN);
					ENVIO_UDR0(13);
					Msj_uart(fraceep);
				}
				cont=0;
				ENVIO_UDR0(13);
				Msj_uart(fre2);
				Msj_numero(IMIN);
				Msj_uart(" LUX");
				ENVIO_UDR0(13);
				Msj_uart(frmant);

				while (cont==0){
					_delay_ms(10);
					if (Datouart==122){
						cont=1;
					}
				}
				limpiaruart();
				llamado=0;
				Msj_uart(frcpar);
				ENVIO_UDR0(13); //salto renglon
				datosmenui();
				MenuI(Datouart);
				while(llamado==0);
				MenuI(Datouart);
			}
			else{
				paraelse();
			}
			break;
			
			case 'z':
					llamado=0;
					Menu1(Datouart);
					while(llamado==0);
					Menu1(Datouart);
			break;
			default:
					limpiaruart();
					Msj_uart(frin);
					ENVIO_UDR0(13);
					llamado=0;
					Msj_uart("Seleccione parametro a cargar");
					ENVIO_UDR0(13); //salto renglon
					datosmenui();
					MenuI(Datouart);
					while(llamado==0);
					MenuI(Datouart);
		}
	}
}

void cargardato(int limh){
	_delay_ms(10);
	var=0;
	temp=0;
	opcancelada=0;
	contador=0;
	llamado=0;
	Datouart=0;
	while (!(Datouart==13)){
		_delay_ms(5);
		while (llamado==0);
		if (((Datouart<58)&&(47<Datouart))||(Datouart==8)){
			if (Datouart!=8){
				if (contador==2){
					ENVIO_UDR0(Datouart);
					Datouart-=48;
					var=var+Datouart;
					contador++;
					}
				if (contador==1){
					ENVIO_UDR0(Datouart);
					temp=Datouart-48;
					var=var+(temp*10);
					contador++;
					}
				if (contador==0){
					ENVIO_UDR0(Datouart);
					Datouart-=48;
					var=Datouart*100;
					contador++;				
					}
				}
			else {
				if (contador==3){
					contador--;
					var=var/10;
					var=var*10;
					ENVIO_UDR0(8);
					}
				else if(contador==2){
					contador--;
					var=var-(temp*10);
					ENVIO_UDR0(8);
					}
				else if(contador==1){
						contador--;
						var=0;
						ENVIO_UDR0(8);
					}
				}
			}
				llamado=0;
	}	
	if((var==0)&&(Datouart==13)){
		opcancelada=1;
		//limpiaruart();
		Msj_uart("No se ingresaron datos");
		ENVIO_UDR0(13);
	}
	if ((contador==1)&&(opcancelada==0)){
		var/=100;
		variable=var;
	}
	if((contador==2)&&(opcancelada==0)){
		var/=10;
		variable=var;
	}
	if((contador==3)&&(opcancelada==0)){
		if(limh>=var){
			variable=var;
		}
		else{
		ENVIO_UDR0(13);
		Msj_uart("Dato ingresado fuera de rango");
		ENVIO_UDR0(13);
		opcancelada=1;
		}
	}
	}
void Msj_numero(int x){
	num=x;
	c=num/100;
	d=(num/10)%10;
	u=num%10;
	
	ENVIO_UDR0(c+48);
	ENVIO_UDR0(d+48);
	ENVIO_UDR0(u+48);
}
void datosmenui(){
	ENVIO_UDR0(13);
	Msj_uart(frT);
	ENVIO_UDR0(13);
	Msj_uart(frt);
	ENVIO_UDR0(13);
	Msj_uart(frk);
	ENVIO_UDR0(13);
	Msj_uart(frH);
	ENVIO_UDR0(13);
	Msj_uart(frh);
	ENVIO_UDR0(13);
	Msj_uart(frS);
	ENVIO_UDR0(13);
	Msj_uart(frs);
	ENVIO_UDR0(13);
	Msj_uart(frV);
	ENVIO_UDR0(13);
	Msj_uart(frv);
	ENVIO_UDR0(13);
	Msj_uart(frE);
	ENVIO_UDR0(13);
	Msj_uart(fre);
	ENVIO_UDR0(13);
	Msj_uart(frmant);
	ENVIO_UDR0(13); //salto renglon
}

void paraelse(){
	Msj_uart("Seleccione nuevamente el parametro a cargar");
	datosmenui();
	MenuI(Datouart);
	while(llamado==0);
	MenuI(Datouart);
}

void EEPROM_write(unsigned int uiAddress, unsigned char ucData){
	while(EECR & (1<<EEPE));
	EEAR = uiAddress;
	EEDR = ucData;
	EECR |= (1<<EEMPE);
	EECR |= (1<<EEPE);
}

unsigned char EEPROM_read(unsigned int uiAddress){
	while(EECR & (1<<EEPE));
	EEAR = uiAddress;
	EECR |= (1<<EERE);
	return EEDR;
}

void lcdT(){
	Seleccion_I2C(0x70);//direccion 70 para el i2c
	Envio_Com(0x01); //limpio lcd
	Direccion(01); //diireccion 1 en el lcd para escribir la frase de abajo
	LCD_Mensaje(frT);
	Direccion(68);
	LCD_Mensaje("temp");
	LCD_Mensaje(frgrado);
}
void lcdt(){
	Seleccion_I2C(0x70);
	Envio_Com(0x01);
	Direccion(01);
	LCD_Mensaje(frt);
	Direccion(90);
	LCD_Mensaje("temp");
	LCD_Mensaje(frgrado);
	for (int i=14; i<=35; i++)//va corriendo el cursos para desplazar el texto en el lcd
	{
		Direccion(i);
		Envio_Com(0x18);	//es la direccion de lcd para que se desplaze
		_delay_ms(100);
	}
}
void lcdk(){
	Seleccion_I2C(0x70);
	Envio_Com(0x01);
	Direccion(00);
	LCD_Mensaje(frk);
	Direccion(93);
	LCD_Mensaje("temp");
	LCD_Mensaje(frgrado);
	for (int i=14; i<=39; i++)
	{
		Direccion(i);
		Envio_Com(0x18);
		_delay_ms(100);
	}
}
void lcdH(){
	Seleccion_I2C(0x70);
	Envio_Com(0x01);
	Direccion(00);
	LCD_Mensaje(frH);
	Direccion(71);
	LCD_Mensaje("temp");
	LCD_Mensaje(frporcen);
	for (int i=14; i<=16; i++)
	{
		Direccion(i);
		Envio_Com(0x18);
		_delay_ms(100);
	}
}
void lcdh(){
	Seleccion_I2C(0x70);
	Envio_Com(0x01);
	Direccion(00);
	LCD_Mensaje(frh);
	Direccion(94);
	LCD_Mensaje("temp");
	LCD_Mensaje(frporcen);
	for (int i=14; i<=40; i++)
	{
		Direccion(i);
		Envio_Com(0x18);
		_delay_ms(100);
	}
}
void lcdS(){
	Seleccion_I2C(0x70);
	Envio_Com(0x00);
	Direccion(01);
	LCD_Mensaje(frS);
	Direccion(71);
	LCD_Mensaje("temp");
	LCD_Mensaje(frporcen);
	for (int i=14; i<=16; i++)
	{
		Direccion(i);
		Envio_Com(0x18);
		_delay_ms(100);
	}
}
void lcds(){
	Seleccion_I2C(0x70);
	Envio_Com(0x00);
	Direccion(01);
	LCD_Mensaje(frs);
	Direccion(93);
	LCD_Mensaje("temp");
	LCD_Mensaje(frporcen);
	for (int i=14; i<=38; i++)
	{
		Direccion(i);
		Envio_Com(0x18);
		_delay_ms(100);
	}
}
void lcdV(){
	Seleccion_I2C(0x70);
	Envio_Com(0x01);
	Direccion(00);
	LCD_Mensaje(frV);
	Direccion(78);
	LCD_Mensaje("temp");
	LCD_Mensaje(frm7s);
	for (int i=14; i<=24; i++)
	{
		Direccion(i);
		Envio_Com(0x18);
		_delay_ms(100);
	}
}
void lcdv(){
	
	Envio_Com(0x01);
	Direccion(00);
	LCD_Mensaje(frv);
	Direccion(78);
	LCD_Mensaje("temp");
	LCD_Mensaje(frm7s);
	for (int i=14; i<=24; i++)
	{
		Direccion(i);
		Envio_Com(0x18);
		_delay_ms(100);
	}
}
void lcdE(){
	Seleccion_I2C(0x70);
	Envio_Com(0x01);
	Direccion(00);
	LCD_Mensaje(frE);
	Direccion(74);
	LCD_Mensaje("temp");
	LCD_Mensaje(frlux);
	for (int i=14; i<=21; i++)
	{
		Direccion(i);
		Envio_Com(0x18);
		_delay_ms(100);
	}
}
void lcde(){
	Seleccion_I2C(0x70);
	Envio_Com(0x01);
	Direccion(00);
	LCD_Mensaje(fre);
	Direccion(74);
	LCD_Mensaje("temp");
	LCD_Mensaje(frlux);
	for (int i=14; i<=21; i++)
	{
		Direccion(i);
		Envio_Com(0x18);
		_delay_ms(100);
	}
}
void lcdVIV(){
	Seleccion_I2C(0x70);
	Envio_Com(0x01);
	Direccion(0x02);
	LCD_Mensaje(Presentlcd);
}

ISR(INT0_vect){
	
	if (ContadorMuestroLCD>=12)
	{
		ContadorMuestroLCD=0;
	}
	//
	ContadorMuestroLCD++;
}

void init_Int0(){
	DDRD=(0<<DDD2);
	PORTD=(1<<PORTD2);  //PD2 activo resistencia PULL-UP
	EICRA=(1<<ISC01)|(1<<ISC00);
	EIMSK=(1<<INT0);
	sei();
}

