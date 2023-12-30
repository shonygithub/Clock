 static bool minus;
 static bool plus;
void BrightnessCheck() {         //функція авто-регулювання яскравості відносно освітлення.
 photoresistorValue = map((analogRead(ANALOG_PIN)), 0, 1023, brightnessMin, brightnessMax); //конвертуєм у діапазон макс і мін  яскравості
 photoresistorValue = constrain(photoresistorValue,brightnessMin,brightnessMax);             // піддверджуєм діапазон
 if (nightMode) {
  if ( photoresistorValue<brightness-2){minus=true;}//якщо освітлення у кімнаті зменшилось 
  if ( photoresistorValue>=brightness){minus=false;}//якщо яскравість дисплею вже стала більша
  if ( photoresistorValue>brightness+2){plus=true;} //якщо освітлення у кімнаті збільшилось
  if ( photoresistorValue<=brightness){plus=false;} //якщо яскравість дисплею вже стала меньша
  if (minus){
   brightness--; //зменьшуєм яскравість
   znachennya = 1000/(1+abs (photoresistorValue-brightness)); //встановлюєм інтервал виклику функції, визначивши модуль різниці
  }
  if (plus){
   brightness++; //збільшуєм яскравість
   znachennya=1000/(1+abs (photoresistorValue-brightness));
  }
  if (!dawnIsWorking) {LEDS.setBrightness(brightness);}
 }
}
