

//-----Protótipos--------//
void lcd_init();			//----Função de inicialização do LCD
void toggle();				//----Função de travamento do LCD
void lcd_cmd_hf(char v1);   //----Função para enviar comando de meio byte para LCD
void lcd_cmd(char v2);		//----Função para enviar Comando para LCD
void lcd_dwr(char v3);		//----Função para enviar dados para LCD
void lcd_msg(char *c);		//----Função para enviar string para LCD
void delay(int ms);			//----Função de atraso
void lcd_lef_sh();			//----Função de deslocamento para a esquerda
void lcd_rig_sh();			//----Função de deslocamento para a direita
//----------------------//

void toggle()
{
	TWDR |= 0x02;					//--------Travando dados no registro de dados LCD usando sinal de alto para baixo
	TWCR = (1<<TWINT) | (1<<TWEN);	//---Habilita I2C e limpa a interrupção
	while  (!(TWCR &(1<<TWINT)));	
	delay(1);
	TWDR &= ~0x02;					//---PINO de ativação do LCD em = 0;
	TWCR = (1<<TWINT) | (1<<TWEN);	//---Habilita I2C e limpa a interrupção
	while  (!(TWCR &(1<<TWINT)));
}

void lcd_cmd_hf(char v1)
{
	TWDR &=~0x01;					//rs = 0; ----Selecionando registro como registro de comando
	TWCR = (1<<TWINT) | (1<<TWEN);	//Habilita I2C e limpa a interrupção
	while  (!(TWCR &(1<<TWINT)));
	TWDR &= 0x0F;					//----limpando os 4 bits mais altos
	TWCR = (1<<TWINT) | (1<<TWEN);	//Habilita I2C e limpa a interrupção
	while  (!(TWCR &(1<<TWINT)));
	TWDR |= (v1 & 0xF0);			//----Mascarando 4 bits mais altos e enviando para LCD
	TWCR = (1<<TWINT) | (1<<TWEN);	//Habilita I2C e limpa a interrupção
	while  (!(TWCR &(1<<TWINT)));
	toggle();
}

void lcd_cmd(char v2)
{
	TWDR&=~0x01;					//rs = 0; ----Selecionando registro como registro de comando
	TWCR = (1<<TWINT) | (1<<TWEN);	//Habilita I2C e limpa a interrupção
	while  (!(TWCR &(1<<TWINT)));
	TWDR &= 0x0F;                   //----limpando os 4 bits mais altos
	TWCR = (1<<TWINT) | (1<<TWEN);	//Habilita I2C e limpa a interrupção
	while  (!(TWCR &(1<<TWINT)));
	TWDR |= (v2 & 0xF0);			//----Mascarando 4 bits mais altos e enviando para LCD
	TWCR = (1<<TWINT) | (1<<TWEN);	//Habilita I2C e limpa a interrupção
	while  (!(TWCR &(1<<TWINT)));
	toggle();
	
	TWDR &= 0x0F;                    //----limpando os 4 bits mais altos
	TWCR = (1<<TWINT) | (1<<TWEN);	//Habilita I2C e limpa a interrupção
	while  (!(TWCR &(1<<TWINT)));
	TWDR |= ((v2 & 0x0F)<<4);		//----Mascarando 4 bits mais baixos e enviando para LCD
	TWCR = (1<<TWINT) | (1<<TWEN);	//Habilita I2C e limpa a interrupção
	while  (!(TWCR &(1<<TWINT)));
	toggle();
}

void lcd_dwr(char v3)
{
	TWDR|=0x01;						//rs = 1; ----Selecionando registro como registro de comando
	TWCR = (1<<TWINT) | (1<<TWEN);	//Habilita I2C e limpa a interrupção
	while  (!(TWCR &(1<<TWINT)));
	TWDR &= 0x0F;				    //----limpando os 4 bits mais altos
	TWCR = (1<<TWINT) | (1<<TWEN);	//Habilita I2C e limpa a interrupção
	while  (!(TWCR &(1<<TWINT)));
	TWDR |= (v3 & 0xF0);			//----Mascarando 4 bits mais altos e enviando para LCD
	TWCR = (1<<TWINT) | (1<<TWEN);	//Habilita I2C e limpa a interrupção
	while  (!(TWCR &(1<<TWINT)));
	toggle();
	
	TWDR &= 0x0F;					//----limpando os 4 bits mais altos
	TWCR = (1<<TWINT) | (1<<TWEN);	//Habilita I2C e limpa a interrupção
	while  (!(TWCR &(1<<TWINT)));
	TWDR |= ((v3 & 0x0F)<<4);		//----Mascarando 4 bits mais baixos e enviando para LCD
	TWCR = (1<<TWINT) | (1<<TWEN);	//Habilita I2C e limpa a interrupção
	while  (!(TWCR &(1<<TWINT)));
	toggle();
}

void lcd_init()
{
	lcd_cmd_hf(0x30);       //------ Sequência para inicializar LCD
	lcd_cmd_hf(0x30);       //-----     "            "              "               "
	lcd_cmd_hf(0x20);       //-----     "            "              "               "
	lcd_cmd(0x28);          //-----Seleciona o LCD 16 x 2 no modo 4 bits
	lcd_cmd(0x0C);          //-----Display ON Cursor OFF
	lcd_cmd(0x01);          //-----Limpar exibição
	lcd_cmd(0x06);          //-----Incremento Automático do Cursor
	lcd_cmd(0x80);          //-----1ª linha 1ª localização do LCD
}

void delay(int ms)
{
	int i,j;
	for(i=0;i<=ms;i++)
	for(j=0;j<=120;j++);
}

void lcd_msg(char *c)
{
	while(*c != 0)      //Espera até que todas as strings sejam passadas para o LCD
	lcd_dwr(*c++);		//Envia a string para o LCD
}

void lcd_rig_sh()
{
	lcd_cmd(0x1C);      //Comando para deslocamento à direita
	delay(400);
}

void lcd_lef_sh()
{
	lcd_cmd(0x18);      //Comando para deslocamento à esquerda
	delay(200);
}
