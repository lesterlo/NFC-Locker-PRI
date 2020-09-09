#ifndef PROGRAM_SETTING_H_
#define PROGRAM_SETTING_H_


enum State{LOCKER_CLOSED, LOCKER_OPENED};


//Program Constant
#define LOOP_DELAY_INTERVAL 1000



//Hardware Pin Define
#define BUZZER_PIN 9 //Use D9 PWM pin on Mega

#define ENCODER_S1_PIN 2 //Define you encoder connection pin Here
#define ENCODER_S2_PIN 3
#define ENCODER_KEY_PIN 5

#define DOOR_LOCK_PIN 10

#define DOOR_SWITCH_PIN 47

#endif