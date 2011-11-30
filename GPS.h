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

/**************************************************************/
/************************** GPS *******************************/
/**************************************************************/
// This is experimental, it is not yet functional

#ifdef UseGPS
class GPS {
private:
  bool has_fix;
  
  float lat, lon, course, altitude;
  unsigned long fix_age, last_update;
  
protected:
  TinyGPS gps;
  
  void update(void){
    gps.f_get_position(&lat, &lon, &fix_age);
    if (fix_age == TinyGPS::GPS_INVALID_AGE)
      has_fix = false;
    else if (fix_age > 5000)
      has_fix = false;
    else
      has_fix = true;
    
    course = gps.f_course();
    altitude = gps.f_altitude();
    
    last_update = millis();
  }
  
public: 
 
  GPS(void){ 
    has_fix = false;
  }

  // **********************************************************************
  // The following function calls must be defined inside any new subclasses
  // **********************************************************************
  const bool feedgps(void);
  
  // *********************************************************
  // The following functions are common between all subclasses
  // *********************************************************
  
  const float getLat(){ return lat; }
  const float getLon(){ return lon; }
  
  const float getCourse(){ return course; }
  const float getAltitude(){ return altitude; }
  
  const bool hasFix(){ return has_fix; }
  const unsigned long getFixAge(){ return fix_age + (millis() - last_update); }
  const unsigned long getLastUpdate(){ return last_update; }
};


class SerialGPS : public GPS { 
public:
  SerialGPS() : GPS(){
  }
  
  // Read serial data off of the gps, run it through tinygps
  // Update values and return true if we successfully decoded a sentence
  const bool feedgps(){
    
    while (Serial2.available()){
      char c = Serial2.read();
      if (gps.encode(c))
        update();
        return true;
    }
    
    return false;
  }
};

#endif


