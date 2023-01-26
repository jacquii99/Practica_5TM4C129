/*
 * ADC.c
 *
 *  Created on: 04/11/2019
 *      Author: ravenelco
 */
#include "lib/include.h"

extern void Configura_Reg_ADC0(void)
{
    /*
    Habilitar el modulo 0 del ADC con dos canales analogicos 
    en el puerto E a una velocidad de conversion de 250ksps
    dandole la mayor prioridad al secuenciador 2 con evento
    de procesador 
    */
     //Pag 396 para inicializar el modulo de reloj del adc RCGCADC
    SYSCTL->RCGCADC = (1<<0); 
    //Pag 382 (RGCGPIO) Puertos base habilitación del reloj
    //                    
    SYSCTL->RCGCGPIO |= (0<<0)|(1<<1)|(0<<2)|(1<<3)|(1<<4)|(0<<5)|(0<<6)|(0<<7)|(0<<8)|(0<<9)|(0<<11)|(0<<12);//se habilitan E4 D3 Y B1  
    //Pag 760 (GPIODIR) Habilta los pines como I/O un cero para entrada y un uno para salida
    GPIOB_AHB->DIR = (0<<5) ; //habilita B5
    GPIOE_AHB->DIR = (0<<1) |(0<<4); //E1 y E4
    GPIOD_AHB->DIR = (0<<4) | (0<<5) | (0<<7); //D4 D5 Y D7 
    //(GPIOAFSEL) pag.770 Enable alternate función para que el modulo analógico tenga control de esos pines
    GPIOB_AHB->AFSEL = (1<<5):
    GPIOE_AHB->AFSEL = (1<<1) | (1<<4);
    GPIOD_AHB->AFSEL = (1<<4) | (1<<5) |(1<<7);
    //(GPIODEN) pag.781 desabilita el modo digital
    GPIOB_AHB->DEN = (0<<5);
    GPIOE_AHB->DEN = (0<<1) | (0<<4);
    GPIOD_AHB->DEN = (0<<4) | (0<<5) | (0<<7);
    //Pag 787 GPIOPCTL registro combinado con el GPIOAFSEL y la tabla pag 1808
    GPIOB_AHB->PCTL = GPIOB_AHB->PCTL & (0xFFF0FFFF);
    GPIOE_AHB->PCTL = GPIOE_AHB->PCTL & (0XFFFF0FF0); 
    GPIOD_AHB->PCTL = GPIOD_AHB->PCTL & (0XF0F00FFF);
    //(GPIOAMSEL) pag.786 habilitar analogico
    GPIOB_AHB->AMSEL = (1<<5);
    GPIOE_AHB->AMSEL = (1<<1) | (1<<4); 
    GPIOD_AHB->AMSEL = (1<<4) | (1<<5) | (1<<7); 
    //Pag 1159 El registro (ADCPC) establece la velocidad de conversión por segundo
    ADC0->PC = 0X7;//250ksps
    //Pag 1099 Este registro (ADCSSPRI) configura la prioridad de los secuenciadores
    ADC0->SSPRI = 0x0231;
    //Pag 1077 (ADCACTSS) Este registro controla la activación de los secuenciadores
    ADC0->ACTSS  = (0<<2) | (0<<3) | (0<<1);
    //Pag 1091 Este registro (ADCEMUX) selecciona el evento que activa la conversión (trigger)
    ADC0->EMUX  = (0x0000);//
    //Pag 1129 Este registro (ADCSSMUX2) define las entradas analógicas con el canal y secuenciador seleccionado
    ADC0->SSMUX2 = 0x0089;//
    //pag 868 Este registro (ADCSSCTL2), configura el bit de control de muestreo y la interrupción
    ADC0->SSCTL2 = (1<<6) | (1<<5) ;
    /* Enable ADC Interrupt */
    ADC0->IM |= (1<<2); /* Unmask ADC0 sequence 2 interrupt pag 1082*/
    //NVIC_PRI4_R = (NVIC_PRI4_R & 0xFFFFFF00) | 0x00000020;
    //NVIC_EN0_R = 0x00010000;
    //Pag 1077 (ADCACTSS) Este registro controla la activación de los secuenciadores
    ADC0->ACTSS = (0<<3) | (1<<2) | (0<<1) | (0<<0);
    ADC0->PSSI |= (1<<2);
}
extern void ADC0_InSeq2(uint16_t *Result,uint16_t *duty){

    //ADC Processor Sample Sequence Initiate (ADCPSSI)
       ADC0->PSSI = 0x00000004;
       while((ADC0->RIS&0x04)==0){}; // espera al convertidor
       Result[1] = ADC0->SSFIFO2&0xFFF; //  Leer  el resultado almacenado en la pila2
       Result[0] = ADC0->SSFIFO2&0xFFF;
       duty[0] = (Result[0]*20000)/4096;
       duty[1] = (Result[1]*20000)/4096;
       ADC0->ISC = 0x0004;  //Conversion finalizada

}
extern void Configurar_GPIO


