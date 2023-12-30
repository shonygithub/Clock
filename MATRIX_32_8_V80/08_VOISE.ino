uint8_t Play(uint8_t playHour) {//рандомний(випадковий) вибір треків із записаних на SDкарту вимовлянням часу від різних людей 
  //якщо не записали 5 різних голосів , просто клонуйте(повторюйте)вже раніш записані 
 uint8_t playTable[17][5] = {
  {1,2,3,4,5},//різні голоси для оповіщення,що вже 7 година ранку(відповідно назви треків 0001.mp3 ; 0002.mp3 ;..... 0005.mp3                                                                                                
  {6,7,8,9,10},//оповіщення,що  8 година ранку(відповідно назви треків 0006.mp3 ; 0007.mp3 ;..... 0010.mp3 
  {11,12,13,14,15},//оповіщення,що вже 9 година ранку 
  {16,17,18,19,20},{21,22,23,24,25},{26,27,28,29,30},{31,32,33,34,35},{36,37,38,39,40},{41,42,43,44,45},
  {46,47,48,49,50},{51,52,53,54,55},{56,57,58,59,60},{61,62,63,64,65},{66,67,68,69,70},{71,72,73,74,75},
  {76,77,78,79,80}, //оповіщення,що вже 10 година вечора
  {81,82,83,84,85}  //оповіщення,що вже 11 година вечора, назви треків 0081.mp3 ; 0082.mp3 ;..... 0085.mp3  
 }; 
 return (playTable[playHour][random(5)]);
}
  
void PlayHours () {               // вимовляємо час
 if (nowMinute == 0) {            // якщо хвилини мають значення 0 
  if (nowHour > 6) {              // починаючи із сьомої години ранку
   mp3_set_volume (volume);       // встановіть у веб інтерфейсі гучність звуку від 1 до 30(за замовчуванням буде 10)
   mp3_play (Play(nowHour - 7));  // викликаємо функцію програвання голосових сповіщень записаних на SDкарту
   if (nowHour == 9) {
    runningText = (F("Biтаю вcix xopoшиx людей з таким пpeкpacним днем!!!")); 
    parseStarted = false; // забороняєм парсити з блютузу
    runningFlag = true; // дозволяєм процедуру  ,,бігучий текст,, 
    countText = 0;
   }
   tmElements_t Now;
   RTC.read(Now);
   byte Day = Now.Day;
   byte Month = Now.Month;
   FonctionRunningText (1,Day,Month);       // перевірить дату і покажем бігучий текст з привітанням
   FonctionRunningText (2,Day,Month);
   FonctionRunningText (3,Day,Month);
   FonctionRunningText (4,Day,Month);
   FonctionRunningText (5,Day,Month);
   FonctionRunningText (6,Day,Month);
   FonctionRunningText (7,Day,Month);
   FonctionRunningText (8,Day,Month);
  }
 }
}

void FonctionRunningText (byte b, byte Day, byte Month) { 
 if ((byte) i2c_eeprom_read_byte (EEPROM_I2C_ADDRESS,4097 - b) == 1) { // якщо в комірку дані записано  коректно
  if (Day == i2c_eeprom_read_byte (EEPROM_I2C_ADDRESS,4000-100*b+1))  { // якщо день підходить
   if (Month == i2c_eeprom_read_byte (EEPROM_I2C_ADDRESS,4000-100*b+2))  { // і якщо місясь підходить
    byte len_cote = i2c_eeprom_read_byte (EEPROM_I2C_ADDRESS,4000-100*b); // взнаєм довжину тексту
    unsigned char* buf_cote = new unsigned char[len_cote];  // створюєм тимчасовий масив для вкладання символів тексту привітання
     for (byte i=0; i<len_cote; i++) {  //по байтово читаєм з EEPROM
      buf_cote[i] = char(i2c_eeprom_read_byte (EEPROM_I2C_ADDRESS,4000-100*b+3+i)); // вкладаєм в масив зконвертовані символи      delay(1);
     }
     buf_cote[len_cote] = '\x0'; //дописуєм символ завершення
     runningText = ((char*)buf_cote); // масив ,,вливаєм,, у глобальну строку String runningText
     parseStarted = false; // забороняєм парсити з блютузу
     runningFlag = true; // дозволяєм процедуру  ,,бігучий текст,, 
     countText = 2;
     delete[] buf_cote ;
     buf_cote = nullptr ;
   }
  }
 }
}
