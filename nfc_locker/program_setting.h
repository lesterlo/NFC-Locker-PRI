#pragma once 


enum State{LOCKER_CLOSED, LOCKER_OPENED};


//Program Constant
#define OCTOPUS_READ_INTERVAL 1000



//Hardware Pin Define
#define BUZZER_PIN 9 //Use D9 PWM pin on Mega

#define ENCODER_S1_PIN 2 //Define you encoder connection pin Here
#define ENCODER_S2_PIN 3
#define ENCODER_KEY_PIN 5

#define DOOR_LOCK_PIN 10

#define DOOR_SWITCH_PIN 47

