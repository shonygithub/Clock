void DaylightSavingTime() {               // автоматичний перехід на літній час
 if (summerTime) {                        // якщо увімкнено -- ПЕРЕІД НА ЛІТНІЙ ЧАС
  static uint8_t summerTimeshour ;        // щоб лише раз на годину обробляти функцію DaylightSavingTime()
  if (nowHour != summerTimeshour) {
   DateTime now = rtc.now();
   tmElements_t Now;
   RTC.read(Now);
   uint8_t nhour =  now.hour();
   uint8_t ndayOfTheWeek = now.dayOfTheWeek();
   uint8_t nday = now.day();
   uint8_t nmonth = now.month();      
   if (dst == 0) { // поки не відбувся перехід на літній час
    if (nmonth == 3) { // якщо місяць БЕРЕЗЕНЬ
     if (ndayOfTheWeek == 0) { // якщо день тижня НЕДІЛЯ
      if (nday > 24 ) { // якщо настав останній тиждень березня
       if (nhour >= 3 ) { // за звичай переводять на літній час у третій ночі
        if (Now.Hour== 23) {Now.Hour = 0;}
        else {Now.Hour += 1;}
        RTC.write(Now);
        dst = 1;
        i2c_eeprom_write_byte(EEPROM_I2C_ADDRESS,208, dst );// записуємо в енергонезалежну память
       }
      }
     }
    }
   }
   else  { // інакше поки не відбувся перехід на зимовий час
    if (nmonth == 10) { // якщо місяць ЖОВТЕНЬ
     if (ndayOfTheWeek == 0) {
      if (nday > 24 ) {
       if (nhour  >= 4) {
        Now.Hour -= 1;
        RTC.write(Now);
        dst = 0;
        i2c_eeprom_write_byte(EEPROM_I2C_ADDRESS,208, dst );
       }
      }
     }     
    }
   }
   summerTimeshour = nowHour; 
  } 
 }  
}
