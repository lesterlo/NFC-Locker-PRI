/**
 * Buzzer Test.ino
 * 
 * Test your buzzer
 * 
 * Program tested on Arduino Mega 2560
 * 
 * Modified from https://www.arduino.cc/en/Tutorial/ToneMelody?from=Tutorial.Tone 
 * 
 */
#include "pitches.h"


#define BUZZER_PIN 11 //Use D11 PWM pin on Mega
#define LED_PIN LED_BUILTIN

// notes in the melody:
const int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
const int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

void play_melody()
{
    digitalWrite(LED_PIN, HIGH);
    for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(BUZZER_PIN, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(BUZZER_PIN);
  }
  digitalWrite(LED_PIN, LOW);
}

void setup() {
  pinMode(LED_PIN, OUTPUT); //Initial the LED pin
}

void loop() {
  play_melody(); //Play the melody 
  delay(2000); //Wait 2 second to play the melody again
}