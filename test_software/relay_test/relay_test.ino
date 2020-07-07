/**
 * relay test.ino
 * 
 * Test your relay module
 * 
 * Program tested on Arduino Mega 2560
 * 
 * Modified from https://www.arduino.cc/en/Tutorial/ToneMelody?from=Tutorial.Tone 
 * 
 */

#define RELAY_PIN 10
#define LED_PIN LED_BUILTIN

void toogle_relay(int state)
{
    digitalWrite(RELAY_PIN, state);

    digitalWrite(LED_PIN, state);
}

void setup()
{
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, HIGH); //Disable the RELAY_PIN

    pinMode(LED_PIN, OUTPUT);
}

void loop()
{
    toogle_relay(LOW);
    delay(2000);
    toogle_relay(HIGH);
    delay(2000);
}
