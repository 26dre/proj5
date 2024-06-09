#ifndef PASSWORD_H
#define PASSWORD_H

#include <stdlib.h>
#include <utility.h>
// #include "keypad.c"

#define PASSWORD_MAX_LENGTH 10
#define KEYBOARD_MAX_STATES 16
#define GENERIC_ONE_TIME_LENGTH 4
#define LOG_OUT_KEY '*'
#define LOG_IN_KEY '*'
#define CHANGE_PASSWORD_KEY '#'
#define ENTER_KEY '#'
#define GENERATE_ONE_TIME '1'
#define FORCE_UNLOCK_START '#'
#define FORCE_UNLOCK_PSWD "ABCD1234\0"
#define MAX_CONSECUTIVE_FAILED 3


enum PSWD_LOCK_ENUM {
    LOCKED, UNLOCKED
};

enum PSWD_LOCK_ENUM PSWD_LOCK = UNLOCKED; 


char CURR_KEY = '\0';
char TIMES_PASSWORD_FAILED = 0; 

char curr_password[PASSWORD_MAX_LENGTH + 1] = "\0"; 
char curr_one_time[PASSWORD_MAX_LENGTH + 1] = "\0";

enum LOGGED_IN_STATE {
    LOGGED_OUT, LOGGED_IN
}; 

enum LOGGED_IN_STATE LOG_STATE = LOGGED_OUT;



void toggle_log_state(); 
void set_curr_key(char c); 
char read_curr_key(); 

void get_state_as_char(char* buf); 
void generate_one_time_password (char length, char* pswd); 

void read_from_keypad_wait(); 

void black_out_password(char* password_to_blackout); 
void black_out_password_length(char* pswd_to_blackout, char length);
void set_password(char* password_to_set);

void inc_pswd_fail(); 
void reset_pswd_fail(); 

void get_user_login_attempt(char* buf);

void main_sm(); 
char handle_override_login();
char handle_regular_login();  




#endif