
void setup() {
 Wire.begin(4,5);             //підключаємось до шини І2С у статусі МАЙСТЕР 
 delay(1000); 
 sensors.begin(); 
 Serial.begin(9600);
 mp3_set_serial(Serial);   //set Serial for DFPlayer-mini mp3 module     
 udp.begin(localPort);
 pinMode(MOTION_SENSOR_PIN, INPUT); // определяем pin как вход
 FastLED.addLeds<WS2812, NEW_PIXEL_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
 photoresistorValue = map(( analogRead(ANALOG_PIN)), 0, 1023, brightnessMin, brightnessMax); // обмеження (коридор) яскравості
 FastLED.setBrightness(constrain(photoresistorValue, brightnessMin, brightnessMax));
 if (CURRENT_LIMIT > 0) {FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);}
 WiFi.softAP(APssid, password);
 IPAddress myIP = WiFi.softAPIP();
 server.begin();
 server.on("/", HandleClient);
 server.on("/set_clock_end_sensors", SetTime);
 server.on("/set_alarm_clock", SetAlarmClock);
 server.on("/set_WI_FI", handleRoot);
 server.on("/ok", handleOk);
 server.on("/write_congratulation", StringWrite);
 server.on("/string_ok_1", StringCell_1);
 server.on("/string_ok_2", StringCell_2);
 server.on("/string_ok_3", StringCell_3);
 server.on("/string_ok_4", StringCell_4);
 server.on("/string_ok_5", StringCell_5);
 server.on("/string_ok_6", StringCell_6);
 server.on("/string_ok_7", StringCell_7);
 server.on("/string_ok_8", StringCell_8);
}
