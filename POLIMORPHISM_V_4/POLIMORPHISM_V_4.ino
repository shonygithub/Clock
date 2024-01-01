
class Figure {
 public :
 const float Pi = 3.14 ;
 virtual float calculate(){
  return 0;
 }
 virtual void print () {
  Serial.println("Фігура не створена!!!"); 
 }
};

class Circle : public Figure {
 public :
 float diameter ;
  Circle (float diameter) {
   this-> diameter = diameter;
  }
  float calculate() override {
   float r = diameter/2 ; 
   return Pi * (r*r);
  }
  void print () override {
  Serial.print("Площа круга : ");
  Serial.println(calculate()); 
 }
};

class Ball : public Figure {
 public :
 float diameter ;
  Ball (float diameter) {
   this-> diameter = diameter;
  }
  float calculate() override {
   float r = diameter/2 ;
   return 4 * r * r * r * Pi / 3;
  }
  void print () override {
  Serial.print("Об'єм кулі: ");
  Serial.println(calculate()); 
 }
};

class Cylinder : public Circle {
 public :
  float diameter;
  float height;
  Cylinder (float diameter_, float _height) : Circle ( diameter_ ) {// в конструкторі створюєио об'єкт проміжного класу Circle ...
   this-> diameter = diameter_;   //...класу Circle  щоб скористатись саме його методом calculate...
   this-> height = _height;       //...методом calculate , який обчислює площу круга (основу циліндра)
  }
  float calculate() override { 
   return Circle :: calculate() * height;
  }
  void print () override {
  Serial.print("Об'єм циліндра: ");
  Serial.println(calculate()); 
 }
};

class Cone : public Cylinder  {
 public :
  float diameter; 
  float height;
  Cone (float d_, float v_)  : Cylinder ( d_, v_ ) { // в конструкторі створюєио об'єкт проміжного класу Cylinder ...
   this-> diameter = d_;    //...класу Cylinder  щоб скористатись саме його методом calculate...
   this-> height = v_;      //...методом calculate , який обчислює об'єм циліндра
  }
  float calculate() override{
   float obyem = Cylinder :: calculate() / 3 ; // використовуємо метод класу Cylinder
   return obyem;
  }
 void print () override {
  Serial.print("Об'єм конуса: ");
  Serial.println(calculate()); 
 }
};

 class Calculator {
  public : 
  void  calculateFigure (Figure * miFigure) {
    miFigure->print () ;
  }
 };

void setup() {
 Serial.begin (115200);
 Serial.println("Старт програми ");
 Figure miFigure ;
 Ball smollBall (1.0);
 Ball bigBall (5.0 ); 
 Circle smollCircle (1.0);
 Circle bigCircle (5.0 );
 Cylinder smollCylinder (1.0 , 1.0);
 Cylinder bigCylinder (5.0 , 10.0);
 Cone smollCone (1.0 , 1.0);
 Cone bigCone (5.0 , 10.0);            // створений об'єкт (укземпляр класу)
 Figure * f_k = new Cone(5.0, 10.0);   // МОЖНА І ТАК  джаваподібний спосіб
 Calculator figCal ;
 Figure & rFigure = bigCone;
 
 figCal.calculateFigure (& miFigure);
 figCal.calculateFigure (& smollBall);
 figCal.calculateFigure (& bigBall);
 figCal.calculateFigure (& smollCircle);
 figCal.calculateFigure (& bigCircle);
 figCal.calculateFigure (& smollCylinder);
 figCal.calculateFigure (& bigCylinder);
 figCal.calculateFigure (& smollCone);
 figCal.calculateFigure (& bigCone);   // в параметр передається посилання на об'єкт
 figCal.calculateFigure ( f_k );       // МОЖНА І ТАК , в параметр передається вказівник на обєкт
 figCal.calculateFigure ( & rFigure );
}

void loop() {
  // put your main code here, to run repeatedly:

}
