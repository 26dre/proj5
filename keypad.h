/*
 * keypad.h
 *
 * Created: 5/13/2024 7:08:03 AM
 *  Author: Administrator
 */ 
#ifndef KEYPAD
#define KEYPAD
#include "utility.h"
#include "avr.h"
// * for reset
// D for confirm




/************************************************************************/
/* get the integer representation of the key                                                                     */
/************************************************************************/
int get_key();

int is_pressed(int r, int c);

char get_char_wait();

char get_char_no_wait();
#endif