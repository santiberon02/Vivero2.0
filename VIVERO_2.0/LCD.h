
#include <avr/io.h>
#include <util/delay.h>
char Presentlcd[]="Vivero 2.0";

void Envio_Dato(char v1);
void Envio_Com(char v2);
void Inicio_LCD();
void LCD_Com_Config(char v5);
void Pin_Enable();
void LCD_Mensaje(const char *c);
void Direccion(char v3);
void Mostradatos();

void Direccion(char v3)
{
	v3|= 0x80;
	TWDR&=~0x01;                       // Configuramos "RS" en "0", para los envios de comandos.
	TWCR=(1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	TWDR&= 0x0F;                       // Clear de la parte alta de "TWDR".
	TWCR=(1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	TWDR|=(v3 & 0xF0);                 // Clear de la parte baja de "v2" y enviamos la parte alta a "TWDR".
	TWCR=(1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	Pin_Enable();                      // Evio parte alta.
	
	TWDR&= 0x0F;
	TWCR =(1<<TWINT)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	TWDR|= ((v3 & 0x0F)<<4);
	TWCR =(1<<TWINT)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	Pin_Enable();                     // Envio parte baja.
}

void Pin_Enable()                      // En este proceso se pone el pin "Enable" del LCD en "1" y luego en "0", para habilitar los envios de datos.
{
	TWDR|=0x02;                        // Colocamos en "1" el pin "E" (Enable) del LCD.
	TWCR =(1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	_delay_ms(1);
	TWDR &= ~0x02;
	TWCR =(1<<TWINT)|(1<<TWEN);       // Colocamos en "0" el pin "E" (Enable) del LCD.
	while (!(TWCR & (1<<TWINT)));
}

void Envio_Dato(char v1)
{
	TWDR|= 0x01;                       // Colocamos "RS" en "1", para el envios de datos o caracter.
	TWCR =(1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	TWDR &= 0x0F;                      // Limpio los bit de más significativos de TWDR
	TWCR =(1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	TWDR |= (v1 & 0xF0);               // Envio solo los bits más significativos de "v1"
	TWCR =(1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	Pin_Enable();                      // Envio parte alta
	TWDR &= 0x0F;
	TWCR =(1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	TWDR |=((v1 & 0x0F)<<4);			//desplaza 4 lugares la parte alta de lo que envia que vendria a ser parte baja ya que solo se trabaja con los 4 bits mas grande el lcd
	TWCR =(1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	Pin_Enable();                       //Envio parte baja
}

void Envio_Com(char v2)
{
	TWDR&=~0x01;                       // Configuramos "RS" en "0", para los envios de comandos.
	TWCR=(1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	TWDR&= 0x0F;                       // Clear de la parte alta de "TWDR".
	TWCR=(1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	TWDR|=(v2 & 0xF0);                 // Clear de la parte baja de "v2" y enviamos la parte alta a "TWDR".
	TWCR=(1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	Pin_Enable();                      // Evio parte alta.
	
	TWDR&= 0x0F;
	TWCR =(1<<TWINT)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	TWDR|= ((v2 & 0x0F)<<4);
	TWCR =(1<<TWINT)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	Pin_Enable();                     // Envio parte baja.
}

void LCD_Mensaje(const char *c)
{
	while(*c != 0) //una vez que *c este vacio se termina el proceso
	Envio_Dato(*c++);
}

void LCD_Com_Config(char v5)
{
	TWDR&= ~0x01;                   // colocamos "RS" en "0", ya que se trata de un comando
	TWCR =(1<<TWINT)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	TWDR&= 0x0F;                   // Hacemos un clear de la parte alta de "TWDR"
	TWCR =(1<<TWINT)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	TWDR|=(v5 & 0xF0);             // Enviamos la parte alta de "v4" a "TWDR"
	TWCR =(1<<TWINT)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	Pin_Enable();
}

void Inicio_LCD()
{
	LCD_Com_Config(0x30);        // Bits de inicio del LCD
	_delay_ms(5);
	LCD_Com_Config(0x30);        // Bits de inicio del LCD
	_delay_us(100);
	LCD_Com_Config(0x20);        // Bits de inicio del LCD
	Envio_Com(0x28);             // Activamos el uso de ambas lineas del LCD y lo configuramos para que trabaje con 4 bits.
	Envio_Com(0x0C); //0C      0F      //  Encendemos el display
	Envio_Com(0x01);             // Clear display
//	Envio_Com(0x06);             // Moviento del cursor
//	Envio_Com(0x80);             // Posición
}