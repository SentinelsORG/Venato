#include <DFRobot_SIM808.h>
DFRobot_SIM808 sim808(&Serial);

//define the phone number
#define PHONE_NUMBER  "0779675524"
#define message
#define date
#define time 
#define location
#define speed
#define heading


void setup() {
  Serial.begin(9600);
  //******** Initialize sim808 module *************
  while(!sim808.init()) { 
      delay(1000);
      Serial.print("Sim808 init error\r\n");
  }
  Serial.println("Sim808 init success");

  //************* Turn on the GPS power************
  if( sim808.attachGPS())
      Serial.println("Open the GPS power success");
  else 
      Serial.println("Open the GPS power failure");
  

}

void loop() {
   //************** Get GPS data *******************
   if (sim808.getGPS()) {
     date= sim808.GPSdata.year + "/" +sim808.GPSdata.month + "/" + sim808.GPSdata.date + "/";
     time = sim808.GPSdata.hour + ":" +sim808.GPSdata.minute + ":" +sim808.GPSdata.second + "/n" ;
     location = "latitude :" + sim808.GPSdata.lat + "/n" + "longitude :" + sim808.GPSdata.lon ;
     speed =  "speed_kph :" + sim808.GPSdata.speed_kph ;
     hedaing = "heading :" + sim808.GPSdata.heading;
     message = date + time + location + speed + hedaing;
     sim808.sendSMS((char *)PHONE_NUMBER, (char *)message);

    //************* Turn off the GPS power ************
    sim808.detachGPS();

  }

}