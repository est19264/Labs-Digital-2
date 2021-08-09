/* 
 * File:   ADC.h
 * Author: diego
 *
 * Created on 19 de julio de 2021
 */

#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#pragma config FOSC = INTRC_NOCLKOUT
#include <xc.h> // include processor files - each processor file is guarded.  

// Prototipo
void ADC_config(char frec); 

#endif	/* XC_HEADER_TEMPLATE_H */