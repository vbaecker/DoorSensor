/**
 *
 * DESCRIPTION
 *
 * Interrupt driven door / window sensor
 * Author: Patrick 'Anticimex' Fallberg
 * Adaptation: Vladimir Baecker
 * 
 * Connect one button or door/window reed switch between 
 * digitial I/O pin 3 (BUTTON_PIN below) and GND 
 * 
 * This example is designed to fit Arduino Nano/Pro Mini
 * 
 */


// Enable debug prints to serial monitor
#define MY_DEBUG 

// Enable and select radio type attached
#define MY_RADIO_NRF24


#define SKETCH_NAME "Door and Window Sensor"
#define SKETCH_MAJOR_VER "0"
#define SKETCH_MINOR_VER "1"

#define MY_NODE_ID 22
#define PRIMARY_CHILD_ID 1

#include <SPI.h>
#include <MySensors.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

#define PRIMARY_BUTTON_PIN 3   // Arduino Digital I/O pin for button/reed switch

#if (PRIMARY_BUTTON_PIN < 2 || PRIMARY_BUTTON_PIN > 3)
#error PRIMARY_BUTTON_PIN must be either 2 or 3 for interrupts to work
#endif

// Change to V_LIGHT if you use S_LIGHT in presentation below
MyMessage msg(PRIMARY_CHILD_ID, V_TRIPPED);

void setup()  
{  
  // Setup the buttons
  pinMode(PRIMARY_BUTTON_PIN, INPUT);

  // Activate internal pull-ups
  digitalWrite(PRIMARY_BUTTON_PIN, HIGH);

}

void presentation() {
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo(SKETCH_NAME, SKETCH_MAJOR_VER "." SKETCH_MINOR_VER);

  // Register binary input sensor to sensor_node (they will be created as child devices)
  // You can use S_DOOR, S_MOTION or S_LIGHT here depending on your usage. 
  // If S_LIGHT is used, remember to update variable type you send in. See "msg" above.
  present(PRIMARY_CHILD_ID, S_DOOR);  
}

// Loop will iterate on changes on the BUTTON_PINs
void loop() 
{
  uint8_t value;
  static uint8_t sentValue=2;

  // Short delay to allow buttons to properly settle
  sleep(5);
  
  value = digitalRead(PRIMARY_BUTTON_PIN);
  
  if (value != sentValue) {
     // Value has changed from last transmission, send the updated value
     send(msg.set(value==HIGH ? 1 : 0));
     sentValue = value;
  }

  // Sleep until something happens with the sensor
  sleep(PRIMARY_BUTTON_PIN-2, CHANGE, 0);
  //sleep(PRIMARY_BUTTON_PIN, CHANGE, 0);
} 
