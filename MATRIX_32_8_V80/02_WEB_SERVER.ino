static String tmp_arg_st;
static int16_t tmp_arg_int;

void getByte (byte i , char* str,  int8_t minimum, byte maximum, byte adr , int8_t &val , uint8_t subtractor) {
 if (server.argName(i) == str) {            // якщо ключ підходить  
  tmp_arg_st = server.arg(i);               // переписуєм з ,,бірки на ключі,який підходить,, в динамічну строку  tmp_arg_st
  if (tmp_arg_st.length() >= 1) {           // якщо записали хоч один символ
   tmp_arg_int = tmp_arg_st.toInt();        // символ конвертуєм в ціле число і присвоюєм  динамічній змінній tmp_arg_int 
   if ((tmp_arg_int >= minimum) && (tmp_arg_int <= maximum))  {// якщо число коректне , аж тоді переписуєм глобальну змінну
    val = tmp_arg_int;      
    i2c_eeprom_write_byte(EEPROM_I2C_ADDRESS, adr, val + subtractor); // також вписуєм в EEPROM ,щоб після випадкового...      
    delay (5);  // ...перезавантаження вже прочитати змінене значення  . Ну і паузу добавляєм ,щоб гарантовано відбувся запис у ЕЕПРОМ
   }
  }
 }
}
  
void SetTimeDateAndDate(    //встановлюєм час і дату отримані веб-сторінки : 192.168.4.1/set
                   byte second, // 0-59
                   byte minute, // 0-59
                   byte hour, // 1-23
                   byte dayOfWeek, // 1-7
                   byte dayOfMonth, // 1-28/29/30/31
                   byte month, // 1-12
                   byte year) // 0-99
                   
{ Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0);
  Wire.write(decToBcd(second)); // 0 to bit 7 starts the clock
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour)); // If you want 12 hour am/pm you need to set
  Wire.write(decToBcd(dayOfWeek));
  Wire.write(decToBcd(dayOfMonth));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));
  Wire.endTransmission();
}

void HandleClient() {
 DateTime now = rtc.now();
 String webpage;
 webpage =  "<html>";
 webpage += "<head><title>ESP8266 clock</title><meta charset='UTF-8' http-equiv='refresh' content='5' >";
 webpage += "<style>";
 webpage += "body {font-family: Verdana; Color: #00979d;}";
 webpage += "h2 {line-height: 30%;} ";
 webpage += "p {line-height: 60%;} ";
 webpage += "</style>";
 webpage += "</head>";
 webpage += "<body>";
 webpage += "<p><b>&nbsp;&nbsp;- ESP8266 clock -</b></p><br>";
 webpage += "<p>&nbsp;&nbsp;&nbsp; часовий пояс &nbsp;&nbsp;&nbsp;&nbsp;" + String(timezone)  + "  <br><br></p>";
 webpage += "<p>&nbsp;&nbsp;&nbsp; актуальний час &nbsp;&nbsp;&nbsp;"  + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()) + " <br><br></p>";
 webpage += "<p>&nbsp;&nbsp;&nbsp; дата &nbsp;&nbsp;&nbsp;" + String(now.day()) + "/" + String(now.month()) + " <br><br></p>";
 webpage += "<p>&nbsp;&nbsp;&nbsp; рік , день тижня :&nbsp;&nbsp;" + String(now.year()) + " &nbsp;&nbsp;" + daysOfTheWeek[now.dayOfTheWeek()]+ "  <br><br></p>";
 webpage += "<p>&nbsp;&nbsp;&nbsp; режим будильника у будні дні  &nbsp;&nbsp;&nbsp;" + String(alarmWorkingDays)  + "  <br><br></p>";
 webpage += "<p>&nbsp;&nbsp;&nbsp; також будити у вихідні  &nbsp;&nbsp;&nbsp;" + String(alarmAndWeekend)  + "  <br><br><br><br></p>";
 webpage += "<p>&nbsp;&nbsp; <a href='http://192.168.4.1/set_clock_end_sensors'>НАЛАШТУВАННЯ ГОДИННИКА І СЕНСОРІВ</a></p><br>";//для порожньої строки  <br>
 webpage += "<p>&nbsp;&nbsp; <a href='http://192.168.4.1/set_alarm_clock'>НАЛАШТУВАННЯ БУДИЛЬНИКА</a></p><br>";
 webpage += "<p>&nbsp;&nbsp; <a href='http://192.168.4.1/set_WI_FI'>НАЛАШТУВАННЯ WI FI</a></p><br>";
 webpage += "<p>&nbsp;&nbsp; <a href='http://192.168.4.1/write_congratulation'>ЗАПИСАТИ ПРИВІТАННЯ</a></p>";
 webpage += "</body>";
 webpage += "</html>";
 server.send(200, "text/html", webpage);
}

void SetAlarmClock() { 
 String webpage;//описуємо на вебсторінці ESP8266 clock setup
 webpage =  "<html>"; //відкриваючий тег
 webpage += "<head><title>ESP8266 clock setup </title><meta charset='UTF-8'>";
 webpage += "<style>"; //відкриваючий тег
 webpage += "body { font-family: Verdana; Color: #00979d;}"; 
 webpage += "</style>";//закриваючий тег
 webpage += "</head>"; //закриваючий тег  webpage += "<body>";  
 String IPaddress = "192.168.4.1";
 webpage += "<p><b>&nbsp;&nbsp;- alarm clock setup -</b></p><BR>";
 webpage += "<form action='http://" + IPaddress + "/set_alarm_clock' method='POST'>";  
 webpage += "<p>&nbsp;&nbsp;&nbsp;Встановлення хвилин спрацювання будильника </p>";  
 webpage += "&nbsp;&nbsp;&nbsp; хвилина активації будильника (0...59): <input type='text' name='input_MAC' placeholder='" + String(minuteAlarmClock) + "'><BR><BR>";
 webpage += "<p>&nbsp;&nbsp;&nbsp;Встановлення години спрацювання будильника </p>";
 webpage += "&nbsp;&nbsp;&nbsp;  година активації будильника (1...22) : <input type='text' name='input_HAC' placeholder='" + String(hourAlarmClock) + "'><BR><BR>";
 webpage += "<p>&nbsp;&nbsp;&nbsp;Встановлення режиму будильника </p>";  
 webpage += "&nbsp;&nbsp;&nbsp; будити дозволено у будні дні (0 або 1) : <input type='' name='input_AWD' placeholder='" + String(alarmWorkingDays) + "'><BR><BR>";
 webpage += "<p>&nbsp;&nbsp;&nbsp;Режим будильника для вихідних днів </p>";  
 webpage += "&nbsp;&nbsp;&nbsp; дозволено будити ще і у віхідні (0 або 1) : <input type='' name='input_AAW' placeholder='" + String(alarmAndWeekend) + "'><BR><BR>";
 webpage += "<p>&nbsp;&nbsp;&nbsp;Дозвіл режиму ,,світанок,, </p>"; 
 webpage += "&nbsp;&nbsp;&nbsp; не дозволено або дозволено (0 або 1): <input type='text' name='input_DaM' placeholder='" + String(dawnMode) + "'><BR><BR>";
 webpage += "<p>&nbsp;&nbsp;&nbsp; Світанок до активації будильника </p>";  
 webpage += "&nbsp;&nbsp;&nbsp;  хвилини випередження будильника  (5...20): <input type='text' name='input_AMA' placeholder='" + String(aheadAlarm) + "'><BR><BR>";
 webpage += "<p>&nbsp;&nbsp;&nbsp; Світанок після активації будильника  </p>";  
 webpage += "&nbsp;&nbsp;&nbsp;  хвилини продовження світанку (1...10): <input type='text' name='input_PrD' placeholder='" + String(prolongationDawn) + "'><BR><BR>";
 webpage += "<p>&nbsp;&nbsp;&nbsp; Вибираємо трек для будильника </p>";  
 webpage += "&nbsp;&nbsp;&nbsp;  номер треку будильника  (1...20): <input type='text' name='input_TNA' placeholder='" + String(trackNumberAlarm) + "'><BR><BR>";
 webpage += "<p>&nbsp;&nbsp;&nbsp; Вибираємо трек для світанку  </p>";  
 webpage += "&nbsp;&nbsp;&nbsp;  номер треку світанку (1...10): <input type='text' name='input_TND' placeholder='" + String(trackNumberDawn) + "'><BR><BR>"; 
 webpage += "<p>&nbsp;&nbsp;&nbsp; Гучність звуку будильника </p>";  
 webpage += "&nbsp;&nbsp;&nbsp;  встановлюєм гучність будильника  (1...30): <input type='text' name='input_SVA' placeholder='" + String(setVolumeAlarm) + "'><BR><BR>";
 webpage += "<p>&nbsp;&nbsp;&nbsp; Гучність звуку світанку  </p>";  
 webpage += "&nbsp;&nbsp;&nbsp;  встановлюєм гучність світанку (1...30): <input type='text' name='input_SVD' placeholder='" + String(setVolumeDawn) + "'><BR><BR>"; 


 webpage += "<p>&nbsp;&nbsp;&nbsp; Гучність привітання  </p>";  
 webpage += "&nbsp;&nbsp;&nbsp;  встановлюєм гучність вітання  (0...30): <input type='text' name='input_VGr' placeholder='" + String(volumeGreeting) + "'><BR><BR><BR><BR>";

 webpage += "&nbsp;&nbsp;&nbsp;&nbsp;<input type='submit' value='Enter'>";
 webpage += "</form>";
 webpage += "</body>";
 webpage += "</html>";
 server.send(200, "text/html", webpage); // Send a response to the client asking for input
 if (server.args() > 0 ) {                    // якщо отримана хочаб якась кількість аргументів
  for ( uint8_t i=0; i<server.args(); i++) {  // надамо нове значення змінним у яких збігається ключ ,який вибирається у циклі 
   getByte ( i , "input_MAC" , 0 , 59 , 231 , minuteAlarmClock ,0 ); 
   getByte ( i , "input_HAC" , 1 , 22 , 232 , hourAlarmClock ,0 ); 
   getByte ( i , "input_AWD" , 0 , 1 , 233 , alarmWorkingDays ,0 );
   getByte ( i , "input_AAW" , 0 , 1 , 234 , alarmAndWeekend ,0 );
   getByte ( i , "input_DaM" , 0 , 1 , 235 , dawnMode ,0 );
   getByte ( i , "input_AMA" , 3 , 20 , 236 , aheadAlarm ,0 );
   getByte ( i , "input_PrD" , 1 , 10 , 237 , prolongationDawn ,0 );
   getByte ( i , "input_TNA" , 1 , 20 , 238 , trackNumberAlarm ,0 );
   getByte ( i , "input_TND" , 1 , 10 , 239 , trackNumberDawn ,0 );
   getByte ( i , "input_SVA" , 1 , 30 , 240 , setVolumeAlarm ,0 );
   getByte ( i , "input_SVD" , 1 , 30 , 241 , setVolumeDawn ,0 );  
   getByte ( i , "input_VGr" , 1 , 30 , 242 , volumeGreeting ,0 );      
  }
 }
}

void SetTime() {
 DateTime now = rtc.now();
 int nhour =  now.hour();
 int shour = nhour;
 int nminute = now.minute();
 int sminute = nminute;
 int ndayOfTheWeek = now.dayOfTheWeek();
 int sdayOfTheWeek = ndayOfTheWeek;
 int nday = now.day();
 int sday = nday;
 int nmonth = now.month();
 int smonth = nmonth;
 int nyear = now.year();
 int syear = nyear - 48;  
 String webpage;//описуємо на вебсторінці ESP8266 clock setup
 webpage =  "<html>"; //відкриваючий тег
 webpage += "<head><title>ESP8266 clock setup </title><meta charset='UTF-8'>";
 webpage += "<style>"; //відкриваючий тег
 webpage += "body { font-family: Verdana; Color: #00979d;}"; 
 webpage += "</style>";//закриваючий тег
 webpage += "</head>"; //закриваючий тег  webpage += "<body>";  
 String IPaddress = "192.168.4.1";
 webpage += "<p><b>&nbsp;&nbsp;- clock end sensors setup -</b></p><BR>";
 webpage += "<form action='http://" + IPaddress + "/set_clock_end_sensors' method='POST'>";  
 webpage += "<p>&nbsp;&nbsp;&nbsp;Встановлення мінімальної яскравості</p>";  
 webpage += "&nbsp;&nbsp;&nbsp;  яскравість мінімальна (2...29): <input type='text' name='input_MIN' placeholder='" + String(brightnessMin) + "'><BR><BR>";
 webpage += "<p>&nbsp;&nbsp;&nbsp;Встановлення максимальної яскравості</p>";
 webpage += "&nbsp;&nbsp;&nbsp;  яскравість максимальна (30...70) : <input type='text' name='input_MAX' placeholder='" + String(brightnessMax) + "'><BR><BR>";
 webpage += "<p>&nbsp;&nbsp;&nbsp;Корекція темп.сенсора</p>";  
 webpage += "&nbsp;&nbsp;&nbsp;  меньше або більше (-10...+10) : <input type='' name='input_TeC' placeholder='" + String(temp_corr) + "'><BR><BR>";
 webpage += "<p>&nbsp;&nbsp;&nbsp;Встановлення тайм-зони</p>"; 
 webpage += "&nbsp;&nbsp;&nbsp; мій часовий пояс (-11...+11): <input type='text' name='input_TaZ' placeholder='" + String(timezone) + "'><BR><BR>";
 webpage += "<p>&nbsp;&nbsp;&nbsp; Хвилина конекту з сервером часу  </p>";  
 webpage += "&nbsp;&nbsp;&nbsp;  вибір хв. конекту (0...59): <input type='text' name='input_SEM' placeholder='" + String(setMinute) + "'><BR><BR>";
 webpage += "<p>&nbsp;&nbsp;&nbsp; Година конекту з сервером часу  </p>";  
 webpage += "&nbsp;&nbsp;&nbsp;  вибір год. конекту (3...23): <input type='text' name='input_SEH' placeholder='" + String(setHour) + "'><BR><BR>";
 webpage += "<p>&nbsp;&nbsp;&nbsp;Дозвіл на автоматичний перехід на літній час </p>";
 webpage += "&nbsp;&nbsp;&nbsp; не дозволено або дозволено (0 або 1): <input type='text' name='input_DST' placeholder='" + String(summerTime) + "'><BR><BR>";
 webpage += "<p>&nbsp;&nbsp;&nbsp; Одноразовий ручний перехід на літній час </p>";  
 webpage += "&nbsp;&nbsp;&nbsp; зима або літо (0 або 1): <input type='text' name='input_DsT' placeholder='" + String(dst) + "'><BR><BR>";
 webpage += "<p>&nbsp;&nbsp;&nbsp;Встановлення гучності </p>";  
 webpage += "&nbsp;&nbsp;&nbsp;  гучність сповіщень (1...30): <input type='text' name='input_VOL' placeholder='" + String(volume) + "'><BR><BR>"; 
 webpage += "<p>&nbsp;&nbsp;&nbsp;Встановлення порогу нічного режиму </p>";  
 webpage += "&nbsp;&nbsp;&nbsp;  поріг нічного режиму (1...10): <input type='text' name='input_NMT' placeholder='" + String(nightModeThreshold) + "'><BR><BR>"; 
 webpage += "<p>&nbsp;&nbsp;&nbsp;Встановлення яскравості нічного режиму </p>";  
 webpage += "&nbsp;&nbsp;&nbsp;  яскравість у нічному режимі (1...3): <input type='text' name='input_NbV' placeholder='" + String(nightBrightness) + "'><BR><BR>"; 
 webpage += "<p>&nbsp;&nbsp;&nbsp;Встановлення часу і дати </p>"; 
 webpage += "&nbsp;&nbsp;&nbsp; ГОДИНА: <input type='text' name='input_HH' placeholder='" + String(nhour) + "'><BR><BR>"; 
 webpage += "&nbsp;&nbsp;&nbsp; ХВИЛИНА: <input type='text' name='input_MI' placeholder='" + String(nminute) + "'><BR><BR>";
 webpage += "&nbsp;&nbsp;&nbsp; ДЕНЬ ТИЖНЯ: <input type='text' name='input_DW' placeholder='" + String(ndayOfTheWeek) + "'><BR><BR>";
 webpage += "&nbsp;&nbsp;&nbsp; ДЕНЬ МІСЯЦЯ: <input type='text' name='input_DM' placeholder='" + String(nday) + "'><BR><BR>";
 webpage += "&nbsp;&nbsp;&nbsp; МІСЯЦЬ: <input type='text' name='input_MO' placeholder='" + String(nmonth) + "'><BR><BR>";
 webpage += "&nbsp;&nbsp;&nbsp; РІК: <input type='text' name='input_YE' placeholder='" + String(nyear) + "'><BR><BR>";
 webpage += "&nbsp;&nbsp;&nbsp;&nbsp;<input type='submit' value='Enter'>";
 webpage += "</form>";
 webpage += "</body>";
 webpage += "</html>";
 server.send(200, "text/html", webpage); // Send a response to the client asking for input
 if (server.args() > 0 ) { // Arguments were received
  for ( uint8_t i=0; i<server.args(); i++) {
   getByte ( i , "input_MIN" , 1 , 30 , 201 , brightnessMin ,0 ); 
   getByte ( i , "input_MAX" , 30 , 70 , 202 , brightnessMax ,0 ); 
   getByte ( i , "input_TeC" , -10 , 10 , 203 , temp_corr ,100 );
   getByte ( i , "input_TaZ" , -11 , 11 , 204 , timezone ,100 );
   getByte ( i , "input_SEM" , 0 , 59 , 205 , setMinute ,0 );
   getByte ( i , "input_SEH" , 3 , 23 , 206 , setHour ,0 );
   getByte ( i , "input_DST" , 0 , 1 , 207 , summerTime ,0 );
   getByte ( i , "input_DsT" , 0 , 1 , 208 , dst ,0 );
   getByte ( i , "input_VOL" , 1 , 30 , 209 ,  volume ,0 );
   getByte ( i , "input_NMT" , 1 , 10 , 210 , nightModeThreshold ,0 );
   getByte ( i , "input_NbV" , 1 , 3 , 211 , nightBrightness ,0 );  
   if (server.argName(i) == "input_HH") { 
    tmp_arg_st = server.arg(i); 
    if (tmp_arg_st.length() >= 1) { 
     tmp_arg_int = tmp_arg_st.toInt();
     if (tmp_arg_int < 25) { 
      shour = tmp_arg_int ;
     }
    }
   }
   if (server.argName(i) == "input_MI") {   
    tmp_arg_st = server.arg(i); 
    if (tmp_arg_st.length() >= 1) { 
     tmp_arg_int = tmp_arg_st.toInt();
     if (tmp_arg_int < 60) {
      sminute = tmp_arg_int ;
     }
    }
   }
   if (server.argName(i) == "input_DW") {
    tmp_arg_st = server.arg(i); 
    if (tmp_arg_st.length() >= 1) { 
     tmp_arg_int = tmp_arg_st.toInt();
     if ((tmp_arg_int > 0) && (tmp_arg_int < 8)) {
      sdayOfTheWeek = tmp_arg_int ;
     }
    }
   }
   if (server.argName(i) == "input_DM") {
    tmp_arg_st = server.arg(i); 
    if (tmp_arg_st.length() >= 1) { 
     tmp_arg_int = tmp_arg_st.toInt();
     if ((tmp_arg_int > 0) && (tmp_arg_int < 32)) {
      sday = tmp_arg_int ;
     }
    }
   }
   if (server.argName(i) == "input_MO") {
    tmp_arg_st = server.arg(i); 
    if (tmp_arg_st.length() >= 1) { 
     tmp_arg_int = tmp_arg_st.toInt();
     if ((tmp_arg_int> 0) && (tmp_arg_int< 13)) {
      smonth = tmp_arg_int ;
     }
    }
   }
   if (server.argName(i) == "input_YE") {
    tmp_arg_st = server.arg(i); 
    if (tmp_arg_st.length() >= 1) { 
     tmp_arg_int = tmp_arg_st.toInt();
     if ((tmp_arg_int > 2000) && (tmp_arg_int < 2100)) {
      syear = tmp_arg_int - 48; //так і не зрозумів чому саме 48?...
     }
    }
   }
  }
  SetTimeDateAndDate(00, sminute, shour, sdayOfTheWeek, sday, smonth, syear); // записуєм у модуль DS3231
 }
}

void handleRoot() {//обробляєм рут
 String webpage;//описуємо на вебсторінці ESP8266 WI FI setup
 webpage =  "<html>"; //відкриваючий тег
 webpage += "<head><title>ESP8266 WI FI setup </title><meta charset='UTF-8'>";
 webpage += "<style>"; //відкриваючий тег
 webpage += "body { font-family: Verdana; Color: #00979d;}";
 webpage += "</style>";//закриваючий тег
 webpage += "</head>"; //закриваючий тег
 webpage += "<body>";
 webpage += "<body>\
 <form method=\"POST\" action=\"ok\">\
 <p>&nbsp;&nbsp;&nbsp;Пропишіть назву домашньої сітки WIFI</p>\
 &nbsp;&nbsp;&nbsp;<input name=\"ssid\"> WIFI Net</br></br>\ 
 <p>&nbsp;&nbsp;&nbsp;Пропишіть пароль сітки WIFI</p>\ 
 &nbsp;&nbsp;&nbsp;<input name=\"pswd\"> Password</br></br>\
 <p>&nbsp;&nbsp;&nbsp; Збережіть нові налаштування </p>\
 &nbsp;&nbsp;&nbsp;<input type=SUBMIT value=\"Save settings\">\
 </form>\
 </body>\
 </html>";
 server.send ( 200, "text/html", webpage );
} 

void handleOk() {
 String webpage;//описуємо на вебсторінці ESP8266 clock setup
 webpage =  "<html>"; //відкриваючий тег
 webpage += "<head><title>ESP8266 WI FI reboot </title><meta charset='UTF-8'>";
 webpage += "<style>"; //відкриваючий тег
 webpage += "body { font-family: Verdana; Color: #00979d;}";
 webpage += "</style>";//закриваючий тег
 webpage += "</head>"; //закриваючий тег 
 webpage += "<body>";
 String ssid_ap = server.arg(0);
 String pass_ap = server.arg(1);
 if (ssid_ap != "") {
  byte ssid_byte = ssid_ap.length();
  byte pass_byte = pass_ap.length();
  i2c_eeprom_write_byte(EEPROM_I2C_ADDRESS, 96, ssid_byte );
  delay(5);
  i2c_eeprom_write_byte(EEPROM_I2C_ADDRESS, 97, pass_byte);
  delay(5);
  unsigned char *buf = new unsigned char[32];
  ssid_ap.getBytes(buf, ssid_byte + 1);
  for(byte i=0; i< ssid_byte; i++) {
   i2c_eeprom_write_byte(EEPROM_I2C_ADDRESS, i, buf[i] );
   delay(5);
   }
  pass_ap.getBytes(buf, pass_byte + 1);
  for(byte i=0; i< pass_byte; i++) {
   i2c_eeprom_write_byte(EEPROM_I2C_ADDRESS, i+32, buf[i] );
   delay(5);
   }
  webpage +="<big><center>Нова конфігурація збережена у EEPROM </br>\   
  <big><center>Назву сітки WIFI і пароль змінено </p></br></br>\
  <a href=\"/\">Return</a> to settings page</br>";
  delete[] buf ;
  buf = nullptr ;
 }
 else {
  webpage += "<big><center>Не задані нові параметри сітки WIFI</br>\
  <a href=\"/\">Return</a> to settings page</br>";
 }
 webpage += "</body></html>";
 server.send ( 200, "text/html", webpage );
}
void addString (String &webpage, byte application, byte multiplier) {
  if ((byte) i2c_eeprom_read_byte (EEPROM_I2C_ADDRESS,4089+application) == 1) {
  byte len_cote = i2c_eeprom_read_byte (EEPROM_I2C_ADDRESS,4000-(100*multiplier)); 
  if ((len_cote > 0)&&(len_cote < 96)) {
   webpage += "<p><b>&nbsp;&nbsp;В комірці N1 збережені такі налаштування.</b></p>"; 
   webpage += "<p><b>&nbsp;&nbsp;число:</b>&nbsp;" + String(i2c_eeprom_read_byte (EEPROM_I2C_ADDRESS,4001-(100*multiplier))) +
   "<b>&nbsp;&nbsp;&nbsp;&nbsp;місяць:</b>&nbsp;" + String(i2c_eeprom_read_byte(EEPROM_I2C_ADDRESS,4002-(100*multiplier)))+ "<br></p>";
    unsigned char *buf_cote = new unsigned char[len_cote];
   for (byte i=0; i<len_cote; i++) {
    buf_cote[i] = char(i2c_eeprom_read_byte (EEPROM_I2C_ADDRESS,4003+i-(100*multiplier)));
    delay(1);
   }
   buf_cote[len_cote] = '\x0';
   webpage += "<p><b>&nbsp;&nbsp;текст привітання:</b>&nbsp;&nbsp;" + String((char*)buf_cote) + "</p>&nbsp;&nbsp;<br>";
   webpage += "<p><b>-------------------------------------------------------------------</b></p>";
   webpage += "<p><b>&nbsp;&nbsp;ЗмІнити дату і текст  в  комірці ?</b></p>"; 
   delete[] buf_cote ;
   buf_cote = nullptr ;
  }
 } 
}

void StringWrite() {
 String webpage;//описуємо на вебсторінці ESP8266 WI FI setup
 webpage =  "<html>"; //відкриваючий тег
 webpage += "<head><title>ESP8266 WI FI setup </title><meta charset='UTF-8'>";
 webpage += "<style>"; //відкриваючий тег
 webpage += "body { font-family: Verdana; Color: #00979d;}";
 webpage += "</style>";//закриваючий тег
 webpage += "</head>"; //закриваючий тег
 webpage += "<body>";
 addString (webpage,7,1);
 webpage += "<body>\
 <p><b>&nbsp;&nbsp;&nbsp;ЗАПОВНІТЬ ІСНУЮЧІ ПОЛЯ КОМІРКИ N1</b></p>\
 <p>&nbsp;&nbsp;&nbsp;Пропишіть число коли відбуватиметься привітання </p>\
 <form method=\"POST\" action=\"string_ok_1\">\
 &nbsp;&nbsp;&nbsp;<input name=\"dotm\"> Від 1 до 31 </br></br>\ 
 <p>&nbsp;&nbsp;&nbsp;Пропишіть порядковий номер місяця </p>\
 &nbsp;&nbsp;&nbsp;<input name=\"monu\"> Від 1 до 12</br></br>\ 
 <p>&nbsp;&nbsp;&nbsp;Пропишіть текст привітання </p>\ 
 &nbsp;&nbsp;&nbsp;<input name=\"cote\"> Не більше 47 символів кирилицею </br></br>\
 <p>&nbsp;&nbsp;&nbsp; Збережіть нове привітання </p>\
 &nbsp;&nbsp;&nbsp;<input type=SUBMIT value=\"Accept\">\
 <p><b>***************************************************************************</b></p>\
 </form>\
 </body>\
 </html>";
 addString (webpage,6,2);
 webpage += "<body>\
 <p><b>&nbsp;&nbsp;&nbsp;ЗАПОВНІТЬ ІСНУЮЧІ ПОЛЯ КОМІРКИ N2</b></p>\ 
 <p>&nbsp;&nbsp;&nbsp;Пропишіть число коли відбуватиметься привітання </p>\
 <form method=\"POST\" action=\"string_ok_2\">\
 &nbsp;&nbsp;&nbsp;<input name=\"dotm\"> Від 1 до 31 </br></br>\ 
 <p>&nbsp;&nbsp;&nbsp;Пропишіть порядковий номер місяця </p>\
 &nbsp;&nbsp;&nbsp;<input name=\"monu\"> Від 1 до 12</br></br>\ 
 <p>&nbsp;&nbsp;&nbsp;Пропишіть текст привітання </p>\ 
 &nbsp;&nbsp;&nbsp;<input name=\"cote\"> Не більше 47 символів кирилицею </br></br>\
 <p>&nbsp;&nbsp;&nbsp; Збережіть нове привітання </p>\
 &nbsp;&nbsp;&nbsp;<input type=SUBMIT value=\"Accept\">\ 
 <p><b>***************************************************************************</b></p>\
 </form>\
 </body>\
 </html>";
 addString (webpage,5,3);
 webpage += "<body>\
 <p><b>&nbsp;&nbsp;&nbsp;ЗАПОВНІТЬ ІСНУЮЧІ ПОЛЯ КОМІРКИ N3</b></p>\ 
 <p>&nbsp;&nbsp;&nbsp;Пропишіть число коли відбуватиметься привітання </p>\
 <form method=\"POST\" action=\"string_ok_3\">\
 &nbsp;&nbsp;&nbsp;<input name=\"dotm\"> Від 1 до 31 </br></br>\ 
 <p>&nbsp;&nbsp;&nbsp;Пропишіть порядковий номер місяця </p>\
 &nbsp;&nbsp;&nbsp;<input name=\"monu\"> Від 1 до 12</br></br>\ 
 <p>&nbsp;&nbsp;&nbsp;Пропишіть текст привітання </p>\ 
 &nbsp;&nbsp;&nbsp;<input name=\"cote\"> Не більше 47 символів кирилицею </br></br>\
 <p>&nbsp;&nbsp;&nbsp; Збережіть нове привітання </p>\
 &nbsp;&nbsp;&nbsp;<input type=SUBMIT value=\"Accept\">\
 <p><b>***************************************************************************</b></p>\
 </form>\
 </body>\
 </html>";
 addString (webpage,4,4);
 webpage += "<body>\
 <p><b>&nbsp;&nbsp;&nbsp;ЗАПОВНІТЬ ІСНУЮЧІ ПОЛЯ КОМІРКИ N4</b></p>\ 
 <p>&nbsp;&nbsp;&nbsp;Пропишіть число коли відбуватиметься привітання </p>\
 <form method=\"POST\" action=\"string_ok_4\">\
 &nbsp;&nbsp;&nbsp;<input name=\"dotm\"> Від 1 до 31 </br></br>\ 
 <p>&nbsp;&nbsp;&nbsp;Пропишіть порядковий номер місяця </p>\
 &nbsp;&nbsp;&nbsp;<input name=\"monu\"> Від 1 до 12</br></br>\ 
 <p>&nbsp;&nbsp;&nbsp;Пропишіть текст привітання </p>\ 
 &nbsp;&nbsp;&nbsp;<input name=\"cote\"> Не більше 47 символів кирилицею </br></br>\
 <p>&nbsp;&nbsp;&nbsp; Збережіть нове привітання </p>\
 &nbsp;&nbsp;&nbsp;<input type=SUBMIT value=\"Accept\">\
 <p><b>***************************************************************************</b></p>\
 </form>\
 </body>\
 </html>";
 addString (webpage,3,5);
 webpage += "<body>\
 <p><b>&nbsp;&nbsp;&nbsp;ЗАПОВНІТЬ ІСНУЮЧІ ПОЛЯ КОМІРКИ N5</b></p>\ 
 <p>&nbsp;&nbsp;&nbsp;Пропишіть число коли відбуватиметься привітання </p>\
 <form method=\"POST\" action=\"string_ok_5\">\
 &nbsp;&nbsp;&nbsp;<input name=\"dotm\"> Від 1 до 31 </br></br>\ 
 <p>&nbsp;&nbsp;&nbsp;Пропишіть порядковий номер місяця </p>\
 &nbsp;&nbsp;&nbsp;<input name=\"monu\"> Від 1 до 12</br></br>\ 
 <p>&nbsp;&nbsp;&nbsp;Пропишіть текст привітання </p>\ 
 &nbsp;&nbsp;&nbsp;<input name=\"cote\"> Не більше 47 символів кирилицею </br></br>\
 <p>&nbsp;&nbsp;&nbsp; Збережіть нове привітання </p>\
 &nbsp;&nbsp;&nbsp;<input type=SUBMIT value=\"Accept\">\
 <p><b>***************************************************************************</b></p>\
 </form>\
 </body>\
 </html>";
 addString (webpage,2,6);
 webpage += "<body>\
 <p><b>&nbsp;&nbsp;&nbsp;ЗАПОВНІТЬ ІСНУЮЧІ ПОЛЯ КОМІРКИ N6</b></p>\ 
 <p>&nbsp;&nbsp;&nbsp;Пропишіть число коли відбуватиметься привітання </p>\
 <form method=\"POST\" action=\"string_ok_6\">\
 &nbsp;&nbsp;&nbsp;<input name=\"dotm\"> Від 1 до 31 </br></br>\ 
 <p>&nbsp;&nbsp;&nbsp;Пропишіть порядковий номер місяця </p>\
 &nbsp;&nbsp;&nbsp;<input name=\"monu\"> Від 1 до 12</br></br>\ 
 <p>&nbsp;&nbsp;&nbsp;Пропишіть текст привітання </p>\ 
 &nbsp;&nbsp;&nbsp;<input name=\"cote\"> Не більше 47 символів кирилицею </br></br>\
 <p>&nbsp;&nbsp;&nbsp; Збережіть нове привітання </p>\
 &nbsp;&nbsp;&nbsp;<input type=SUBMIT value=\"Accept\">\
 <p><b>***************************************************************************</b></p>\
 </form>\
 </body>\
 </html>"; 
 addString (webpage,1,7);
 webpage += "<body>\
 <p><b>&nbsp;&nbsp;&nbsp;ЗАПОВНІТЬ ІСНУЮЧІ ПОЛЯ КОМІРКИ N7</b></p>\ 
 <p>&nbsp;&nbsp;&nbsp;Пропишіть число коли відбуватиметься привітання </p>\
 <form method=\"POST\" action=\"string_ok_7\">\
 &nbsp;&nbsp;&nbsp;<input name=\"dotm\"> Від 1 до 31 </br></br>\ 
 <p>&nbsp;&nbsp;&nbsp;Пропишіть порядковий номер місяця </p>\
 &nbsp;&nbsp;&nbsp;<input name=\"monu\"> Від 1 до 12</br></br>\ 
 <p>&nbsp;&nbsp;&nbsp;Пропишіть текст привітання </p>\ 
 &nbsp;&nbsp;&nbsp;<input name=\"cote\"> Не більше 47 символів кирилицею </br></br>\
 <p>&nbsp;&nbsp;&nbsp; Збережіть нове привітання </p>\
 &nbsp;&nbsp;&nbsp;<input type=SUBMIT value=\"Accept\">\
 <p><b>***************************************************************************</b></p>\
 </form>\
 </body>\
 </html>";
 addString (webpage,0,8);
 webpage += "<body>\
 <p><b>&nbsp;&nbsp;&nbsp;ЗАПОВНІТЬ ІСНУЮЧІ ПОЛЯ КОМІРКИ N8</b></p>\ 
 <p>&nbsp;&nbsp;&nbsp;Пропишіть число коли відбуватиметься привітання </p>\
 <form method=\"POST\" action=\"string_ok_8\">\
 &nbsp;&nbsp;&nbsp;<input name=\"dotm\"> Від 1 до 31 </br></br>\ 
 <p>&nbsp;&nbsp;&nbsp;Пропишіть порядковий номер місяця </p>\
 &nbsp;&nbsp;&nbsp;<input name=\"monu\"> Від 1 до 12</br></br>\ 
 <p>&nbsp;&nbsp;&nbsp;Пропишіть текст привітання </p>\ 
 &nbsp;&nbsp;&nbsp;<input name=\"cote\"> Не більше 47 символів кирилицею </br></br>\
 <p>&nbsp;&nbsp;&nbsp; Збережіть нове привітання </p>\
 &nbsp;&nbsp;&nbsp;<input type=SUBMIT value=\"Accept\">\
 <p><b>***************************************************************************</b></p>\
 </form>\
 </body>\
 </html>";
 server.send ( 200, "text/html", webpage );
} 

void addFoonctional (String &webpage, byte application, byte multiplier) {
 webpage =  "<html>"; //відкриваючий тег
 webpage += "<head><title>ESP8266 WI FI reboot </title><meta charset='UTF-8'>";
 webpage += "<style>"; //відкриваючий тег
 webpage += "body { font-family: Verdana; Color: #00979d;}";
 webpage += "</style>";//закриваючий тег
 webpage += "</head>"; //закриваючий тег 
 webpage += "<body>";
 String dotm = server.arg(0);
 String monu = server.arg(1);
 String cote = server.arg(2);
 if (cote != "") {
  byte length_cote = cote.length(); // вказуємо кількість символів у тексті
  if (length_cote < 96) {
   if (dotm != "") {
    byte byte_dotm = dotm.toInt(); // конвертуєм String в byte
    if ((byte_dotm > 0) && (byte_dotm <=31)) {
     if (monu != "") {
      byte byte_monu = monu.toInt();
      if ((byte_monu > 0) && (byte_monu <=12)) {
       i2c_eeprom_write_byte(EEPROM_I2C_ADDRESS,4089+application, 1 ); // вказуємо, що комірка заповнена
       delay(5);
       i2c_eeprom_write_byte(EEPROM_I2C_ADDRESS, 4000-(100*multiplier), length_cote ); // записуєм  кількість символів у тексті
       delay(5);
       i2c_eeprom_write_byte(EEPROM_I2C_ADDRESS, 4001-(100*multiplier), byte_dotm ); // byte записуємо в EEPROM
       delay(5); 
       i2c_eeprom_write_byte(EEPROM_I2C_ADDRESS, 4002-(100*multiplier), byte_monu );
       delay(5);
       unsigned char *buf = new unsigned char[length_cote];
       cote.getBytes(buf, length_cote + 1);  // String конвертуєм у байти ,і вкладаєм у тимчасовий буфер + ,,символ завершення,,
       for(byte i=0; i < length_cote; i++) { // кожен байт із конвертованого тексту по черзі записуєм в EEPROM 
        i2c_eeprom_write_byte(EEPROM_I2C_ADDRESS, 4003+i-(100*multiplier), buf[i] ); // в EEPROM у вказану комірку записуєм  байт
        delay(5);
       }
       webpage +="<big><center>Нова конфігурація збережена у EEPROM </br>\   
       <big><center>Число і текст змінено </p></br></br>\
       <a href=\"/\">Return</a> to settings page</br>";
       delete[] buf ;
       buf = nullptr ;
      }
      else {
       webpage += "<big><center>Не коректно вказано значення місяця</br>\
       <a href=\"/\">Return</a> to settings page</br>";
      } 
     }
    }
    else {
     webpage += "<big><center>Не коректно вказано значення дня</br>\
     <a href=\"/\">Return</a> to settings page</br>";
    } 
   }
  }
  else {
   webpage += "<big><center>Перевищено максимальну довжину тексту</br>\
   <a href=\"/\">Return</a> to settings page</br>";
  }
 }
 else {
  i2c_eeprom_write_byte(EEPROM_I2C_ADDRESS,4089+application, 0 ); // вказуємо, що комірка порожня
  delay(5);
  webpage += "<big><center>Не задане відображення тексту</br>\
  <a href=\"/\">Return</a> to settings page</br>";
 }
  webpage += "</body></html>"; 
}

void StringCell_1 () {
 String webpage;
 addFoonctional (webpage,7,1); 
 server.send ( 200, "text/html", webpage );
}

void StringCell_2() {
 String webpage;
 addFoonctional (webpage,6,2);
 server.send ( 200, "text/html", webpage );
}

void StringCell_3() {
 String webpage;
 addFoonctional (webpage,5,3);
 server.send ( 200, "text/html", webpage );
}

void StringCell_4 () {
 String webpage;
 addFoonctional (webpage,4,4);
 server.send ( 200, "text/html", webpage );
}

void StringCell_5 () {
 String webpage;
 addFoonctional (webpage,3,5);
 server.send ( 200, "text/html", webpage );
}

void StringCell_6 () {
 String webpage;
 addFoonctional (webpage,2,6);
 server.send ( 200, "text/html", webpage );
}

void StringCell_7 () {
 String webpage;
 addFoonctional (webpage,1,7);
 server.send ( 200, "text/html", webpage );
}

void StringCell_8 () {
 String webpage;
 addFoonctional (webpage,0,8);
 server.send ( 200, "text/html", webpage );
}
