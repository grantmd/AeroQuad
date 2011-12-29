/*
  AeroQuad v2.5 Beta 1 - July 2011
  www.AeroQuad.com
  Copyright (c) 2011 Ted Carancho.  All rights reserved.
  An Open Source Arduino based multicopter.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

// UAVTalk.pde is responsible for handling communication with ground control over the UAVTalk protocol

// Based on code for the OpenPilot project: http://wiki.openpilot.org/display/Doc/UAVTalk

int32_t WriteMsg(uint8_t *data, int32_t length) {
  for (int i=0; i<length; i++){
   SERIAL_PRINT(data[i]);
 }
 
 return 0;
}
 
void ObjEventCallback(UAVObjEvent* ev) {
 uint32_t id = UAVObjGetID(ev->obj);
 switch(ev->event) {
 case EV_UNPACKED:
   Serial.print("Unpacked event for ");
   Serial.println(id, HEX);
   break;
 case EV_UPDATED:
 case EV_UPDATED_MANUAL:
   Serial.print("Updated event for ");
   Serial.println(id, HEX);
   UAVTalkSendObject(ev->obj, ev->instId, 1, 100);
   break;
 case EV_UPDATE_REQ:
   Serial.print("Update request event for ");
   Serial.println(id, HEX);
   break;
 }
}
 
void UAVTalkSetup() {
 //Serial.println("In UAVTalkSetup()");
 // Initialize UAVTalk
 UAVTalkInitialize(WriteMsg);
 UAVObjInitialize();
 
 // Initialize the UAVTalk objects that you need.
 GCSTelemetryStatsInitialize();
 GCSTelemetryStatsConnectCallback(ObjEventCallback);
}

void UAVTalkProcess(){
  if (SERIAL_AVAILABLE()) {
    digitalWrite(LEDPIN, LOW);
    int in = SERIAL_READ();
    //Serial.println(in, HEX);
    UAVTalkProcessInputStream(in);
    digitalWrite(LEDPIN, HIGH);
  }
}

