//direcciones
uint8_t Direccion_ADS1110 = 0x90;
uint8_t Direccion_HTU21D = 0x80;
uint8_t Direccion_BMP180 = 0XEE;	
uint8_t Direccion_FDC1004 = 0x50;
//configuraciones
uint8_t Lectura_Humin = 0xE5;
uint8_t Lectura_Tempin = 0xE3;
uint8_t LecturaLux = 0x9C;
uint8_t MSB,LSB;
uint16_t AC1, AC2, AC3, AC4, AC5, AC6, B1, B2, MB, MC, MD;
uint32_t UT, UP, X1, X2, B5, B6, X3,B3,B4,B7;

uint32_t Temporal = 0;
int contar=0;
float valor_humi;
float valor_tempi;
float valor_lux;
float valor_humt;
float v_sensor;
float valor_tempext=0;
float valor_presext;
void ads1110();
void HTU21D();
void FDC1004();
void BMP180();
char BufferUSART[30];
char humint[3];
char tempint[4];
char humt[7];
char tempext[7];
char presext[7];

void HTU21D(){
//humedad interna 
	Start_I2C();
	
	I2C_sendAdrr(Direccion_HTU21D, TWI_W);
	I2C_write(Lectura_Humin);
	
	I2C_restrtCond();
	
	I2C_sendAdrr(Direccion_HTU21D, TWI_R);
	Temporal = I2C_read(TWI_ACK);								//leo parte alta segun el datasheet y envio el ack
	Temporal = Temporal << 8;									// corro los 8 bits de la parte alta para recibir la parte baja y no sobreescribir
	Temporal = Temporal + I2C_read(TWI_NACK);					//leo parte alta y envio el nack para finalizar
	
	Stop_I2C();
	
	valor_humi = -6 + (125 * ((Temporal)/65536.0));				//conversion del resultado
	dtostrf(valor_humi,3,0,humint);	
	
	ENVIO_UDR0(13);
	Msj_uart(humint);
	ENVIO_UDR0(13);							//pasamos el valor de la variable tipo float a tipo string	//Conversion del resultado

//reset sensor	
	Start_I2C();												//Comienzo de comunicacion

	I2C_sendAdrr(Direccion_HTU21D,TWI_W);
	I2C_write(0XFE);											//Reseteo el registro del sensor a predeterminado
	
	Stop_I2C();
	
//temperatura interna
	Start_I2C();
	
	I2C_sendAdrr(Direccion_HTU21D, TWI_W);
	I2C_write(Lectura_Tempin);
	
	I2C_restrtCond();
	
	I2C_sendAdrr(Direccion_HTU21D, TWI_R);
	Temporal = I2C_read(TWI_ACK);								//leo parte alta segun el datasheet y envio el ack
	Temporal = Temporal << 8;									// corro los 8 bits de la parte alta para recibir la parte baja y no sobreescribir
	Temporal = Temporal + I2C_read(TWI_NACK);					//leo parte alta y envio el nack para finalizar 
	
	Stop_I2C();
	
	valor_tempi = -46.5 + (175.72 * ((Temporal)/65536.0));			//Conversion del resultado
	dtostrf(valor_tempi,4,0,tempint);
			//pasamos el valor de la variable tipo float a tipo string
	ENVIO_UDR0(13);
	Msj_uart(tempint);
	ENVIO_UDR0(13);
//reset sensor
	Start_I2C();												//Comienzo de comunicacion
	
	I2C_sendAdrr(Direccion_HTU21D,TWI_W);
	I2C_write(0XFE);											//Reseteo el registro del sensor a predeterminado
	
	Stop_I2C();
	
	
} 
//funciona 

void FDC1004(){
	Temporal=0;
	I2C_startCond();
	
	I2C_sendAdrr(Direccion_FDC1004, TWI_W);
	I2C_write(0x08);								//direccion de configuracion

	I2C_write(0b00011100);							//canal 1 de bits 15 a 8
	I2C_write(0b00000000);							//capda on y 96pf
	
	I2C_stopCond();
	_delay_ms(10);
	
	I2C_restrtCond();
	
	I2C_sendAdrr(Direccion_FDC1004, TWI_W);
	I2C_write(0x0C);
	I2C_write(0b00000100);							//leo a 200s/s y sin repeticion 
	I2C_write(0b10000000);							//activo la medicion 1
	
	I2C_stopCond();
	_delay_ms(300);
	
	I2C_startCond();
	I2C_sendAdrr(Direccion_FDC1004,TWI_W);
	I2C_write(0x00);
	I2C_stopCond();
	_delay_ms(10);
	
	I2C_startCond();
	I2C_sendAdrr(Direccion_FDC1004,TWI_R);
	Temporal = I2C_read(TWI_ACK);								// Lectura de la primera parte de la parte alta de la lectura del sensor + Bit de reconocimiento
	Temporal <<= 8;
	Temporal = Temporal + I2C_read(TWI_NACK);					// Lectura de la segunda parte de la parte alta del sensor + Bit de no reconocimiento
	Temporal <<= 8;
	
	_delay_ms(10);
	
	I2C_restrtCond();
	I2C_sendAdrr(Direccion_FDC1004,TWI_W);
	I2C_write(0x01);
	I2C_stopCond();
	
	_delay_ms(10);
	
	I2C_restrtCond();
	I2C_sendAdrr(Direccion_FDC1004,TWI_R);
	Temporal =Temporal + I2C_read(TWI_ACK);								// Lectura de la primera parte de la parte alta de la lectura del sensor + Bit de reconocimiento
	I2C_read(TWI_NACK);
	I2C_stopCond();	
	
	valor_humt =Temporal*15.0/8388608.0;

	
	
	dtostrf(valor_humt,2,0,humt);		
	
		ENVIO_UDR0(13);
		Msj_uart(humt);
		ENVIO_UDR0(13);
}
//no funcion

void BMP180(){
	
	I2C_startCond();
	
	if (contar==0)
	{
	I2C_sendAdrr(Direccion_BMP180, TWI_W);
	I2C_write(0XAA);
	
	I2C_restrtCond();
	
	I2C_sendAdrr(Direccion_BMP180,TWI_R);
	
	Temporal=I2C_read(TWI_ACK);
	Temporal<<=8;
	AC1=Temporal+I2C_read(TWI_ACK);
	
	Temporal=I2C_read(TWI_ACK);
	Temporal<<=8;
	AC2=Temporal+I2C_read(TWI_ACK);
	
	Temporal=I2C_read(TWI_ACK);
	Temporal<<=8;
	AC3=Temporal+I2C_read(TWI_ACK);
	
	Temporal=I2C_read(TWI_ACK);
	Temporal<<=8;
	AC4=Temporal+I2C_read(TWI_ACK);
	
	Temporal=I2C_read(TWI_ACK);
	Temporal<<=8;
	AC5=Temporal+I2C_read(TWI_ACK);
	
	Temporal=I2C_read(TWI_ACK);
	Temporal<<=8;
	AC6=Temporal+I2C_read(TWI_ACK);
	
	Temporal=I2C_read(TWI_ACK);
	Temporal<<=8;
	B1=Temporal+I2C_read(TWI_ACK);
	
	Temporal=I2C_read(TWI_ACK);
	Temporal<<=8;
	B2=Temporal+I2C_read(TWI_ACK);
	
	Temporal=I2C_read(TWI_ACK);
	Temporal<<=8;
	MB=Temporal+I2C_read(TWI_ACK);
	 
	Temporal=I2C_read(TWI_ACK);
	Temporal<<=8;
	MC=Temporal+I2C_read(TWI_ACK);
	
	Temporal=I2C_read(TWI_ACK);
	Temporal<<=8;
	MD=Temporal+I2C_read(TWI_NACK);
	contar++;
	}
	
	I2C_stopCond();
	
	I2C_startCond();
	
	I2C_sendAdrr(Direccion_BMP180,TWI_W);
	I2C_write(0XF4);
	I2C_write(0X2E);
	
	_delay_ms(4.5);
	
	I2C_restrtCond();
	I2C_sendAdrr(Direccion_BMP180,TWI_W);
	I2C_write(0XF6);
	
	I2C_restrtCond();
	I2C_sendAdrr(Direccion_BMP180,TWI_R);
	MSB=I2C_read(TWI_ACK);
	LSB=I2C_read(TWI_NACK);
	
	I2C_stopCond();
	
		UT=(MSB<<8) + LSB;
		X1=(UT-AC6)*AC5/32768;
		X2=(MC*2048)/(X1+MD);
		B5=X1+X2;
		valor_tempext=((B5+8)/16)*0.1;
		
		dtostrf(valor_tempext,2,1,tempext);
		ENVIO_UDR0(13);
		Msj_uart(tempext);
		ENVIO_UDR0(13);
/*	I2C_restrtCond();
	I2C_sendAdrr(Direccion_BMP180,TWI_W);
	
	I2C_write(0xF4);
	I2C_write(0x34+(0<<6));
	
	_delay_ms(5);
	
	I2C_restrtCond();
	I2C_sendAdrr(Direccion_BMP180,TWI_W);
	I2C_write(0xF6);
	
	I2C_restrtCond();
	
	I2C_sendAdrr(Direccion_BMP180, TWI_R);
	Temporal=I2C_read(TWI_ACK);
	Temporal<<=8;
	Temporal=Temporal+I2C_read(TWI_ACK);
	Temporal<<=8;
	Temporal=Temporal+I2C_read(TWI_NACK);
	UP=Temporal>>(8-0);
	
	I2C_stopCond();
	
	B6=B5-4000;
	X1=(B2*(B6*B6/4096))/2048;
	X2=AC2*B6/2048;
	X3=X1+X2;
	B3=(((AC1*4+X3)<<0)+2)/4;
	X1=AC3*B6/8192;
	X2=(B1*(B6*B6/4096))/65536;
	X3=((X1+X2)+2)/4;
	B4=AC4*(X3+32768)/32768;
	B7=UP-B3*(50000>>0);
	if(B7<0x80000000){
		valor_presext=(B7*2)/B4;
		}
	else
	{
		valor_presext=(B7/B4)*2;
	}
	X1=(valor_presext/256)*(valor_presext/256);
	X1=(X1*3038)/65536;
	X2=(-7357*valor_presext)/65536;
	valor_presext=valor_presext+(X1+X2+3791)/16;
	*/
	/*f(valor_presext,4,2,presext);
	ENVIO_UDR0(13);
	Msj_uart(presext);
	ENVIO_UDR0(13);*/
}
//no funciona

void ads1110(){
	Temporal=0;
	v_sensor=0;
	valor_lux=0;
	I2C_startCond();
	
	I2C_sendAdrr(Direccion_ADS1110,TWI_W);
	I2C_write(LecturaLux);
	
	I2C_restrtCond();
	
	I2C_sendAdrr(Direccion_ADS1110,TWI_R);
	Temporal=I2C_read(TWI_ACK);
	Temporal=Temporal<<8;
	v_sensor=Temporal+I2C_read(TWI_NACK);
	
	I2C_stopCond();
	
	valor_lux=(v_sensor/2.048)*32678.0;
	
}
//no funciona