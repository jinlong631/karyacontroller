
#include "common.h"
#include "gcode.h"
#include "temp.h"
#include "timer.h"
#include<stdint.h>
extern void demo();
extern void motionloop();


int line_done, ack_waiting = 0;
void gcode_loop() {
  //float x=12.345;
  //xprintf(PSTR("Motion demo %d %f\n"),10,x);
  //delay(500);
  //demo();
#if defined(__AVR__) || defined(ESP8266)
  motionloop();
  if (ack_waiting) {
    zprintf(PSTR("ok\n"));
    ack_waiting = 0;
  }
  if (Serial.available() > 0)
  {
    line_done = gcode_parse_char(Serial.read());
    if (line_done) {
      ack_waiting = line_done - 1;
    }
  }
#else
#endif

}

// generic sdcard add about 800byte ram and 8kb code
/*
#include <SPI.h>
#include <SD.h>

File myFile;
void demoSD() {
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}
*/
void setup() {
  // put your setup code here, to run once:
  //  Serial.setDebugOutput(true);
  Serial.begin(115200);
  initmotion();
  init_gcode();
  init_temp();
  zprintf(PSTR("start\nok\n"));
  zprintf(PSTR("Motion demo\nok\n"));
  //demoSD();
}

void loop() {
  //demo();
  gcode_loop();
  /*  if (feedthedog()){
      float f=123.456;
      int32_t i=1234;
      xprintf("%f",ff(f),i);
    }
  */
}
