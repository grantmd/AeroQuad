#ifdef UAVTALK

int32_t WriteMsg(uint8_t *data, int32_t length) {
 SERIAL_PRINT(data, length);
}
 
void ObjEventCallback(UAVObjEvent* ev) {
 uint32_t id = UAVObjGetID(ev->obj);
 switch(ev->event) {
 case EV_UNPACKED:
   ns.print("Unpacked event for ");
   ns.println(id, HEX);
   break;
 case EV_UPDATED:
 case EV_UPDATED_MANUAL:
   ns.print("Updated event for ");
   ns.println(id, HEX);
   UAVTalkSendObject(ev->obj, ev->instId, 1, 100);
   break;
 case EV_UPDATE_REQ:
   ns.print("Update request event for ");
   ns.println(id, HEX);
   break;
 }
}
 
void UAVTalkSetup() { 
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
    UAVTalkProcessInputStream(SERIAL_READ());
    digitalWrite(LEDPIN, HIGH);
  }
}

#endif
