
#include <stdlib.h>
#include "utility.h"

#include "keypad.h"
#include <string.h>
#include <stdio.h>
#include "lcd.h"
#include "password.h"

char FORCE_UNLOCK_PSWD[] = "ABCD\0";

enum PSWD_LOCK_ENUM PSWD_LOCK = UNLOCKED;
char CURR_KEY = '\0';
char TIMES_PASSWORD_FAILED = 0;
char curr_password[PASSWORD_MAX_LENGTH + 1] = "\0";
char curr_one_time[PASSWORD_MAX_LENGTH + 1] = "\0";
enum LOGGED_IN_STATE LOG_STATE = LOGGED_OUT;
const char local_chars[] = {'1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', '*', '0', 'D'};
void toggle_log_state()
{
    if (LOGGED_OUT == LOG_STATE)
    {
        LOG_STATE = LOGGED_IN;
    }
    else
    {
        LOG_STATE = LOGGED_OUT;
    }
}

void set_curr_key(char c)
{
    CURR_KEY = c;
}
char read_curr_key()
{
    char tmp = CURR_KEY;
    CURR_KEY = '\0';
    return tmp;
}

void get_state_as_char(char *buf)
{
    if (LOGGED_OUT == LOG_STATE)
    {
        buf = "Logged Out";
    }
    else
    {
        buf = "Logged Out";
    }
}

void generate_one_time_password(char length, char *pswd)
{
    // char* ret_str = (char*) malloc (sizeof(char)*length);

    lcd_clr();
    lcd_pos(0, 0);
    lcd_puts2("One time password gen");
    lcd_pos(1, 0);
    wait_avr(1000);

    black_out_password(pswd);
    for (int i = 0; i < length; i++)
    {
        int tmp_idx = rand() % KEYBOARD_MAX_STATES;
        pswd[i] = local_chars[tmp_idx];
    }

    char buf[17];
    sprintf(buf, "New pswd: %s", pswd);
    lcd_puts2(buf);
    wait_avr(2000);
    // return ret_str;
}

void read_from_keypad_wait()
{
    char c = get_char_wait();
    // lcd_clr();

    set_curr_key(c);
}

// this function assumes that the password to set can handle the max length
void set_password(char *password_to_set)
{
    lcd_clr();
    lcd_pos(0, 0);
    lcd_puts2("Set password: ");
    lcd_pos(1, 0);
    wait_avr(1000);
    black_out_password_length(password_to_set, PASSWORD_MAX_LENGTH + 1);
    while (strlen(password_to_set) == 0)
    {

        for (int i = 0; i < PASSWORD_MAX_LENGTH; i++)
        {
            read_from_keypad_wait();
            char tmp = read_curr_key();
            if (ENTER_KEY == tmp)
            {
                break;
            }
            else
            {
                password_to_set[i] = tmp;
                lcd_pos(1, 0);
                lcd_puts2(password_to_set);
            }
        }
    }

    wait_avr(1000);
}

void black_out_password(char *password_to_blackout)
{
    for (int i = 0; password_to_blackout[i] != '\0'; i++)
    {
        password_to_blackout[i] = '\0';
    }
}

void black_out_password_length(char *pswd_to_blackout, char length)
{
    for (int i = 0; i < length; i++)
    {
        pswd_to_blackout[i] = '\0';
    }
}

void inc_pswd_fail()
{
    TIMES_PASSWORD_FAILED++;
}

void reset_pswd_fail()
{
    TIMES_PASSWORD_FAILED = 0;
}

void get_user_login_attempt(char *buf)
{
    lcd_clr();
    lcd_pos(0, 0);
    lcd_puts2("Regular login: ");
    lcd_pos(1, 0);
    wait_avr(1000);
    buf[0] = 0;
    while (strlen(buf) == 0)
    {

        for (int i = 0; i < PASSWORD_MAX_LENGTH; i++)
        {
            read_from_keypad_wait();
            char tmp = read_curr_key();
            if (ENTER_KEY == tmp)
            {
                break;
            }
            else
            {
                buf[i] = tmp;
                lcd_pos(1, 0);
                lcd_puts2(buf);
            }
        }
    }

    wait_avr(1000);
}

void handle_logged_in()
{
    char last_in = '\0';
    // lcd_clr();
    // lcd_pos(0,0);
    // char buf[17];
    // get_state_as_char(buf);
    char buf[17];
    sprintf(buf, "%c = logout", LOG_OUT_KEY);
    lcd_clr();
    lcd_pos(0, 0);
    lcd_puts2("Logged in");
    wait_avr(1000);
    while (LOG_OUT_KEY != last_in)
    {
        lcd_clr();
        lcd_pos(0, 0);
        char buf1[17];
        char buf2[17];
        sprintf(buf1, "%c = change pwd", CHANGE_PASSWORD_KEY);
        sprintf(buf2, "%c = 1 time", GENERATE_ONE_TIME);
        lcd_puts2(buf1);
        lcd_pos(1, 0);
        lcd_puts2(buf2);

        read_from_keypad_wait();
        last_in = read_curr_key();

        switch (last_in)
        {
        case CHANGE_PASSWORD_KEY:
            set_password(curr_password);
            break;
        case GENERATE_ONE_TIME:
            generate_one_time_password(GENERIC_ONE_TIME_LENGTH, curr_one_time);
            break;
        default:
            break;
        }
    }

    toggle_log_state();
}

void toggle_locked_out()
{
    if (UNLOCKED == PSWD_LOCK)
    {
        PSWD_LOCK = LOCKED;
    }
    else
    {
        PSWD_LOCK = UNLOCKED;
    }
}

void handle_logged_out()
{

    char login_succesful = 0;

    while (!login_succesful)
    {
        // lcd_puts2("Press ")
        lcd_clr();
        char buf1[15];
        sprintf(buf1, "%c = log in", LOG_IN_KEY);
        char buf2[15];
        sprintf(buf2, "%c = manager", FORCE_UNLOCK_START);
        lcd_pos(0, 0);
        lcd_puts2(buf1);
        lcd_pos(1, 0);
        lcd_puts2(buf2);

        char last_in = '\0';
        read_from_keypad_wait();
        last_in = read_curr_key();
        switch (last_in)
        {
        case LOG_IN_KEY:
        {

            login_succesful = handle_regular_login();

            if (MAX_CONSECUTIVE_FAILED == TIMES_PASSWORD_FAILED)
            {
                toggle_locked_out();
            }

            break;
        }
        case FORCE_UNLOCK_START:
        {
            login_succesful = handle_override_login();
        }
        default:
            break;
        }

        if (MAX_CONSECUTIVE_FAILED == TIMES_PASSWORD_FAILED)
        {
            login_succesful = handle_locked_out();
        }
    }

    toggle_log_state();
}
void main_sm()
{

    while (1)
    {
        if (LOGGED_IN == LOG_STATE)
        {
            handle_logged_in();
        }
        else if (LOGGED_OUT == LOG_STATE)
        {
            handle_logged_out();
        }
    }
}

void full_sm()
{
    initial_setup();
    main_sm();
}

// void print_password(char* pswd, int r, int c) {

// }

void initial_setup()
{
    lcd_init();
    set_password(curr_password);
    PSWD_LOCK = UNLOCKED;

    TIMES_PASSWORD_FAILED = 0;
    LOG_STATE = LOGGED_IN;
    srand(time(NULL));
}

char handle_override_login()
{
    lcd_clr();
    lcd_pos(0, 0);
    lcd_puts2("Manager Login");

    char login_succesful = 0;
    char override_attempt[100] = "\0";
    get_user_override_attempt(override_attempt);

    char override_res = strcmp(override_attempt, FORCE_UNLOCK_PSWD) == 0 ? 1 : 0;
    lcd_pos(1, 0);
    if (override_res)
    {
        login_succesful = 1;
        reset_pswd_fail();
        toggle_locked_out();
        lcd_puts2("Login succesful");
    }
    else
    {
        lcd_puts2("Login unsuccesful");
    }

    return login_succesful;
}

char handle_regular_login()
{
    lcd_clr();
    lcd_pos(0, 0);
    lcd_puts2("Regular Login");
    char login_succesful = 0;
    char login_attempt[100] = "\0";
    get_user_login_attempt(login_attempt);
    char res_reg = strcmp(login_attempt, curr_password) == 0 ? 1 : 0;

    char res_one_time = 0;
    // curr one time is not set
    if (curr_one_time[0] != '\0')
    {
        res_one_time = strcmp(login_attempt, curr_one_time) == 0 ? 1 : 0;
    }

    if (res_reg == 1)
    {
        login_succesful = 1;
        reset_pswd_fail();
        lcd_clr();
        lcd_pos(0, 0);
        lcd_puts2("Login Succesful");
    }
    else if (res_one_time == 1)
    {
        login_succesful = 1;

        reset_pswd_fail();
        lcd_clr();
        lcd_pos(0, 0);
        lcd_puts2("Login Succesful");
        black_out_password_length(curr_one_time, PASSWORD_MAX_LENGTH + 1);
    }
    else
    {
        lcd_clr();
        lcd_pos(0, 0);
        lcd_puts2("Login failed");
        lcd_pos(1, 0);
        char buf[20];
        inc_pswd_fail();
        sprintf(buf, "Failed Cnt: %d", TIMES_PASSWORD_FAILED);
        lcd_puts2(buf);

        wait_avr(2000);
    }

    return login_succesful;
}

char handle_locked_out()
{
    lcd_clr();
    lcd_pos(0, 0);
    lcd_puts2("Locked out");
    lcd_pos(1, 0);
    for (int i = 60; i > 0; i--)
    {

        char c = get_char_no_wait();

        if (c == FORCE_UNLOCK_START && handle_override_login())
        {
            return 1;
        }
        // note this will keep the timer going even after an attempt for a manager log in
        // that is a user will still be locked out for the full time regardless

        lcd_pos(1, 0);

        char time_left[10];
        sprintf(time_left, "Left: %02d", i);
        lcd_puts2(time_left);
        wait_avr(995);
    }

    reset_pswd_fail();
    toggle_locked_out();
    return 0;
}
void get_user_override_attempt(char *buf)
{
    lcd_clr();
    lcd_pos(0, 0);
    lcd_puts2("Manager login: ");
    lcd_pos(1, 0);
    wait_avr(1000);
    buf[0] = 0;
    while (strlen(buf) == 0)
    {

        for (int i = 0; i < PASSWORD_MAX_LENGTH; i++)
        {
            read_from_keypad_wait();
            char tmp = read_curr_key();
            if (ENTER_KEY == tmp)
            {
                break;
            }
            else
            {
                buf[i] = tmp;
                lcd_pos(1, 0);
                lcd_puts2(buf);
            }
        }
    }

    wait_avr(1000);
}
