/*
 * File:   main.c
 * Author: diego
 *
 * Created on 15 de agosto de 2021, 06:36 PM
 */


//------------------------------- LIBRERIAS ------------------------------------
#define _XTAL_FREQ 4000000
#include <xc.h>
#include <stdint.h>
#include <stdio.h>  // Para usar printf
#include <string.h> // Concatenar
#include <stdlib.h>


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


//--------------------------------- Variables ----------------------------------
char valor, centenas, residuo, decenas, unidades;
char cen, dec, uni;
int  count, select;
char var, con;
int  full;

//-------------------------------- Prototipos ----------------------------------
void setup(void);
void Text(void);
char division (char valor);

//------------------------------ Interrupciones --------------------------------
void __interrupt() isr(void)
{
     // Interrupcion del Puerto B
    if (RBIF == 1)                              // Verificar bandera de la interrupcion del puerto b
    {
        if (PORTBbits.RB0 == 0)                 // Si oprimo el boton 1
        {
            count = count + 1;  
        }
        if  (PORTBbits.RB1 == 0)                // Se oprimo el boton 2
        {
            count = count - 1;  
        }
        INTCONbits.RBIF = 0;                    // Se limpia la bandera de la interrupcion
    }
}

//----------------------------------- Main -------------------------------------
void main(void) {
    
    setup();                                    // Configuración principal

    while(1)  
    {
        PORTA = count;
        PORTD = select;
        Text();
    }
    return;
}

//--------------------------------- Funciones ----------------------------------
// Funcion de stdio.h
void putch(char data){      
    while(TXIF == 0);
    TXREG = data; 
    return;
}   

// Configuración principal
void setup(void){
    // Puertos digitales
    ANSEL = 0;
    ANSELH = 0;
    
    // Inputs y outputs de los puertos
    TRISA = 0;
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    TRISD = 0;
        
    // Se limpian los puertos
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
    
    // Configuración reloj interno
    OSCCONbits.IRCF0 = 0;                       // 4mhz
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.SCS = 1;  
    
    // Configuracion de pull-up interno
    OPTION_REGbits.nRBPU = 0;
    WPUB = 0b00000011;
    IOCBbits.IOCB0 = 1;
    IOCBbits.IOCB1 = 1;

    // Configuraciones TX y RX
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    BAUDCTLbits.BRG16 = 0;
    
    SPBRG = 25;
    SPBRGH = 1;
    
    RCSTAbits.SPEN = 1;
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;
    TXSTAbits.TX9 = 0;                          // 8 bits
    
    TXSTAbits.TXEN = 1;
    
    PIR1bits.RCIF = 0;  
    PIR1bits.TXIF = 0;  
    
    // Configuración de interrupciones
    INTCONbits.GIE = 1;     
    INTCONbits.PEIE = 1;    
    PIE1bits.RCIE = 0;      
    PIE1bits.TXIE = 0;      
    INTCONbits.RBIF = 1;    
    INTCONbits.RBIE = 1;    

}

// Función para escribir en el UART
void Text(void){
     __delay_ms(250);                           //Tiempos para el despliegue de los caracteres
     division(count);
    printf("Valor del contador:\r");
    __delay_ms(250);
    TXREG = centenas;
    __delay_ms(250);
    TXREG = decenas;
    __delay_ms(250);
    TXREG = unidades;
    __delay_ms(250);
    printf("\r");
    
    
     printf("Ingresar numero para el contador: Rango(0-255)\r");
      defensa1:  
       while(RCIF == 0);
        cen = RCREG -48;  

       while(RCREG > '2'){ 
           goto defensa1;
       }
    
      defensa2:
        while(RCIF == 0); 
         dec = RCREG -48; 

        if(cen == 2){
           while(RCREG > '5'){
               goto defensa2;
           }
       }

      defensa3:
       while(RCIF == 0); 
        uni = RCREG - 48;

       if(cen == 2 && dec == 5){
           while(RCREG > '5'){
               goto defensa3;
           }
       }
      con = concat(cen, dec);
      full = concat(con, uni);
      __delay_ms(250);
    printf("El numero elegido es: %d \r", full);
    select = full;
}

// Función para despliegue de valores
char division (char valor){
    centenas = valor/100;
    residuo = valor%100; 
    decenas = residuo/10; 
    unidades = residuo%10; 
    // Se le suma 47 para que entregue el valor requerido
    centenas = centenas + 48;
    decenas = decenas + 48;
    unidades = unidades + 48;
} 

// Función para concatenar
int concat(int a, int b)
{
    char s1[20];
    char s2[20];
 
    // Convert both the integers to string
    sprintf(s1, "%d", a);
    sprintf(s2, "%d", b);
 
    // Concatenate both strings
    strcat(s1, s2);
 
    // Convert the concatenated string
    // to integer
    int c = atoi(s1);
 
    // return the formed integer
    return c;
}
