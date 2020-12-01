#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "i2c.h"
#include "LCD_I2C.h"

#define set_bit(Y,bit_x) (Y|=(1<<bit_x)) //ativa bit
#define clr_bit(Y,bit_x) (Y&=~(1<<bit_x)) //limpa bit
#define tst_bit(Y,bit_x) (Y&(1<<bit_x)) //testa bit
#define LED PB0
#define tam_vetor 4
#define som PB1
//---------------------------------------------------------------------------------
int off=1,on=0;

unsigned char temp_string[tam_vetor];
uint16_t temp = 0;

ISR (INT0_vect){  // ON ou OFF
	
	on=!on;
	off=!off;
	
}


void int2string(unsigned int valor, unsigned char *disp)
{

	for(uint8_t n=0; n<tam_vetor; n++)
	disp[n] = 0 + 48; //limpa vetor para armazenagem dos digitos
	disp += (tam_vetor-1);
	do
	{
		*disp = (valor%10) + 48; //pega o resto da divisão por 10
		valor /=10; //pega o inteiro da divisão por 10
		disp--;
	}while (valor!=0);
	
	
}
int main()
{
	DDRB = 0x01; //pino do LED como saída
	DDRB |= (1<<som); //habilita a saída de som
	DDRC= 0X00; // PORTA C como entrada
	DDRD = 0x00;//PORTA D(do comparador) como entrada 
	PORTC= 0XFE; // desabilita o pull-up do PC0
	PORTD = 0xFF; //habilita pull-ups
	PORTB = 0xFE; //apaga LED e habilita pull-ups
	// pwm

	ICR1 = 0xFFFF;
	// seta TOP para 16bit
	
	// seta PWM com 75% duty cycle ->  49152/65536 =75%


	TCCR1A |= (1 << COM1A1)|(1 << COM1B1);
	// seta modo não invertido

	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12)|(1 << WGM13);
	// seta PWM no modo rápido usando ICR1 como TOP
	
	TCCR1B |= (1 << CS10);
	// Inicia o timer1 sem prescaler
//-----------------------------------------------------------
	DIDR1 = 0b00000011; //desabilita as entradas digitais nos pinos AIN0 e AIN1
	
	// Interrupções
	EICRA |= (1<<ISC01) | (0<<ISC00);
	EIMSK = (1<<INT0); //habilita a interrupção externa 0
	//Configuração do ADC
	ADMUX = 0b01000000; //Tensão interna de ref (1.1V), canal 0
	ADCSRA = 0b11101111; //Habilita o AD, habilita interrupção, modo de conversão contínua, prescaler = 128
	ADCSRB = 0x00; //Modo de conversão contínua
	set_bit(DIDR0,0); //desabilita pino PC0 como I/0, entrada do ADC0
	
	sei(); //habilita a chave geral de interrupções
	
	i2c_init();
	i2c_start();
	i2c_write(0x70);
	lcd_init();
	
	while(1){
		if(on==1){
			ACSR = 1<<ACIE; //habilita interrupção por mudança de estado na saída do comparador
			lcd_cmd(0x80);
			lcd_msg("Temperatura:");
			lcd_cmd(0xC3);
			int2string(temp,temp_string);
			lcd_msg(temp_string);
			lcd_cmd(0xC7); 
			lcd_msg(" Graus");
			if(temp<36 || temp>38){
				OCR1A=16384; // duty=25%
			}else
				OCR1A = 49152; // gera pwm com duty=75%, 49152/65536;
		}
		
	   if(off==1){
		   
		    OCR1A = 0; // desativa o pwm
		    ACSR = 0<<ACIE; //desabilita interrupção por mudança de estado na saída do comparador
		    clr_bit(PORTB,LED); // desliga o LED
			lcd_cmd(0x01); // limpa display
	   }
			
	}
}

//Interrupção do Comparador Analógico

ISR(ANALOG_COMP_vect)//O LED vai acender quando o ambiente tiver com pouca luminosidade (tensão maior no terminal positivo do comparador)
{ 
	if(tst_bit(ACSR,ACO))//verifica a mudança ocorrida na saída do comparador
	set_bit(PORTB,LED);
	else
	clr_bit(PORTB,LED);
}


ISR(ADC_vect)
{
	temp=ADC;
	temp = (temp/100000.0)*1465.0+30;
	 // tempetura tem uma faixa de 30 a 45 graus celsius
	//O valor de leitura do ADC é dado por ADC = Vin*1024/Vref, com Vref = 1,1V -> ADC=Vin*930.9090909
	
}

