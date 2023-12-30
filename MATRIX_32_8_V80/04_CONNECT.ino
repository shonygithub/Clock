void GetTimeFromServer() {  
 if(nowHour == setHour) {           // якщо година підходить
  if (nowMinute == setMinute) {     // і якщо хвилини зійдуться 
   if (once) {                      // і поки корекція часу ще не відбулась, відбудеться синхронізація
    disconnect = false;
    ConnectWifi(); 
    if (WiFi.status() == WL_CONNECTED) {
     if (GetNTP() ) {CorectTime();} // якщо відбувся звязок із сервером часу викликаєм функцію корекції часу
    }
   }
  }
  else { //якщо час вже не дорівнює (setHour:setMinute)
   once = true;
    disconnect = true;
  }
 }
}

void ConnectWifi() {  //Соединение с WiFi
 if (WiFi.status() != WL_CONNECTED) {   
  byte len_ssid = i2c_eeprom_read_byte (EEPROM_I2C_ADDRESS,96); //читаєм з регістра кількість знаків назви WI-FI сітки вашого роутера
  byte len_pass = i2c_eeprom_read_byte (EEPROM_I2C_ADDRESS,97); //читаєм з регістра кількість знаків пароля від WI-FI сітки вашого роутера
  if (len_pass > 32) {len_pass = 0;} // якщо кількість знаків пароля перевищує можливе значення, тоді обнулюєм змінну len_pass
   if ((len_ssid < 33) && (len_ssid != 0)) {//якщо кількості знаків пароля  і знаків паспорту допустима, 
    //складаєм масив із даних прочитаних з EEPROM
    unsigned char* buf_ssid = new unsigned char[len_ssid];   // виділяємо в оперативці память для динамічного масиву
    unsigned char* buf_pass = new unsigned char[len_pass];
    for (byte i=0; i<len_ssid; i++) {                                 // допоки не дійшли до певного значення прочитаного з 96 комірки ЕЕПРОМу
     buf_ssid[i] = char(i2c_eeprom_read_byte (EEPROM_I2C_ADDRESS,i)); // побайтово пкладаємо в масив символи прочитані з ЕЕПРОМу
     delay(1);
    }
    buf_ssid[len_ssid] = '\x0'; // присвоюємо символ завершення для масиву
    char *ssid  = (char*)buf_ssid;
    for (byte i=0; i<len_pass; i++) {
     buf_pass[i] = char(i2c_eeprom_read_byte (EEPROM_I2C_ADDRESS,i + 32));
     delay(1);
    }
    buf_pass[len_pass] = '\x0';
    char *pass  = (char*)buf_pass;  
    WiFi.begin(ssid, pass); //конектимось із роутером з підставленими char *ssid ,і char *pass.
    WiFi.hostname(APssid);
    MDNS.begin(APssid);
    MDNS.addService("http", "tcp", 80);
    delete[] buf_pass ;
    buf_pass = nullptr ;
    delete[] buf_ssid ;
    buf_ssid = nullptr ; 

  }
 } 
}

void CorectTime() { //корегування модуля реального часу DS 3231 відносно отриманих значень з інтернету
 uint8_t se = (ntpTime )%60;//призначаєм значення секунд отримані з інтернету (NTP сервер часу)
 uint8_t mi = (ntpTime/60 )%60;//призначаєм значення хвилин отримані з інтернету (NTP сервер часу)
 uint8_t ho = (ntpTime/3600 )%24;//призначаєм значення годин отримані з інтернету (NTP сервер часу)
 uint8_t da = (day(ntpTime));//призначаєм значення днів отримані з інтернету застосувуючи бібліотеку <TimeLib.h>
 uint8_t mo = (month(ntpTime));//призначаєм значення місяців отримані з інтернету застосувуючи бібліотеку <TimeLib.h>
 uint8_t we = (weekday(ntpTime));//призначаєм значення днів тижня (неділя = 1,понеділок = 2, ... субота = 7.)
 uint16_t ye = ((year(ntpTime))-48);//призначаєм значення року отриманого з інтернету застосувуючи бібліотеку <TimeLib.h>
 SetTimeDateAndDate(se, mi, ho, we, da, mo, ye); // записуєм у модуль DS3231
 delay(100);
 once = false;
 disconnect = true;  
}

unsigned long getEpoch () {
  WiFi.hostByName(ntpServerName, timeServerIP);  
  sendNTPpacket(timeServerIP);   
  delay(1000);  
  int cb = udp.parsePacket();
   if (!cb) {return false;}
  else {   
   udp.read(packetBuffer, 48); // Читаем пакет в буфер 
   unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
   unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
   unsigned long secsSince1900 = highWord << 16 | lowWord; // Конвертируем два слова в переменную long
   const unsigned long seventyYears = 2208988800UL;        // Конвертируем в UNIX-таймстамп (число секунд от 01.01.1970
   return secsSince1900 - seventyYears;
 }
}

bool GetNTP() { // Посылаем и парсим запрос к NTP серверу  
 unsigned long epochControl = getEpoch ();
 if (!epochControl) {return false;} 
 else {
  unsigned long epoch = getEpoch ();
  if ((epoch - epochControl)  == 1) {
   ntpTime = epoch + ((timezone + dst) *3600);            // Делаем поправку на местную тайм-зону і літній час 
   return true; 
  }
  else {return false;} 
 }
}

unsigned long sendNTPpacket(IPAddress& address) { //Посылаем запрос NTP серверу на заданный адрес
 memset(packetBuffer, 0, 48);// Очистка буфера в 0
 packetBuffer[0] = 0b11100011;   // LI, Version, Mode // Формируем строку зыпроса NTP сервера
 packetBuffer[1] = 0;     // Stratum, или тип часов
 packetBuffer[2] = 6;     // Интервал опроса
 packetBuffer[3] = 0xEC;  // Точность одноранговых часов
 packetBuffer[12]  = 49;  // 8 bytes of zero for Root Delay & Root Dispersion
 packetBuffer[13]  = 0x4E;
 packetBuffer[14]  = 49;
 packetBuffer[15]  = 52;
 udp.beginPacket(address, 123); // Посылаем запрос на NTP сервер (123 порт)
 udp.write(packetBuffer, 48);
 udp.endPacket();
}
