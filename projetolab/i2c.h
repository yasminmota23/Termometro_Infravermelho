
void i2c_init(){
	TWBR = 0x62;		//A taxa de transmiss�o � definida calculando (verificar)
	TWCR = (1<<TWEN);	//Habilita I2C
	TWSR = 0x00;		//Prescaler setado para 1 

}
//Condi��o de partida
void i2c_start(){
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTA);	//condi��o inicial
	while (!(TWCR & (1<<TWINT)));				//verifica��o da condi��o inicial

}
//I2C condi��o de parada
void i2c_write(char x){				// Com esta fun��o se escreve no barramento TWD
	TWDR = x;						//Move o valor para I2C
	TWCR = (1<<TWINT) | (1<<TWEN);	//Habilita I2C e cancela a interrup��o
	while  (!(TWCR &(1<<TWINT)));
}

char i2c_read(){
	TWCR  = (1<<TWEN) | (1<<TWINT);	//Habilita I2C e cancela a interrup��o
	while (!(TWCR & (1<<TWINT)));	//Leitura bem-sucedida com todos os dados recebidos em TWDR
	return TWDR;
}