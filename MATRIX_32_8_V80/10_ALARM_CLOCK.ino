static byte counter;

void AlarmClockFonction() {
 if (alarmWorkingDays) {                                                   // якщо дозволено будильник
  if (!alarmAndWeekend) {                                                  // якщо не дозволенно будити у вихідні
   DateTime now = rtc.now();
   uint8_t ndayOfTheWeek = now.dayOfTheWeek();
   if ((ndayOfTheWeek == 0)||(ndayOfTheWeek == 6)) {return;}               // виходимо з функції якщо субота або неділя
  }
  uint16_t valueClock = (nowHour*60+nowMinute);
  uint16_t valueAlarmClock = (hourAlarmClock*60+minuteAlarmClock);
  if (dawnMode) {                                                          // якщо дозволено світанок
   if (! dawnIsWorking) {                                                  // і поки ще не розпочалась процедура світанку
    if ((valueClock >= (valueAlarmClock - aheadAlarm)) && (valueClock < valueAlarmClock )) {
  // ящо час на годиннику знаходиться у таких рамках ,після подачі живлення на девайс (відбулось непередбачене знеживлення)
     dawnIsWorking = true;  // час на годиннику вже більший від часу початку світанку, тому розпочато процедуру світанку
     LedsFon(0,NUM_LEDS);   // гасимо всі пікселі
     timeDawn = (aheadAlarm + prolongationDawn) *60000/255; // встановлення інтервалів збільшення яскравості світанку
     mp3_set_volume (setVolumeDawn); //встановіть у веб інтерфейсі гучність звуку світанку від 1 до 30(за замовчуванням буде 3)
     mp3_play (trackNumberDawn+220);  //викликаємо функцію програвання треку світанку записаних на SDкарту
    }
   }
   else {
    if (valueClock>=(valueAlarmClock+prolongationDawn)) {  //  завершився світанок
     dawnIsWorking = false;     //  процедуру світанку скасовано
     counter = 0;               //  обнулюємо лічильник
     dawnColor = CHSV(0, 0, 0); //  основному фону надаємо чорний колір
    }
   } 
  }
  if (! onceAlarm) {                                               // поки ще не увімкнута процедура будильника
   if ((valueClock >= valueAlarmClock) && (valueClock < valueAlarmClock + prolongationDawn)) {         
  // ящо час на годиннику знаходиться у таких рамках ,після подачі живлення на девайс (відбулось непередбачене знеживлення)                                          
     onceAlarm = true;                                              // значить розпочато процедуру будильника
     mp3_set_volume (setVolumeAlarm); //встановіть  гучність звуку будильника від 1 до 30(за замовчуванням буде 10)
     mp3_play (trackNumberAlarm + 200 + counterTrack);     //викликаємо функцію програвання треку будильника записаних на SDкарту 
     counterTrack ++;
     if (counterTrack == 15) {counterTrack = 0;} 
   } 
  }
  else {
   if (valueClock >= valueAlarmClock + prolongationDawn) { // якщо час на відтворення треку з будильником вичерпано
    onceAlarm = false;
    mp3_stop();
   }
  }
 }
}

void BrightnessDawnFonction() {
 if (counter < 255) {
  counter ++;
  dawnColor = CHSV(0, 255 - counter, 255);                            // повільно колір фону з червоного змінюєм на білий
  LEDS.setBrightness(map(counter,0,255,brightnessMin,brightnessMax)); // повільно збільшуєм яскравість
 }
}
