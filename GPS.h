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
public: 
  TinyGPS gps;
 
  GPS(void){ 
    // this is the constructor of the object and must have the same name 
    // can be used to initialize any of the variables declared above 
  }

  // **********************************************************************
  // The following function calls must be defined inside any new subclasses
  // **********************************************************************
  void initialize(void); 
  void gpsdump();
  bool feedgps();
  
  // *********************************************************
  // The following functions are common between all subclasses
  // *********************************************************
  void printFloat(double number, int digits)
  {
    // Handle negative numbers
    if (number < 0.0)
    {
       SERIAL_PRINT('-');
       number = -number;
    }
  
    // Round correctly so that print(1.999, 2) prints as "2.00"
    double rounding = 0.5;
    for (uint8_t i=0; i<digits; ++i)
      rounding /= 10.0;
    
    number += rounding;
  
    // Extract the integer part of the number and print it
    unsigned long int_part = (unsigned long)number;
    double remainder = number - (double)int_part;
    SERIAL_PRINT(int_part);
  
    // Print the decimal point, but only if there are digits beyond
    if (digits > 0)
      SERIAL_PRINT("."); 
  
    // Extract digits from the remainder one at a time
    while (digits-- > 0)
    {
      remainder *= 10.0;
      int toPrint = int(remainder);
      SERIAL_PRINT(toPrint);
      remainder -= toPrint; 
    } 
  }
};


class SerialGPS : public GPS { 
public:
  SerialGPS() : GPS(){
  }
  
  void gpsdump()
  {
    long lat, lon;
    float flat, flon;
    unsigned long age, date, time, chars;
    int year;
    byte month, day, hour, minute, second, hundredths;
    unsigned short sentences, failed;
  
    gps.get_position(&lat, &lon, &age);
    SERIAL_PRINT("Lat/Long(10^-5 deg): "); SERIAL_PRINT(lat); SERIAL_PRINT(", "); SERIAL_PRINT(lon); 
    SERIAL_PRINT(" Fix age: "); SERIAL_PRINT(age); SERIAL_PRINTLN("ms.");
    
    feedgps(); // If we don't feed the gps during this long routine, we may drop characters and get checksum errors
  
    gps.f_get_position(&flat, &flon, &age);
    SERIAL_PRINT("Lat/Long(float): "); printFloat(flat, 5); SERIAL_PRINT(", "); printFloat(flon, 5);
    SERIAL_PRINT(" Fix age: "); SERIAL_PRINT(age); SERIAL_PRINTLN("ms.");
  
    feedgps();
  
    gps.get_datetime(&date, &time, &age);
    SERIAL_PRINT("Date(ddmmyy): "); SERIAL_PRINT(date); SERIAL_PRINT(" Time(hhmmsscc): "); SERIAL_PRINT(time);
    SERIAL_PRINT(" Fix age: "); SERIAL_PRINT(age); SERIAL_PRINTLN("ms.");
  
    feedgps();
  
    gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
    SERIAL_PRINT("Date: "); SERIAL_PRINT(static_cast<int>(month)); SERIAL_PRINT("/"); SERIAL_PRINT(static_cast<int>(day)); SERIAL_PRINT("/"); SERIAL_PRINT(year);
    SERIAL_PRINT("  Time: "); SERIAL_PRINT(static_cast<int>(hour)); SERIAL_PRINT(":"); SERIAL_PRINT(static_cast<int>(minute)); SERIAL_PRINT(":"); SERIAL_PRINT(static_cast<int>(second)); SERIAL_PRINT("."); SERIAL_PRINT(static_cast<int>(hundredths));
    SERIAL_PRINT("  Fix age: ");  SERIAL_PRINT(age); SERIAL_PRINTLN("ms.");
    
    feedgps();
  
    SERIAL_PRINT("Alt(cm): "); SERIAL_PRINT(gps.altitude()); SERIAL_PRINT(" Course(10^-2 deg): "); SERIAL_PRINT(gps.course()); SERIAL_PRINT(" Speed(10^-2 knots): "); SERIAL_PRINTLN(gps.speed());
    SERIAL_PRINT("Alt(float): "); printFloat(gps.f_altitude(), 2); SERIAL_PRINT(" Course(float): "); printFloat(gps.f_course(), 2); SERIAL_PRINTLN();
    SERIAL_PRINT("Speed(knots): "); printFloat(gps.f_speed_knots(), 2); SERIAL_PRINT(" (mph): ");  printFloat(gps.f_speed_mph(), 2);
    SERIAL_PRINT(" (mps): "); printFloat(gps.f_speed_mps(), 2); SERIAL_PRINT(" (kmph): "); printFloat(gps.f_speed_kmph(), 2); SERIAL_PRINTLN();
  
    feedgps();
  
    gps.stats(&chars, &sentences, &failed);
    SERIAL_PRINT("Stats: characters: "); SERIAL_PRINT(chars); SERIAL_PRINT(" sentences: "); SERIAL_PRINT(sentences); SERIAL_PRINT(" failed checksum: "); SERIAL_PRINTLN(failed);
  }
  
  bool feedgps()
  {
    while (Serial2.available())
    {
      if (gps.encode(Serial2.read()))
        return true;
    }
    return false;
  }
};

#endif


