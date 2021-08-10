/*
 * File:   main_master.c
 * Author: diego
 *
 * Created on 9 de agosto de 2021, 01:43 PM
 */


//------------------------------- LIBRERIAS ------------------------------------
#define _XTAL_FREQ 4000000
#include <xc.h>
#include <stdint.h>
#include <stdio.h>  // Para usar printf
#include <string.h> // Concatenar
#include <stdlib.h>

// Librerias propias
#include "I2C.h"
#include "LCD.h"                            // Libreria para el LCD

//-------------------------- Bits de configuraciÓn -----------------------------
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT            // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF                       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF                      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF                      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF                         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF                        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF                      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF                       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF                      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF                        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V                   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF                        // Flash Program Memory Self Write Enable bits (Write protection off)

#define RS RE0
#define EN RE2
#define D0 RD0
#define D1 RD1
#define D2 RD2
#define D3 RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7
//--------------------------------- Variables ----------------------------------
int full;
char temp;
char vol;
char cont;
char val;
char val1;
char val2;

//-------------------------------- Prototipos ----------------------------------
void setup(void);

//------------------------------ Interrupciones --------------------------------
void __interrupt() isr(void)
{
     
}

//----------------------------------- Main -------------------------------------
void main(void) {
    
    setup();                                    // Llama a la configuracion principal
    Lcd_Init();                             // LCD esta apagada
    Lcd_Clear();                            // Se limpia la LCD
    char buffer[20];                        // Se guarda el voltaje en un string
//    char buffer1[20];
//    char buffer2[20];
    char val2;
    char val1;
    char val;                               // Valor que deseo almacenar en un str
    Lcd_Set_Cursor(1,1);                    // Primera linea en la posicion 1
    Lcd_Write_String("VOL:  TEMP:  CONT:");      // Se imprimen los indicadores de voltaje
    
    while(1)
    {
        vol = PORTB;
        cont = PORTA;
        val = vol*0.0196; 
        val1 = temp;
        val2 = cont;
        sprintf(buffer, "%d     %d      %d", val, val1, val2); 
        Lcd_Set_Cursor(2,2);                    // Segunda linea
        Lcd_Write_String(buffer);               // Mostrar el valor del string
        
        //Obtener informacion del primer slave
        I2C_Master_Start();
        I2C_Master_Write(0x51);                 // 51, se escribe el 1 para que lea en el puerto de leds
        PORTB = I2C_Master_Read(0);             
        I2C_Master_Stop();
        __delay_ms(200);
        
        //Obtener informacion del segundo slave
        I2C_Master_Start();
        I2C_Master_Write(0x61);                 // 51, se escribe el 1 para que lea en el puerto de leds
        PORTA = I2C_Master_Read(0);             
        I2C_Master_Stop();
        __delay_ms(200);
       //Obtener informacion del sensor de temperatura
        //Direccion 0x90 porque A0 A1 y A2 estan a tierra
        I2C_Master_Start();
        I2C_Master_Write(0x80);                 // Selecciona el sensor y se escribe read temperature
        I2C_Master_Write(0xF3);                 
        I2C_Master_Stop();
        __delay_ms(200);
        
        I2C_Master_Start();
        I2C_Master_Write(0x81);                 // Ahora lee
        temp = I2C_Master_Read(0);             // Read temperature
        I2C_Master_Stop();
        __delay_ms(200);

        
    }
}

//--------------------------------- Funciones ----------------------------------

void setup(void){
    
    ANSEL = 0;
    ANSELH = 0;
    TRISA = 0;
    TRISB = 0;
    PORTA = 0;
    PORTB = 0;
    TRISD = 0;
    TRISE = 0;
    
    //limpiar puertos
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
    
    //Configurar reloj interno
    OSCCONbits.IRCF0 = 0;                       // 8mhz
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.SCS = 1; 
    
    // I2C configuracion Maestro
    I2C_Master_Init(100000);                    // Inicia comuncación I2C
}