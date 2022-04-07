/*
  by David Rios
  NYU IMA - Designing Interfaces for Live Performance 2022
  Code for DILP MIDI/ Arduino Class 2022

  Code based off Tom Igoe's Midi USB Examples
  https://tigoe.github.io/SoundExamples/midiusb.html
*/

#include <frequencyToNote.h>
#include <MIDIUSB.h>
#include <MIDIUSB_Defs.h>
#include <pitchToFrequency.h>
#include <pitchToNote.h>


// button Variables:
int b1 = 2;
int b2 = 3;
int b3 = 4;

// state change variables
int pb1 = 1;
int pb2 = 1;
int pb3 = 1;

int note = 60;
int led = 5;

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(b1, INPUT_PULLUP);
  pinMode(b2, INPUT_PULLUP);
  pinMode(b3, INPUT_PULLUP);
  digitalWrite(led, LOW);
}

void loop() {
  /* map a potentiometer reading to a midi value
     Full MIDI Range is 0-127
  */
  midiEventPacket_t rx;

  rx = MidiUSB.read();
  int hd = rx.header;

  int prog = rx.byte1;
  int nte = rx.byte2;
  int vel = rx.byte3;

  // 144 note ON, channel 1
  // 144 - 159 note ON, channel 1-16

  // 145 = channel2 on
  // 129 = channel2 off
  if (hd != 0) {
    Serial.print("Received: ");
    Serial.print(hd);
    Serial.print("  ");
    Serial.print(prog);
    Serial.print("  ");
    Serial.print(nte);
    Serial.print("  ");
    Serial.println(vel);



    if (prog == 144) {
      int bright = map(vel, 0, 127, 0, 255);
      analogWrite(led, bright); 
    }
    
    if (prog == 128) {
      digitalWrite(led, LOW);
    }
  }


  if (digitalRead(b1) == 0 && pb1 == 1) {
    noteOn(0, note, 80);
    pb1 = 0;
  }
  if (digitalRead(b1) == 1 && pb1 == 0) {
    noteOff(0, note, 0);
    pb1 = 1;
  }
  if (digitalRead(b2) == 0 && pb2 == 1) {
    noteOn(0, note + 3, 80);
    pb2 = 0;
  }
  if (digitalRead(b2) == 1 && pb2 == 0) {
    noteOff(0, note + 3, 80);
    pb2 = 1;
  }

  if (digitalRead(b3) == 0 && pb3 == 1) {
    noteOn(0, note + 7, 80);
    pb3 = 0;
  }
  if (digitalRead(b3) == 1 && pb3 == 0) {
    noteOff(0, note + 7, 0);
    pb3 = 1;
  }
}

// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
  MidiUSB.flush();
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
  MidiUSB.flush();
}


// Control Change
// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();
}