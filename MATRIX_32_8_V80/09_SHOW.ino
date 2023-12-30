
static int8_t line;

void ShowFonction(uint16_t NowTime) {   // вибір шоу
 static uint8_t VAR_show  = 1;  //для вибору варіанта шоу
 if (VAR_show == 6) {VAR_show = 1;}  
 switch(VAR_show) {
  case 1: TickerLine      (NowTime);           break;
  case 2: FallingLine     (NowTime);           break;
  case 3: Backstage       (NowTime);           break;
  case 4: DiveLine        (NowTime);           break;
  case 5: Curtain         (NowTime);           break;
 }
 VAR_show ++;   
}

void DiveLine(uint16_t nowTime) {  // функція ,,пікіруюча лінія,,
 for (line = 0; line<HEIGHT; line++) {    // з верху по одній лінії опускаємось до низу
  for (int8_t j = 24; j>=0; j-=8) {    //  цикл з трьох ітерацій
   bias = NUM_LEDS - HEIGHT * 4 * (line + 1) + j;   // 
   if (bias%(HEIGHT*2)!=0) {       //якщо порядковий номер стовбчика  парне число -- відраховуєтья зверху до низу
    for (int8_t i = 0; i<= line; i++) {    //
     for(int16_t a=bias+HEIGHT-i-1; a<(NUM_LEDS+HEIGHT*2-i-1); a+=HEIGHT*2 ) {  //
      leds[a] = dawnColor;   //
      int16_t b = a+2*i+1;  //
      if (b<NUM_LEDS)  {leds[b] = dawnColor;}   //
     } 
    }
    uint16_t now = nowTime; // отримуєм чотиризначне значення часу (наприклад з 11:35 оримуєм тисяча сто тридцять п'ять)
    uint8_t dSecund = 2;    // приблизно 2 секунди триває анімація 
    for (int8_t i=1; i<=2; i++) {                  //друкуємо секунди
     int8_t digitSecund = dSecund % 10;            //остача від ділення на 10
     if (i==1) {FallingOf(224, digitSecund);}      //створюєм символ сесунди першого порядку
     else {Falling(184, digitSecund);}             //створюєм символ сесунди другого порядку 
     dSecund /= 10;
    }
    for (uint8_t i=1; i<=4; i++) {   
     uint16_t digit = now % 10; 
     if (i==1) {Falling (136, digit);}
     else if (i==2) {FallingOf(96, digit);}
     else if (i==3) {Falling (40, digit);}
     else {FallingOf(0, digit);}   
     now /= 10;
    }
   }
   else {
    for (int8_t i = 0; i<= line; i++) {    
     for(int16_t a=bias+i; a<(NUM_LEDS-i); a+=HEIGHT*2 ) {
      leds[a] = dawnColor;
      int16_t b = a+HEIGHT*2-2*i-1; 
      leds[b] = dawnColor; 
     } 
    }
    uint16_t now = nowTime; // отримуєм чотиризначне значення часу (наприклад з 11:35 оримуєм тисяча сто тридцять п'ять)
    uint8_t dSecund = 2;    // приблизно 2 секунди триває анімація 
    for (int8_t i=1; i<=2; i++) {                  //друкуємо секунди
     int8_t digitSecund = dSecund % 10;            //остача від ділення на 10
     if (i==1) {Falling(224, digitSecund);}        //створюєм символ сесунди першого порядку
     else {FallingOf(184, digitSecund);}           //створюєм символ сесунди другого порядку 
     dSecund /= 10;
    }
    for (uint8_t i=1; i<=4; i++) {   
     uint16_t digit = now % 10; 
     if (i==1) {FallingOf (136, digit);}
     else if (i==2) {Falling(96, digit);}
     else if (i==3) {FallingOf (40, digit);}
     else {Falling(0, digit);}   
     now /= 10;
    }
   }
   FastLED.show(); // Display leds array
   delay(15);
  }
 }
 line = 0;
}

void TickerLine(uint16_t nowTime) { // функція ,,бігуча лінія,,  
 for(bias=NUM_LEDS-HEIGHT; bias>=0; bias-=HEIGHT) {     // перебираєм стовпчики з кінця до початку
  uint16_t now = nowTime; // отримуєм чотиризначне значення часу (наприклад з 11:35 оримуєм тисяча сто тридцять п'ять)
  uint8_t dSecund = 2;    // приблизно 2 секунди триває анімація 
  LedsFon(bias,NUM_LEDS); // витираєм по стовпчику     
  for (uint8_t i=1; i<=2; i++) {        //друкуємо секунди
   uint8_t digitSecund = dSecund % 10;  //остача від ділення на 10
   if (bias%(HEIGHT*2)==0) {            //якщо порядковий номер стовбчика  парне число -- відраховуєтья зверху до низу
    if (i==1) {PairOfdigits(224, digitSecund);}    //створюєм символ сесунди першого порядку
    else {Pairdigits(184, digitSecund);}           //створюєм символ сесунди другого порядку 
   }
   else {  //інакше порядковий номер стовбчика  не парне число -- відраховуєтья знизу до верху
    if (i==1) {Pairdigits(224, digitSecund);}
    else {PairOfdigits(184, digitSecund);}   
   } 
   dSecund /= 10;
  }
  for(uint8_t i=1; i<=4; i++) {   
   uint16_t digit = now % 10; // get last digit in time
   if (bias%(HEIGHT*2)==0) {
    if (i==1) {Pairdigits (136, digit);}
    else if (i==2) {PairOfdigits(96, digit);}
    else if (i==3) {Pairdigits (40, digit);}
    else {PairOfdigits(0, digit);}
   }
   else {
    if (i==1) {PairOfdigits (136, digit);}
    else if (i==2) {Pairdigits (96, digit);}
    else if (i==3) {PairOfdigits (40, digit);}
    else {Pairdigits (0, digit);}
   }    
   now /= 10;
  }
  FastLED.show(); // Display leds array
  delay(1024/(bias+8)); 
 }
 bias = 0;
}

void Curtain(uint16_t nowTime) { // функція ,,порт'єра,,
 for (line=HEIGHT-1; line>=0; line--) {
  uint16_t now = nowTime;
  uint8_t dSecund = 2;                                  //приблизно 2 секунди триває анімація
  for (int16_t a=line; a<(NUM_LEDS-HEIGHT); a+=HEIGHT*2 ) { // витираєм лінію
   leds[a] = dawnColor;                                 // витираєм піксель у непарному стовчику
   int16_t b = a+HEIGHT*2-2*line-1; 
   leds[b] = dawnColor;                                 // витираєм піксель у парному стовчику
  }
  for (int8_t i=1; i<=2; i++) {                         //друкуємо секунди
   int8_t digitSecund = dSecund % 10;                   //остача від ділення на 10
   if (i==1) {CurtainLine(224, digitSecund);}           //створюєм символ сесунди першого порядку
   else {CurtainOfLine(184, digitSecund);}              //створюєм символ сесунди другого порядку 
   dSecund /= 10;
  }
  for (uint8_t i=1; i<=4; i++) {   
   uint16_t digit = now % 10; 
   if (i==1) {CurtainOfLine (136, digit);}
   else if (i==2) {CurtainLine(96, digit);}
   else if (i==3) {CurtainOfLine (40, digit);}
   else {CurtainLine(0, digit);}  
   now /= 10;
  }
  FastLED.show(); // Display leds array
  delay(100);
 }
}

void FallingLine(uint16_t nowTime) { // функція ,,падаюча лінія,,
 for (line = 0; line < HEIGHT; line++) {
  uint16_t now = nowTime;
  uint8_t dSecund = 2;   //приблизно 2 секунди триває анімація
  for(int16_t a=line; a<(NUM_LEDS-HEIGHT); a+=HEIGHT*2 ) {
   leds[a] = dawnColor;
   int16_t b = a+HEIGHT*2-2*line-1; 
   leds[b] = dawnColor; 
  }
  for (int8_t i=1; i<=2; i++) {                         //друкуємо секунди
   int8_t digitSecund = dSecund % 10;                   //остача від ділення на 10
   if (i==1) {Falling(224, digitSecund);}               //створюєм символ сесунди першого порядку
   else {FallingOf(184, digitSecund);}                  //створюєм символ сесунди другого порядку 
   dSecund /= 10;
  }
  for (uint8_t i=1; i<=4; i++) {   
   uint16_t digit = now % 10; 
   if (i==1) {FallingOf (136, digit);}
   else if (i==2) {Falling(96, digit);}
   else if (i==3) {FallingOf (40, digit);}
   else {Falling(0, digit);}   
   now /= 10;
  }
  FastLED.show(); // Display leds array
  delay(100);
 }
}

void Backstage (uint16_t nowTime) { // функція ,,за лаштунками,,   
 for (bias=0; bias<NUM_LEDS-HEIGHT; bias+=HEIGHT){ 
  uint16_t now = nowTime;
  uint8_t dSecund = 2;   //приблизно 2 секунди триває анімація 
  LedsFon(bias, bias+16);       
  for (uint8_t i=1; i<=2; i++) {                     //друкуємо секунди
   uint8_t digitSecund = dSecund % 10;               //остача від ділення на 10
    if (i==1) {BackstageOfTime (224, digitSecund);}  //створюєм символ сесунди першого порядку
    else {BackstageTime (184, digitSecund);}         //створюєм символ сесунди другого порядку 
   dSecund /= 10;
  }
  for (uint8_t i=1; i<=4; i++) {   
   uint16_t digit = now % 10; 
    if (i==1) {BackstageTime(136, digit);}
    else if (i==2) {BackstageOfTime(96, digit);}
    else if (i==3) {BackstageTime(40, digit);}
    else {BackstageOfTime(0, digit);}   
   now /= 10;
  }
  FastLED.show(); // Display leds array
  delay(25); 
 }
 bias = 0;
}

void BackstageTime (int16_t serialNumber, uint8_t digit) {  
 for(int8_t j=0; j<4; j++) { 
  int8_t f = HEIGHT*j; 
  if (bias == serialNumber + f) {
   int16_t cursor = serialNumber + f;
   for (int8_t k=f; k<f+HEIGHT; k++) { 
    if (digitsPeir[digit][k]== 1) {leds[cursor]=ledColor;}
    else {leds[cursor]=dawnColor;}
    cursor ++;
   }
  }
 }   
}

void BackstageOfTime (int16_t serialNumber, uint16_t digit) {  
 for (int8_t j=0; j<4; j++) { 
  int8_t f = HEIGHT*j;
  if (bias == serialNumber + f) {
   int16_t cursor = serialNumber + f; 
   for (int8_t k=f; k<f+HEIGHT; k++) { 
    if (digits[digit][k]== 1) {leds[cursor]=ledColor;}
    else {leds[cursor]=dawnColor;}
    cursor ++;
   }
  }
 }   
}

void FallingOf (int16_t serialNumber, uint8_t digit) {  
 for (uint8_t j=0; j<4; j++) { 
  int16_t cursor;
  int8_t f = HEIGHT*j; 
  if (j%2==0) { 
   cursor = serialNumber + bias + f + HEIGHT-1 - line;
   if (cursor<NUM_LEDS) {
    for (int8_t k=f ; k<=f+line; k++) { 
     if (digitsPeir[digit][k]== 1) {leds[cursor]=ledColor;}
     else {leds[cursor]=dawnColor;}
     if (cursor<NUM_LEDS) {cursor ++;}
    }
   }
  }
  else {
   cursor = serialNumber + bias + f ;
   if (cursor<NUM_LEDS) {
    for (int8_t k=f+HEIGHT-1-line; k<f+HEIGHT; k++) {     
     if (digitsPeir[digit][k]== 1) {leds[cursor]=ledColor;}
     else {leds[cursor]=dawnColor;}
     if (cursor<NUM_LEDS) {cursor ++;}
    }
   }
  }
 }   
}

void Falling (int16_t serialNumber, uint8_t digit) {  
 for (uint8_t j=0; j<4; j++) {        // по черзі назначаємо індекс кожному стовбчику 
  int16_t cursor;
  int8_t f = HEIGHT*j;                //перемножуємо кількістьпікселей по висоті на індекс
  if (j%2==0) {                       // якщо стовбчик отримав парний індекс
   cursor = serialNumber + bias + f ; // розміщаємо курсор на піксель з порядковим номером 
   if (cursor<NUM_LEDS) {
    for (int8_t k=f+HEIGHT-1-line; k<f+HEIGHT; k++) { 
     if (digits[digit][k]== 1) {leds[cursor]=ledColor;}
     else {leds[cursor]=dawnColor;}
     if (cursor<NUM_LEDS) {cursor ++;}
    }
   }
  }
  else {
   cursor = serialNumber + bias + f + HEIGHT-1 - line;
   if (cursor<NUM_LEDS) {
    
    for (int8_t k=f ; k<=f+line; k++) { 
      
     if (digits[digit][k]== 1) {leds[cursor]=ledColor;}
     else {leds[cursor]=dawnColor;}
     if (cursor<NUM_LEDS) {cursor ++;}
    }
   }
  }
 }   
}

void CurtainLine (int16_t serialNumber, uint8_t digitLine) {  
 for (uint8_t j=0; j<4;j++) { 
  int16_t cursor;
  int8_t f = HEIGHT*j; 
  if (j%2==0){ 
   cursor = serialNumber + f + line;
   for (int8_t k=f+line; k<f+HEIGHT ; k++) {
    if (digits[digitLine][k]== 1) {leds[cursor]=ledColor;}
    else {leds[cursor]=dawnColor;}
    cursor ++;
   }
  }
  else {
   cursor = serialNumber + f ; 
   for (int8_t k=f; k<f+HEIGHT-line; k++) {   
    if (digits[digitLine][k]== 1) {leds[cursor]=ledColor;}
    else {leds[cursor]=dawnColor;}
    cursor ++;
   }
  }
 }   
}

void CurtainOfLine (int16_t serialNumber, uint8_t digitLine) {  
 for (uint8_t j=0; j<4;j++) { 
  int16_t cursor;
  int8_t f = HEIGHT*j; 
  if (j%2==0){ 
   cursor = serialNumber + f ;
   for (int8_t k=f; k<f+HEIGHT-line; k++) { 
    if (digitsPeir[digitLine][k]== 1) {leds[cursor]=ledColor;}
    else {leds[cursor]=dawnColor;}
    cursor ++;
   }
  }
  else {
   cursor = serialNumber + f + line; 
   for (int8_t k=f+line; k<f+HEIGHT ; k++) {  
    if (digitsPeir[digitLine][k]== 1) {leds[cursor]=ledColor;}
    else {leds[cursor]=dawnColor;}
    cursor ++;
   }
  }
 }   
}

void PairOfdigits (int16_t serialNumber, uint16_t digit) { 
 int16_t cursor;
 if (serialNumber + bias < NUM_LEDS) {
  cursor = serialNumber+bias; 
  for (int8_t j=0; j<4; j++) { // кількість циклів дорівнює кількості стовпчиків формованої цифри
   int8_t f = 8*j; 
   for (int8_t k =f; k<f+8; k++) { 
    if (digits[digit][k]== 1) {leds[cursor]=ledColor;}
    else {leds[cursor]=dawnColor;}
    if (cursor<NUM_LEDS) {cursor ++;}
   }
  }
 }   
}

void Pairdigits (int16_t serialNumber, uint16_t digit) { 
 int16_t cursor;
 if (serialNumber + bias < NUM_LEDS) {
  cursor = serialNumber+bias; 
  for (int8_t j=0; j<4; j++) { 
   int8_t f = 8*j; 
   for (int8_t k =f; k<f+8; k++) { 
    if (digitsPeir[digit][k]== 1) {leds[cursor]=ledColor;}
    else {leds[cursor]=dawnColor;}
    if (cursor<NUM_LEDS) {cursor ++;}
   }
  }
 }   
}
