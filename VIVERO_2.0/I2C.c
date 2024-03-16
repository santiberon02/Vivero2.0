/*
 * I2C.c
 *
 * Created: 3/3/2024 00:17:58
 *  Author: Usuario
 */ 
 #include "I2C.h"
 
 void I2C_Init(){

	/////// Setting frequency ///////
	DDRC = (1<<PORTC4)|(1<<PORTC5); //pull up
	TWSR = 0x00;                // Preescalador en 1
	TWBR = 0x0C;                // 400 Khz
	TWCR = (1<<TWEN);
 }

void I2C_startCond(){
	TWCR = ((1<<TWINT) | (1<<TWSTA) | (1<<TWEN)); //se limpia la flag de interrupcion, se habilita la comunicacion, se activa la comunicacion

	while(!(TWCR & (1<<TWINT)));				// se espera la bandera de interrupion sea 1

 }

 void I2C_restrtCond(){
   TWCR = ((1<<TWINT) | (1<<TWSTA) | (1<<TWEN)); //todo igual q el start 

   while(!(TWCR & (1<<TWINT)));
 }

 void I2C_stopCond(){
	TWCR |= ((1<<TWINT) | (1<<TWSTO) | (1<<TWEN)); //limpiamos bandera de int, se ordena apaga la comunicacion, habilitamos la comunicacion
 }

void I2C_sendAdrr(uint8_t adrr, uint8_t action){
	
	if (action == TWI_W){							//que se va a hacer?? 
		adrr &=~ 1;									// si el primer bit es 0 se escribe el esclavo
	}
	else{
		adrr |= 1;									//si el primer bit es 1 se lee el esclavo
	}

	TWDR = adrr;									//se carga la direccion sumada el bit de twi
	TWCR = ((1<<TWINT) | (1<<TWEN));				// se limpia la int y se activa la comu

	while(!(TWCR & (1<<TWINT)));					// esperamos la int sea 1
 }

 void I2C_write(uint8_t data2write){

	TWDR = data2write;								//valor que se quiere enviar al esclavo
	TWCR = ((1<<TWINT) | (1<<TWEN));				// se limpia la int y se activa la comu
	while(!(TWCR & (1<<TWINT)));					// esperamos la int sea 1

 }

int I2C_read(uint8_t ACK_NACK){
	
	TWCR = ((1 << TWINT) | (1 << TWEN) | (ACK_NACK << TWEA));	//se limpia bandera de int. activa la comunicacion; Se carga sobre TWEA un 1 o un 0
																//con 1 sigue la comu, con 0 se corta todo
	while(!(TWCR & (1<<TWINT)));								// esperamos la int sea 1
	return TWDR;
 }
 /////////////////////////////////////////////////////////////


 void Inicio_I2C()
 {
	 
	 DDRC = (1<<PORTC4)|(1<<PORTC5); //pull up
	 TWSR = 0x00;                // Preescalador en 1
	 TWBR = 0x0C;                // 400 Khz
	 TWCR = (1<<TWEN);
 }

 void Start_I2C()  //se usa cada vez que queres llamar a un sensor
 {
	 TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTA);
	 while(!(TWCR & (1<<TWINT)));  //espera q se libere el bus de envio de datos
 }

 void Seleccion_I2C(char x)
 {
	 TWDR = x;
	 TWCR = (1<<TWINT)|(1<<TWEN);
	 while(!(TWCR & (1<<TWINT)));
 }

 void Stop_I2C()
 {
	 TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
 }