/*
 * File:   main_slave2.c
 * Author: diego
 *
 * Created on 9 de agosto de 2021, 07:43 PM
 */

//------------------------------- LIBRERIAS ------------------------------------
#define _XTAL_FREQ 4000000
#include <xc.h>
#include <stdint.h>
#include <stdio.h>  // Para usar printf

// Librerias propias
#include "I2C.h"

//-------------------------- Bits de configuraciÓn -----------------------------
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT        // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF                   // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF                  // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF                  // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF                     // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF                    // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF                  // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF                   // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF                  // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF                    // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V               // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF                    // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h>
#include <stdint.h>

//------------------------------ Variables ------------------------------------- 
uint8_t z;

//----------------------------- Prototipos ------------------------------------- 
void setup(void);                           // Defino las funciones antes de crearlas

//--------------------------- Interrupciones -----------------------------------
void __interrupt() isr(void)
{
    if (RBIF == 1)  // Verificar bandera de la interrupcion del puerto b
    {
        if (PORTBbits.RB0 == 0)             // Si oprimo el boton 1
        {
            PORTA = (PORTA + 1)&0x0F;              
        }
        if  (PORTBbits.RB1 == 0)            // Se oprimo el boton 2
        {
            PORTA = (PORTA - 1)&0x0F;              
        }
        INTCONbits.RBIF = 0;                // Se limpia la bandera de la interrupcion
    }
    
    if(PIR1bits.SSPIF == 1){ 

        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                         // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;               // Clear the overflow flag
            SSPCONbits.WCOL = 0;                // Clear the collision bit
            SSPCONbits.CKP = 1;                 // Enables SCL (Clock)
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            z = SSPBUF;                         // Lectura del SSBUF para limpiar el buffer y la bandera BF
            PIR1bits.SSPIF = 0;                 // Limpia bandera de interrupción recepción/transmisión SSP
            SSPCONbits.CKP = 1;                 // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);             // Espera a que la recepción se complete
            PORTA = SSPBUF;                      // Guardar en el PORTD el valor del buffer de recepción
            __delay_us(250);
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            SSPBUF = PORTA;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    }
}
//-------------------------------- MAIN ----------------------------------------
void main(void) {
    
    setup();                                // Se llama a la sub-rutina de configuracion general
    
    
    while(1)                                
    {

    }
}

//----------------------------- SUB-RUTINAS ------------------------------------
// Función de configuraciones generales
void setup(void){
// Configuración oscilador interno
    OSCCONbits.IRCF2 = 0;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 0;                   // Se configura a 250kHz
    OSCCONbits.SCS = 1;

// Configuración de puertos digitales
    ANSEL = 0x00;
    ANSELH = 0x00;
    
// Configuración inputs y outputs
    TRISA = 0x00;
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    TRISC = 0x00;
    TRISD = 0x00;
    TRISE = 0X00;
    
// Se limpian los puertos
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00; 
    
// Configuracion de pull-up interno
    OPTION_REGbits.nRBPU = 0;
    WPUB = 0b00000011;
    IOCBbits.IOCB0 = 1;
    IOCBbits.IOCB1 = 1;
    
// Configuación interrupciones
    INTCONbits.GIE = 1;
    INTCONbits.RBIF = 1;
    INTCONbits.RBIE = 1;
    
// I2C configuracion esclavo
    I2C_Slave_Init(0x60);   
}
