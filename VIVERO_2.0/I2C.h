

#ifndef TWI_H_
#define TWI_H_

#include <avr/io.h>
#include <stdbool.h>

///// TIW //////
#define	TWI_W			0x00
#define	TWI_R			0x01
#define TWI_ACK			0x01
#define TWI_NACK		0x00

#define TWI_START		0x08
#define TWI_RESTART		0x10
#define TWI_WT_SLA_ACK	0x18
#define TWI_MT_DATA_ACK	0x28
#define TWI_RD_SLA_ACK	0x40

void I2C_Init();
void I2C_startCond();
void I2C_restrtCond();
void I2C_stopCond();
void I2C_write(uint8_t);
int I2C_read(uint8_t);
void I2C_sendAdrr(uint8_t, uint8_t);

 void Inicio_I2C();
 void Start_I2C();
 void Stop_I2C();
 void Seleccion_I2C(char x);

#endif /* TWI_H_ */