#ifndef PASSWORD_H
#define PASSWORD_H

// #include "keypad.c"

#define PASSWORD_MAX_LENGTH 10
#define KEYBOARD_MAX_STATES 15
#define GENERIC_ONE_TIME_LENGTH 4
#define LOG_OUT_KEY '*'
#define LOG_IN_KEY '*'
#define CHANGE_PASSWORD_KEY '#'
#define ENTER_KEY '#'
#define GENERATE_ONE_TIME '1'
#define FORCE_UNLOCK_START '#'
#define MAX_CONSECUTIVE_FAILED 3


enum PSWD_LOCK_ENUM {
    LOCKED, UNLOCKED
};


enum LOGGED_IN_STATE {
    LOGGED_OUT, LOGGED_IN
}; 


void handle_locked_out(); 

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
void initial_setup(); 
void full_sm(); 


#endif