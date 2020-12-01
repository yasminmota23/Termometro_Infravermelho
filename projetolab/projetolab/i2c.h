
void i2c_init(){
	TWBR = 0x62;		//A taxa de transmissão é definida calculando (verificar)
	TWCR = (1<<TWEN);	//Habilita I2C
	TWSR = 0x00;		//Prescaler setado para 1 

}
//Condição de partida
void i2c_start(){
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTA);	//condição inicial
	while (!(TWCR & (1<<TWINT)));				//verificação da condição inicial

}
//I2C condição de parada
void i2c_write(char x){				// Com esta função se escreve no barramento TWD
	TWDR = x;						//Move o valor para I2C
	TWCR = (1<<TWINT) | (1<<TWEN);	//Habilita I2C e cancela a interrupção
	while  (!(TWCR &(1<<TWINT)));
}

char i2c_read(){
	TWCR  = (1<<TWEN) | (1<<TWINT);	//Habilita I2C e cancela a interrupção
	while (!(TWCR & (1<<TWINT)));	//Leitura bem-sucedida com todos os dados recebidos em TWDR
	return TWDR;
}