/**
 * door_test.ino
 * 
 * Test the door with Timer control
 * 
 * Press the encoder button to open the door
 * 
 * !! According to the spec of YF591S lock, the the unlock power 
 * should apply around 100ms, DON'T apply more than 200ms. !!
 * 
 * Program tested on Arduino Mega 2560
 * 
 */

#include <NSEncoder.h>
#include <TimerOne.h>
#include <TimerThree.h>

#define RELAY_PIN 10
#define LED_PIN LED_BUILTIN

#define ENCODER_S1_PIN 2 //Define you encoder connection pin Here
#define ENCODER_S2_PIN 3
#define ENCODER_KEY_PIN 5


NSEncoder_btn enc(ENCODER_S1_PIN, ENCODER_S2_PIN, ENCODER_KEY_PIN, 4, false);


//isr
void isr_encoder()
{
  enc.btn_task();
}

void isr_doorM()
{
    digitalWrite(RELAY_PIN, LOW); //deactive the Power signal, turn it off
    Timer3.stop(); //Stop the timer itself
}

void open_door()
{
    Timer3.attachInterrupt(isr_doorM); 
    digitalWrite(RELAY_PIN, HIGH);//turn the power signal ON, Open the door
    Timer3.initialize(1000 * 1000); //start timer, 100ms clock (100*1000)   
}

void setup()
{
    Serial.begin(115200);

    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW); //Disable the RELAY_PIN

//Setup Timer1 for Encoder
    Timer1.initialize(enc.getBtnDebounceInterval()*1000);
    Timer1.attachInterrupt(isr_encoder); //Set 30 ms clock

}

void loop()
{
    NSEncoder_btn::BTN_STATE enc_btn;
    enc_btn = enc.get_Button();
    switch(enc_btn)
    {
        case NSEncoder_btn::OPEN:
            //Do nothing
        break;
        
        case NSEncoder_btn::PRESSED:
            Serial.print("Pressed");
            open_door();
        break;

        case NSEncoder_btn::RELEASED:
            //Do nothing
        break;
        
        case NSEncoder_btn::HELD:
            //Do nothing
        break;
        
        case NSEncoder_btn::DOUBLE_PRESSED:
            //Do nothing
        break;

    }
}
