
class Figura {
 public :
 float diametr;
 float vusota;
 const float Pi = 3.14 ;
 Figura(float diametr, float vusota) {
  this-> diametr = diametr;
  this-> vusota = vusota;
 }
 virtual float ObchuslutuObyembaba(){
  float obyem =  (Pi * ((diametr / 2)*(diametr / 2)))* vusota;
  return obyem;
 }
 virtual void print () {
  Serial.print("Об'єм фігури: ");
  Serial.println(ObchuslutuObyembaba()); 
 }
};

class Shar : public Figura {
 public :
  float diametr;
  Shar (float diametr)  : Figura ( diametr, vusota ){
   this-> diametr = diametr;
  }
  float ObchuslutuObyembaba() {
   float r = diametr/2 ;
   return 4 * r * r * r * Figura :: Pi / 3;
  }
  void print () {
  Serial.print("Об'єм кулі: ");
  Serial.println(ObchuslutuObyembaba()); 
 }
};

class Chulindr : public Figura {
 public :
  float diametr;
  float vusota;
  Chulindr (float diametr, float vusota)  : Figura ( diametr, vusota ){
   this-> diametr = diametr;
   this-> vusota = vusota;
  }
  float ObchuslutuObyembaba() {
   float r = diametr/2 ; 
   return (Figura :: Pi * (r*r)) * vusota;
  }
  void print () {
  Serial.print("Об'єм циліндра: ");
  Serial.println(ObchuslutuObyembaba()); 
 }
};

class Konys : public Chulindr  {
 public :
  float diametr; 
  float vusota;
  Konys (float diametr, float vusota)  : Chulindr ( diametr, vusota ) {
   this-> diametr = diametr;
   this-> vusota = vusota;
  }
  float ObchuslutuObyembaba() {
   float obyem = (1.0 / 3.0) * Chulindr :: ObchuslutuObyembaba() ;
   return obyem;
  }
 void print () {
  Serial.print("Об'єм конуса: ");
  Serial.println(ObchuslutuObyembaba()); 
 }
};

void setup() {
 Serial.begin (115200);
 Serial.println("Старт програми ");
 Figura * f = new Figura(4.0, 5.0);
 Chulindr * c = new Chulindr(10.0, 5.0);
 Shar * s = new Shar(1.0);
 Konys * k = new Konys(10.0, 5.0);
 Figura * f_k = new Konys(4.0, 5.0);
 Figura * f_c = new Chulindr(4.0, 5.0);
 Figura * f_s = new Shar(5.0);

 f-> print ();
 c-> print ();
 k-> print();
 s-> print();
 f_c-> print();
 f_k-> print();
 f_s-> print();
 Serial.println(f_s->Pi);
 
 delete f_k ;
 delete f_c ;
 delete k ;
 delete c ;
 delete f ;
}

void loop() {
  // put your main code here, to run repeatedly:

}
