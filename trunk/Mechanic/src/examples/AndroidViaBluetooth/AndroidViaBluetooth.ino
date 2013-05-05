/*********************************************************************
 * Mechanic - On-board diagnosis using Arduino and CAN
 *
 * Copyright (C) 2013 Joerg Pleumann
 * 
 * This example is free software; you can redistribute it and/or
 * modify it under the terms of the Creative Commons Zero License,
 * version 1.0, as published by the Creative Commons Organisation.
 * This effectively puts the file into the public domain.
 *
 * This example is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * LICENSE file for more details.
 */

#include <Mechanic.h>
#include <SoftwareSerial.h>

ObdInterface obd;
ObdMessage msg;

SoftwareSerial blue(9, 8);

boolean hasSpeed = false, hasRpm = false, hasLoad = false, hasTemp = false, hasFuel = false; 

float speed = 0, rpm = 0, load = 0, temp = 0, fuel = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  blue.begin(9600);

  obd.setSlow(false);
  obd.setExtended(false);
  obd.setDebug(false);
  obd.begin();
  
  obd.isPidSupported(0x0d, hasSpeed);
  obd.isPidSupported(0x0c, hasRpm);
  obd.isPidSupported(0x04, hasLoad);
  obd.isPidSupported(0x05, hasTemp);
  obd.isPidSupported(0x2f, hasFuel);
}

void loop() {
  if (hasSpeed) {
    obd.getPidAsFloat(0x0d, 0.0f, 255.0f, speed);
  }
  
  if (hasRpm) {
    obd.getPidAsFloat(0x0c, 0.0f, 16383.75f, rpm);
  }
  
  if (hasLoad) {
    obd.getPidAsFloat(0x04, 0.0f, 100.0f, load);
  }
  
  if (hasTemp) {
    obd.getPidAsFloat(0x05, -40.0f, 215.0f, temp);
  }
  
  if (hasFuel) {
	obd.getPidAsFloat(0x2F, 0.0f, 100.0f, fuel);
  }

  blue.print(obd.isSlow() ? "true," : "false,");
  blue.print(obd.isExtended() ? "true," : "false,");
  blue.print(speed);
  blue.print(",");
  blue.print(rpm);
  blue.print(",");
  blue.print(load);
  blue.print(",");
  blue.print(temp);
  blue.print(",");
  blue.print(fuel);
  blue.write(13);
  blue.write(10);
  
  delay(100);
}
