// Include Libraries
#include "Arduino.h"
#include "BTHC05.h"

// Global variables and defines
// Pin Definitions
#define BTHC05_PIN_TXD    5
#define BTHC05_PIN_RXD    6

// object initialization
BTHC05 bthc05(BTHC05_PIN_RXD,BTHC05_PIN_TXD);


void setup()
{
    // Setup Serial which is useful for debugging
    // Use the Serial Monitor to view printed messages
    Serial.begin(9600);
    while (!Serial) ; // wait for serial port to connect. Needed for native USB
    Serial.println("start");
    
    bthc05.begin(9600);
    //This example uses HC-05 Bluetooth to communicate with an Android device.
    //Download bluetooth terminal from google play store, https://play.google.com/store/apps/details?id=Qwerty.BluetoothTerminal&hl=en
    //Pair and connect to 'HC-05', the default password for connection is '1234'.
    //You should see this message from your arduino on your android device
    bthc05.println("Bluetooth On....");
}

void loop()
{
 
  while (!bthc05.available());
    //Serial.print('.');
    Serial.println("ok");

    // HC - 05 Bluetooth Serial Module - Test Code
    String bthc05Str = "";
    //Receive String from bluetooth device
    if (bthc05.available())
    {
    //Read a complete line from bluetooth terminal
    bthc05Str = bthc05.readStringUntil('\n');
    // Print raw data to serial monitor
    Serial.print("\nBT Raw Data: ");
    Serial.println(bthc05Str);
    }
    //Send sensor data to Bluetooth device  
    bthc05.println("PUT YOUR SENSOR DATA HERE");

  delay(5000);
}