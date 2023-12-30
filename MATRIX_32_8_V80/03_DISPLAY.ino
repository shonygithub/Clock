
uint16_t GetTime(uint8_t *p_nowSecund , uint8_t *p_nowHour , uint8_t *p_nowMinute ) { // Get time in a single number
 tmElements_t Now;
 RTC.read(Now);
 *p_nowHour=Now.Hour;
 *p_nowMinute=Now.Minute;
 (*p_nowSecund) = Now.Second; // вказівнику на змінну nowSecund призначаємо значення секунд витягнуте з  модуля DS3231
 return (*p_nowHour*100+*p_nowMinute); //наприклад - поточний час 11:35, 11 множимо на 100 = 1100 плюс 35 , отже  Now = 1135.
}

uint16_t GetDate() {
 tmElements_t Now;
 RTC.read(Now);
 if (!flagGreeting) {         
  if (lastDay != Now.Day) {
   flagGreeting = true;
   lastDay = Now.Day; 
  }
 }
 return (Now.Day*100+Now.Month);
}

void ShowDisplay() {
 static uint8_t lastDigitSecund;
 uint8_t nowSecund ;
 uint16_t  now = GetTime(&nowSecund ,&nowHour ,&nowMinute); 
 if (nowSecund != lastDigitSecund) {   // якщо значення секунд змінилось
  if (photoresistorValue >= (brightnessMin+nightModeThreshold)) {    // поки не темно в кімнаті
   if (!nightMode) {nightMode = true;} 
   if (flagGreeting) { 
    if ((nowHour >= 7) && (nowHour <= 23)) {
     if (digitalRead (MOTION_SENSOR_PIN) == HIGH ) {
      mp3_set_volume (volumeGreeting);
      mp3_play (PlayTableGreetings[random(15)]);  // відтворюємо привітання 
      flagGreeting = false; 
     }   
    } 
   }
   if (nowSecund == 0) {               // якщо секунди дорівнюють нулю 
    AlarmClockFonction();
   dot = !dot; 
    if (! dawnIsWorking)  { 
    if (! onceAlarm) {PlayHours(); }   // якщо не відтворюється трек будильника вимовляєм годину 
     FadeFonction();                    // виконуєм анімацію 
     ledColor = ColorTable[random(24)]; // змінюєм яскраві кольори 24 (відтінків) для відображення часу
     ShowFonction (now);                // виконуєм ,,рухомий дисплей,,
     return;
    }
   }
  }
  else {       // інакше коли вже темно,світимо синім
   if (nowSecund == 0) {AlarmClockFonction();} // на початку кожної хвилини перевіряєм чи можна будити
   if (!dawnIsWorking) {  // поки не активізувалась функція ,,світанку,,
    nightMode = false;
    znachennya=1000;
    ledColor = CRGB::Blue;
    if (digitalRead (MOTION_SENSOR_PIN) == HIGH ) {  //  сенсором зафіксовано рух, значить працюєм як ,,нічник,,
     LEDS.setBrightness(brightness);
     flagNightLight = true ;
     cantorNightLightTime = 0 ;
     }
    NightLight();    
    if (flagNightLight == false) {LEDS.setBrightness(nightBrightness);}  // якщо вже протермінований ,,нічник,,
   }
   else {ledColor = CRGB::Yellow;}          // жовті цифри на червоному фоні при активізації функції ,,світанок,,
  } 
  if ((nowSecund==10)||(nowSecund==11)||(nowSecund==30)||(nowSecund==31)||(nowSecund==50)||(nowSecund==51)) {
   if (tempFlag == false) {
    LedsFon(0,NUM_LEDS); 
    if (!dawnIsWorking) {TempToArray(celsius);}
    else {TimeToArray(nowSecund, now);}
    FastLED.show();
    tempFlag = true;
   }                  
  }
  else {tempFlag = false;} 
  if((nowSecund==20)||(nowSecund==21)||(nowSecund==40)||(nowSecund==41)) {
   if (dateFlag == false) {
    LedsFon(0,NUM_LEDS); 
    if (!dawnIsWorking) {DateToArray();} // якщо не ,,світанок,, показуємо дату
    else {TimeToArray(nowSecund, now);}
    FastLED.show();
    dateFlag = true;
    sensors.requestTemperatures();
    celsius = ( sensors.getTempCByIndex(0)* 10 + temp_corr) ;
   } 
  }
  else {dateFlag = false;}
  if ((tempFlag == false)&&(dateFlag == false)) { // якщо не пора показувати температуру або дату
   LedsFon(0,NUM_LEDS); 
   TimeToArray(nowSecund, now);
  }
  lastDigitSecund = nowSecund; // щоб тільки один раз на секунду
  GetTimeFromServer();  // перевіримо чи не настав час для синхронізації по інтернету
  if (onceAlarm) {                                            // якщо під час відтворення треку з будильником
   if (digitalRead (MOTION_SENSOR_PIN) == HIGH) {mp3_stop();} // сенсор HC-SR505 зафіксував рух - вимикаєм музику 
  } 
 }
}

void TimeToArray(uint8_t dSecund, uint16_t  now) {
 if (!dawnIsWorking) {
  bias = 0;
  if (dSecund % 2==0) {                       // моргаємо розділовими точками
   dot = true; 
  }
  else {
   dot = false;  
  }
   for(uint8_t i=1; i<=2; i++) {
   uint8_t digitSecund = dSecund % 10; 
   if (i==1) {PairOfdigits(224, digitSecund);}  // друкуєм значення секунд нижнього розряду
   else {Pairdigits(184, digitSecund);}       // друкуєм значення секунд вищого розряду
   dSecund /= 10;
  }
 }
 else {bias = 48;}
 for(uint8_t i=1; i<=4; i++) {
  uint16_t digit = now % 10;              //наприклад - поточний час 11:35, отже початкове значення Now=1135. 
  if (i==1) {Pairdigits (136, digit);}     // друкуєм значення хвилин нижнього розряду (остача від Now=1135 ділення на 10 digit=5)
  else if (i==2) {PairOfdigits(96, digit);}// друкуєм значення хвилин вищого розряду (остача від Now=113 ділення на 10 digit=3)
  else if (i==3) {Pairdigits(40, digit);}   // друкуєм значення годин нижнього розряду (остача від Now=11 ділення на 10 digit=1)
  else {PairOfdigits(0, digit);}            // друкуєм значення годин вищого розряду (остача від Now=1 ділення на 10 digit=1)
  now /= 10; //спочатку 1135 ділимо на 10 отримуєм Now=113;потім 113/10 Now=11;після 11/10 Now=1.
 }
}

void TempToArray( uint16_t celsius) {  
 PairOfdigits(224, 11);
 Pairdigits(184, 10);
 for(uint8_t i=1; i<4; i++){
  uint16_t ldigit = celsius % 10; // get last digit in time
  if (i==1) {PairOfdigits(144, ldigit);}
  else if (i==2) {Pairdigits(88, ldigit);}
  else {PairOfdigits(48, ldigit);}
  celsius /= 10; 
 }
 PairOfdigits(8, 10);   // друкуємо значок плюс
 leds [3] = ledColor;   // дописуємо крапку для повного відображення знака 
 leds [135] = ledColor;
}

void DateToArray() { 
 int now = GetDate();
 leds [115] = ledColor;
 leds [124] = ledColor;
 leds [131] = ledColor;
 leds [140] = ledColor;
 for (uint8_t i=1; i<=4; i++) {
  uint16_t digit = now % 10; 
  if (i==1) {PairOfdigits(192, digit);}
  else if (i==2) {Pairdigits(152, digit);}
  else if (i==3) {Pairdigits(72, digit);}
  else {PairOfdigits(32, digit);}
  now /= 10;
 }
}

void blink_Leds(uint16_t first, uint16_t last, long color) { // вибрані пікселі засвічуємо  певним коліром, або ж гасимо
 for(uint16_t index=first; index<last; index++) {leds [index]=color;}
}

void LedsFon(uint16_t first, uint16_t last) { // вибрані пікселі засвічуємо  певним коліром, або ж гасимо
 for(uint16_t index=first; index<last; index++) {leds [index]=dawnColor;}
}

void runningFire () {
 if (!dawnIsWorking) {
  if ((tempFlag == false)&&(dateFlag == false)) {
   if ((cantor == false)&&(dot == false)) {  // пока счётчик настроен на сложение
    if  (arrays  <=  87)  {  // пока не добежали огнем до верху
     arrays ++;  // перескакиваем на один пиксель выше
     blink_Leds(arrays-4, arrays, ledColor);
     leds [arrays-5]= CHSV(0, 0, 0);
     FastLED.show();
    }
    else {
     arrays =  83 ;
     cantor = true;
    } 
   }
   else if ((cantor == true)&&(dot == true)) {  // счётчик на вычитании
     if (arrays >=  81) {  // пока не добежали к низу
     arrays -- ;
     blink_Leds(arrays, arrays+4, ledColor);
     leds [arrays+4]= CHSV(0, 0, 0);
     FastLED.show();
    } 
    else {
     arrays =  84;
     cantor = false;  
    }
   }
  }
 }
}
