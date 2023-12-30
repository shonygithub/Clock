// вкладка работы с bluetooth

#define PARSE_AMOUNT 4    // максимальное количество значений в массиве, который хотим получить
#define header '$'        // стартовый символ
#define divider ' '       // разделительный символ
#define ending ';'        // завершающий символ
static bool sepia = false;
byte prevY = 0;
byte prevX = 0;
byte intData[PARSE_AMOUNT];     // массив численных значений после парсинга
boolean recievedFlag;

void bluetoothRoutine() {
 parsing();                                              // принимаем данные
 if (!parseStarted ) {                                   // тільки якщо не дозволена процедура ,,парсингу,,
  if (runningFlag) {fillString(runningText, ledColor);}  // якщо дозволена  процедура ,,бігуча строка,, викликаємо функцію друку тексту
 } 
}

byte parse_index;
String string_convert = "";
enum modes {NORMAL, COLOR, TEXT} parseMode;

// ********************* ПРИНИМАЕМ ДАННЫЕ **********************
void parsing() {
 if (recievedFlag) { // якщо взедено флажок, бо підтвержено що дані отримано повністю (тобто з буфера взято символ завершення ';')
  switch (intData[0]) {//якщо у масиві intData комірка з індексом 0 (тобто саме перша)набуває значень :
   case 1: // якщо отримали дані $1 20; ($ стартовий символ, 1 управляючий символ, обовязковий пробіл, швидкість від 15 до 40(рекомендовано), ; символ завершення)
    globalSpeed = intData[1];
    if (runningFlag) {scrollTimer.setInterval(globalSpeed);}
    break;
   case 2: // якщо отримали дані $2 текст ($ стартовий, 2 управляючий символ, обовязковий пробіл, прописуємо текст)
    loadingFlag = true;        // разрешаем строке принимается в переменную runningText
    break;
   case 3: // якщо отримали дані $3 1; ($ стартовий, 3 управляючий, обовязковий пробіл, 1 управляючий, ; символ завершення)
                        // або $3 0; ($ стартовий, 3 управляючий, обовязковий пробіл, 0 управляючий стоп, ; символ завершення)
    if (intData[1] == 1) {
     runningFlag = true;   //  дозволяємо  процедуру ,,бігуча строка,,(СТАРТ)
     scrollTimer.setInterval(globalSpeed);
    }
    if (intData[1] == 0) {
     runningFlag = false;  //  забороняємо процедуру ,, бігуча строка,,(СТОП)
     FastLED.clear();
    }
    break;
   case 4: // якщо отримали дані $4; ($ стартовий символ, 4 управляючий символ, ; символ завершення)
    mp3_set_volume(25);
    mp3_play(101);  // відтворюємо трек з номером 0101.мр3
    break;
   case 5: // якщо отримали дані $5; ($ стартовий символ, 5 управляючий символ, ; символ завершення)- 
    mp3_set_volume(25);
    mp3_play(102);  // відтворюємо трек з номером 0102.мр3
    break;
   case 6: // якщо отримали дані $6; ($ стартовий символ, 6 управляючий символ, ; символ завершення)- 
    mp3_set_volume(25);
    mp3_play(103);  // відтворюємо трек з номером 0103.мр3
    break;
   case 7: // якщо отримали дані $7; ($ стартовий символ, 7 управляючий символ, ; символ завершення)- 
    mp3_set_volume(25);
    mp3_play(104);  // відтворюємо трек з номером 0104.мр3
    break;
   case 8: // якщо отримали дані $8; ($ стартовий символ, 8 управляючий символ, ; символ завершення)- 
    mp3_set_volume(25);
    mp3_play(105);  // відтворюємо трек з номером 0105.мр3
    break;
   case 9: // якщо отримали дані $9; ($ стартовий символ, 9 управляючий символ, ; символ завершення)- 
    mp3_set_volume(25);
    mp3_play(106);  // відтворюємо трек з номером 0106.мр3
    break; 
   case 10: // якщо отримали дані $10; ($ стартовий символ, 10 управляючий символ, ; символ завершення)- 
    mp3_set_volume(25);
    mp3_play(107);  // відтворюємо трек з номером 0107.мр3
    break;  
   case 11: // якщо отримали дані $11; ($ стартовий символ, 11 управляючий символ, ; символ завершення)- 
    mp3_set_volume(25);
    mp3_play(108);  // відтворюємо трек з номером 0108.мр3
    break; 
   case 12: // якщо отримали дані $12; ($ стартовий символ, 11 управляючий символ, ; символ завершення)- 
    mp3_set_volume(25);
    mp3_play(109);  // відтворюємо трек з номером 0109.мр3
    break; 
   case 13: // якщо отримали дані $13; ($ стартовий символ, 11 управляючий символ, ; символ завершення)- 
    mp3_set_volume(25);
    mp3_play(110);  // відтворюємо трек з номером 0110.мр3
    break; 
   case 14: // якщо отримали дані $14; ($ стартовий символ, 11 управляючий символ, ; символ завершення)- 
    mp3_set_volume(25);
    mp3_play(111);  // відтворюємо трек з номером 0111.мр3
    break; 
   case 15: // якщо отримали дані $15; ($ стартовий символ, 11 управляючий символ, ; символ завершення)- 
    mp3_set_volume(25);
    mp3_play(112);  // відтворюємо трек з номером 0112.мр3
    break; 
   case 16: // якщо отримали дані $16; ($ стартовий символ, 11 управляючий символ, ; символ завершення)- 
    mp3_set_volume(25);
    mp3_play(113);  // відтворюємо трек з номером 0113.мр3
    break; 
  // дописуєте скільки ще потрібно              
  }
    recievedFlag = false;
 }
 if (Serial.available() > 0) {         // очікуємо поступлення у буфер з блютуз модуля
  char incomingByte;                   // отриманий по блютузу символ який обумовлює подальші дії функції parsing()
  if (parseMode == TEXT) {             // если нужно принять строку
   runningText = Serial.readString();  // принимаем всю
   incomingByte = ending;              // сразу завершаем парс
   parseMode = NORMAL;
  } 
  else {
   incomingByte = Serial.read();       // обязательно ЧИТАЕМ входящий символ
  }
  if (parseStarted) {                  // якщо дозволена процедура ,,парсингу,,(отримали стартовий символ '$')
   if (incomingByte != divider && incomingByte != ending) {   // если это не символ пробела и не символ конца
    string_convert += incomingByte;    // складываем в строку
   } 
   else {                              // а если это пробел или ; конец пакета
    if (parse_index == 0) {
     byte thisMode = string_convert.toInt();
     if (thisMode == 0) {parseMode = COLOR;}    // передача цвета (в отдельную переменную)
     else if (thisMode == 2) {parseMode = TEXT;}
     else parseMode = NORMAL;
    }
     if (parse_index == 1) {          // для второго (с нуля) символа в посылке
      if (parseMode == NORMAL) {intData[parse_index] = string_convert.toInt();}  // преобразуем строку в int и кладём в массив}
      if (parseMode == COLOR) {ledColor = strtol(&string_convert[0], NULL, 16);} // преобразуем строку HEX в цифру
     } 
     else { // інакше коли parse_index набув значення відмінне від 0 абож 1
      intData[parse_index] = string_convert.toInt(); // преобразуем строку в int и кладём в массив 
     }  
     string_convert = "";                 // очищаем строку
     parse_index++;                       // переходим к парсингу следующего элемента массива
   }
  }
  if (incomingByte == header) {           // якщо у буфері $ (стартовий символ)
    parseStarted = true;                  // дозволяєм процедуру ,,парсингу,,
    parse_index = 0;                      // набуває значення 0 (скидаєм в нуль)
    string_convert = "";                  // спорожнюємо строку
  }
  if (incomingByte == ending) {           // если таки приняли ; , значит это конец парсинга
    parseMode = NORMAL;
    parseStarted = false;                 // забороняєм процедуру ,,парсингу,,
    recievedFlag = true;                  // флаг на принятие
  }
 }
}

#define LET_WIDTH 5       //  ширина буквы шрифта
#define LET_HEIGHT 8      //  высота буквы шрифта
#define SPACE 1           //  пробел

void fillAll() {
 for (int i = 0; i < NUM_LEDS; i++)  {leds[i] = CHSV(degreeColor,200,255);}
}

int16_t offset = WIDTH; //порядковий номер вертикального стовпчика на матриці

void fillString(String text, uint32_t color) { // вказуємо в параметрах строку(масив символів) з бігучим текстом і значення коліру
 byte uni = 0;
 if (loadingFlag) { // якщо строка принята за дозволом взведеного флажка loadingFlag 
  offset = WIDTH;   // починаємо з самого крайнього правого стовпчика
  loadingFlag = false; //скидаємо флаг   
 } 
 if (scrollTimer.isReady()) { // якщо підійшов момент оновлення відображення тексту на матриці
  if (sepia){ fillAll(); }
  else {FastLED.clear();}     // повністю витираємо (гасимо) усі пікселі
  byte i = 0, j = 0;
  while (text[i] != '\0') {   // допоки перебираючи елементи масиву не отримали символа '\0'(добавленого компілятором)
   if ((byte)text[i] == 208) { // якщо перед символом розміщено ще і допоміжний симмол UNIT кодування
    uni = 0;
    i++;
   }
   else if ((byte)text[i] == 209) { 
    uni = 1;
    i++;
   } 
   else if ((byte)text[i] == 210) { 
    uni = 2;
    i++;
   } 
   else { 
    drawLetter(j, text[i], offset + j * (LET_WIDTH + SPACE), color, uni); // викликаємо функцію друку літери,
// у яку передаємо  аргументи: порядковий номер літери(яку потрібно друкувати на матриці);
//порядковий номер символа у бігучому тексті;
//суму поточного номера стовпчика і фізичну ширину символів(усіх прочитаних на цей момент із бігучого тексту) з пробілами 
    i++;
    j++;
   }
  }  // після прочитання усіх символів із бігучого тексту(і друку  усього що могло поміститись до правого краю матриці)
  offset--;  // кожен раз зміщуємось на один стовпчик вліво (візуально текст побіг)
  if (offset < -j * (LET_WIDTH + SPACE)) { // якщо позиція offset на горизонтальній осі стала меньша ніж від'ємне значення
      // суми усіх літер з пробілами , значить останній символ тексту вже забіг за лівий край матриці  
   offset = WIDTH ; // починаємо знов все спочатку, виводити бігучий текст.
   counterText ++ ;
   if (counterText > countText) {
    counterText = 0;
    countText = 5;
    parseStarted = true; // дозволяєм парсити з блютузу
    runningFlag = false; // забороняєм процедуру  ,,бігучий текст,,
   }
  }
  FastLED.show();
  degreeColor ++;
 }
}

void drawLetter(uint8_t index, uint8_t letter, int16_t offset, uint32_t color, byte uni) {
 int8_t start_pos = 0 ;
 int8_t finish_pos = LET_WIDTH; //ширина літери 
 CRGB LetterColor;
 if (color == 0) {     // якщо по блютузу передали  $0 000000;
  LetterColor = color; // чорний текст біжить по переливаючому фоні
  sepia = true;
 }
 else {sepia = false;}
 if (color == 1) {     // якщо по блютузу передали  $0 000001;
  LetterColor = CHSV(byte(offset * 10), 255, 255); // кожна літера(символи) бігучого тексту отримує особистий колір
 }
 else if (color == 2) { // якщо по блютузу передали $0 000002;
  LetterColor = CHSV(byte(index * 30), 255, 255); // літери(символи)  змінюють колір відносно поточної позиціі на матриці
 }
 else if (color == 3) { // якщо по блютузу передали $0 000003;
  LetterColor = CHSV(degreeColor, 255, 255); // усі літери(символи) бігучого тексту змінюють колір відносно часу
 }
 else {LetterColor = color;} 

 if (offset < -LET_WIDTH || offset > WIDTH) return;// поки переданий параметр offset меньший від відємного значення 
  // ширини літери або більший від максимального значення стовбчиків на матриці, значить просто виходимо із функції
 if (offset < 0) {start_pos = -offset;}
 if (offset > (WIDTH - LET_WIDTH)) {finish_pos = WIDTH - offset;}
 for (byte i = start_pos; i < finish_pos; i++) {  
  int thisByte;
  thisByte = getFont((byte)letter, i, uni);
  for (byte j = 0; j < LET_HEIGHT; j++) {
   boolean thisBit;
   thisBit = thisByte & (1 << (LET_HEIGHT - 1 - j)); //перевіряємо конкретний біт у байті
   if (thisBit) {
    leds[getPixelNumber(offset + i, j)] = LetterColor;       
   }
  }  
 }
}

uint16_t getPixelNumber(int8_t x, int8_t y) { // отримати порядковий номер пікселя на матриці за координатами
  y = HEIGHT - y - 1 ;
  if (x % 2 == 0)  {          // якщо порядковий номер стовбчика  парне число
    return (x * HEIGHT + y);  // номер стовпчика перемножуємо на кількість пікселів у стовпчику і додаємо номер ряду
  } 
  else {                      // інакше порядковий номер стовбчика  не парне число
    return (x * HEIGHT + HEIGHT - y - 1); // номер стовпчика перемножуємо на кількість пікселів у стовпчику
  }    //і додаємо  різницю кількісті пікселів у стовпчику  і номера ряду мінус один
}

// интерпретатор кода символа в массиве fontHEX (для Arduino IDE 1.8.* и выше)
uint16_t getFont(uint8_t font, uint8_t row, byte uni) {
  if (font <= 126) 
   return pgm_read_byte(&(fontHEX[font - 32][row]));     // для английских букв и символов
  else if (uni == 0 && font >= 129 && font <= 191 ) {    
   return pgm_read_byte(&(fontHEX[font - 34 ][row]));
  } 
  else if (uni == 1 && font >= 128 && font <= 151) {
   return pgm_read_byte(&(fontHEX[font + 30][row]));
  }
  else if (uni == 2 && font >= 144 && font <= 145) {
   return pgm_read_byte(&(fontHEX[font + 38][row]));
  }
}
