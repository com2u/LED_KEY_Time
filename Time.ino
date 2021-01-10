#include <NTPtimeESP.h> // https://github.com/SensorsIot/NTPtimeESP

NTPtime NTPch("ch.pool.ntp.org");   // Choose server pool as required
strDateTime dateTime;

unsigned long now;
unsigned long lastCycle;
unsigned long lastUpdate=0;
int refreshTime = 100000;  // when to request the thime form server [ms]

void initTime(){
  lastUpdate = 99999999999;
}

void getTime(){
 if (millis()-lastUpdate>refreshTime) {
    dateTime = NTPch.getNTPtime(1.0, 1);
    //timeValide = 0;
    if(dateTime.valid){
      lastCycle = millis();
      lastUpdate = lastCycle;
      NTPch.printDateTime(dateTime);
      actualHour = dateTime.hour;
      actualMinute = dateTime.minute;
      actualsecond = dateTime.second;
      actualyear = dateTime.year;
      actualMonth = dateTime.month;
      actualday =dateTime.day;
      actualdayofWeek = dateTime.dayofWeek;
    } else {
      //Serial.println("Time NOT received"); 
    }
 } else {
    now = millis();
    int distance = ((now-lastCycle)/1000);
    lastCycle = lastCycle +(distance*1000);
    actualsecond = actualsecond + distance;
 }
 
  if (actualsecond > 59) {
    actualsecond = 0;
    actualMinute++;
  }
  if (actualMinute > 59) {
    actualMinute = 0;
    actualHour++;
  }
  if (actualHour > 59) {
    actualHour = 0;
    actualday++;
  }
  return;
}
