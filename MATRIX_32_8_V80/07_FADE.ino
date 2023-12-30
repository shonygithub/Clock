static byte hue;

void FadeFonction() {   // вибір анімації
 static uint8_t VAR_animations  = 1;  //для вибору варіанта анімації
 if (VAR_animations == 9) {VAR_animations = 1;}  
 switch(VAR_animations) {
  case 1: Right();                   break;
  case 2: Up();                      break; 
  case 3: FromTheCenterHorizontal(); break;
  case 4: ToTheCenterVertical();     break;
  case 5: Left();                    break;
  case 6: Down();                    break;
  case 7: ToTheCenterHorizontal();   break;
  case 8: FromTheCenterVertical();   break;
 }
 VAR_animations ++;   
}

void fadeall() {     //   притухання кольорів
 for (uint16_t m=0; m<NUM_LEDS; m++) { 
leds[m].nscale8(250); 
 } 
}

void Right() {
 for (int16_t i=0; i<NUM_LEDS; i+=HEIGHT) {
  hue+=2; 
  for (int16_t a=i; a<HEIGHT+i; a++) {leds[a] = CHSV(hue, 255, 255);}
  FastLED.show();
  fadeall();
  delay(20);
 }
 delay(500);
}

void Left() {
 for (int16_t i= NUM_LEDS - HEIGHT; i>=0; i -= HEIGHT) {
  hue+=2;
  for (int16_t a = i; a < HEIGHT+i; a ++ ) {leds[a] = CHSV(hue, 255, 255);}
  FastLED.show();
  fadeall(); 
  delay(20);
 }
 delay(500);
}

void Down() {
 for (int8_t i=0; i<HEIGHT; i++) {
  hue+=5;
  for (int16_t a =i; a<(NUM_LEDS-HEIGHT); a+=HEIGHT*2 ) {
   leds[a] = CHSV(hue, 255, 255);
   int16_t b = (a+HEIGHT*2-(2*i+1)); 
   leds[b] = CHSV(hue, 255, 255);
  }
  FastLED.show();
  fadeall();
  delay(100);
 }
 delay(500);
}

void Up() {
 for (int8_t i=HEIGHT-1; i>=0; i--) {
  hue+=5;
  for (int16_t a=i; a<(NUM_LEDS-HEIGHT); a+=HEIGHT*2) {
   leds[a] = CHSV(hue, 255, 255);
   int16_t b = (a+HEIGHT*2-(2*i+1)); 
   leds[b] = CHSV(hue, 255, 255); 
  }
  FastLED.show();
  fadeall(); 
  delay(100);
 }
 delay(500); 
}

void ToTheCenterHorizontal() {
 for (int16_t i=0; i<(NUM_LEDS/2); i+=HEIGHT) {
  hue+=4;
  for (int16_t a=i; a<=HEIGHT-1+i; a++) {
   leds[a] = CHSV(hue, 255, 255);
   int16_t b = (NUM_LEDS-a)-1;  
   leds[b] = CHSV(hue, 255, 255); 
  }
  FastLED.show();
  fadeall();
  delay(60);
 }
 delay(500);
}

void FromTheCenterHorizontal() {
 for (int16_t i=(NUM_LEDS/2-HEIGHT); i>=0; i-=HEIGHT) {
  hue+=4; 
  for( int16_t a=i; a<=HEIGHT-1+i; a++ ) {
   leds[a] = CHSV(hue, 255, 255);
   int16_t b = (NUM_LEDS-a)-1; 
   leds[b] = CHSV(hue, 255, 255); 
  }
  FastLED.show();
  fadeall();
  delay(60);
 }
 delay(500);
}

void ToTheCenterVertical() {
 for (int8_t i=0; i<HEIGHT/2; i++) {
  hue+=10; 
  for (int16_t a=i; a<(NUM_LEDS-HEIGHT); a+=HEIGHT*2) {
   leds[a] = CHSV(hue, 255, 255);
   int16_t b = (a+HEIGHT*2-(2*i+1)); 
   leds[b] = CHSV(hue, 255, 255);
   int16_t c = a+HEIGHT-1-i*2;  
   leds[c] = CHSV(hue, 255, 255);
   int16_t d = (a+HEIGHT); 
   leds[d] = CHSV(hue, 255, 255); 
  } 
  FastLED.show();
  fadeall();
  delay(200);
 }
 delay(500);
} 
 
void FromTheCenterVertical() {
 for (int8_t i=HEIGHT/2-1; i>=0; i--) {
  hue+=10; 
  for (int16_t a=i; a<(NUM_LEDS-HEIGHT); a+=HEIGHT*2) {
   leds[a] = CHSV(hue, 255, 255);
   int16_t b = (a+HEIGHT*2-(2*i+1)); 
   leds[b] = CHSV(hue, 255, 255);
   int16_t c = a+HEIGHT-1-i*2; 
   leds[c] = CHSV(hue, 255, 255);
   int16_t d = (a+HEIGHT); 
   leds[d] = CHSV(hue, 255, 255); 
  } 
  FastLED.show(); 
  fadeall();
  delay(200);
 }
 delay(500);
}
