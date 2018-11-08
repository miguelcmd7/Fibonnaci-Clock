#include <ArduinoSTL.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <time.h>

LiquidCrystal_I2C lcd(0x27,20,4); //
using namespace std;

// connect to the ST_CP of 74HC595 (pin 3,latch pin)
int latchPin = 8;
// connect to the SH_CP of 74HC595 (pin 4, clock pin)
int clockPin = 12;
// connect to the DS of 74HC595 (pin 2)
int dataPin = 11;
const int hourButton = 2;
const int minuteButton = 3;
const int switchlcd = 9;


//Colors

const String GREEN = "001";
const String RED = "010";
const String BLUE = "100";
const String WHITE = "111";

int hour = 5;
int minute = 10;
int hourButtonState = 0;
int minuteButtonState = 0;
int timesWithoutChanges = 0;
std::vector<std::vector<int> > hourPosi; 
std::vector<std::vector<int> > minutesPosi; 
std::vector<int> hourCount;
std::vector<int> minuteCount;
int hora = 5;
int minutes = 10;
int Hpos = 0;
int Mpos=0;
vector<int> fibHour;
vector<int> fibMinute;
String binary;


int string_to_int(String binary){
  
  int total = 0;
  for (int i=0; i<binary.length(); i++){
    total *= 2; 
    if (binary.charAt(i) == '1') total++;  
  }
  return total;
}

String createString( vector<int> fibHour,vector<int> fibMinute){
  String result= "";
  vector<int>::iterator startH = fibHour.begin(); 
  vector<int>::iterator startM = fibMinute.begin();
  int fibArray[] ={1,1,2,3,5};
  bool setHour =false;
  bool setMinute= false;
  for (int i=0;i<5;i++){
    if (startH!= fibHour.end() && fibArray[i]==*startH){
      setHour =true;
      startH++;
      }
    if (startM!= fibMinute.end() && fibArray[i]==*startM){
      setMinute =true;
      startM++;
      }

    if (setHour && !setMinute){
      result=result+RED;
      }
    if (!setHour && setMinute){
      result=result+GREEN;
      }
    if (setHour && setMinute){
      result=result+BLUE;
      }
    if (!setHour && !setMinute){
      result=result+WHITE;
      }
    setHour=false;
    setMinute=false;
  }
  return result;
  
  
}


int fibonnaci(int* legth, int* ant, int* sig, int* count) {
  *ant += *sig;
  (*count)++;
  if ((*count) < *legth)
    return fibonnaci(legth, sig, ant, count);
  return (*sig);


}

int fibPos(int legth) {
  int ant = 0, sig = 1, count = 0;
  return fibonnaci(&legth, &ant, &sig, &count);
}
int sum(std::vector<int> *element) {
  int acu = 0;

  for (std::vector<int>::iterator it = element->begin(); it != element->end(); ++it) {
    acu += (*it);
  }
  return acu;

}
void posibilities(std::vector<std::vector<int> > *posibles, std::vector<int> *v, int* valor, int pos) {
  int sum_of_elems = sum(v);
  int x = pos + 1;
  if (sum_of_elems < *valor) {
    while (x <= 5) {
      if (sum_of_elems + fibPos(x) <= (*valor)) {
        (*v).push_back(fibPos(x));
        posibilities(posibles, v, valor, x);
        (*v).pop_back();
      }
      x++;
    }

  } else {
    if (sum_of_elems == *valor) {
      std::vector<int> nuevo = (*v);
      (*posibles).push_back(nuevo);

    }
  }
}
void show(std::vector<std::vector<int> > *posibles) {
  for (std::vector<std::vector<int> >::iterator it = posibles->begin(); it != posibles->end(); ++it) {
    std::cout << "[";
    for (std::vector<int>::iterator it2 = it->begin(); it2 != it->end(); ++it2) {
      std::cout << ", " << (*it2);
    }
    std::cout << "], " ;

  }
  std::cout << '\n';

}
void showElection(std::vector<int> *election,String text){
  std::cout<< '\n';
  Serial.print(text);
  std::cout<< '\n';

  for (std::vector<int>::iterator it2 = election->begin(); it2 != election->end(); ++it2) {
      std::cout << ", " << (*it2);
    }
  std::cout << '\n';

  }


void showTime(int hour, int minute) {
  lcd.setCursor(0, 0);
  // Escribimos el número de segundos trascurridos
  lcd.print("Hour: ");
  lcd.print(hour);
  lcd.setCursor(0, 1);
  lcd.print("Minutes: ");
  lcd.print(minute);
}


// display a number on the digital segment display
void sevenSegWrite( String binary) {
  // set the latchPin to low potential, before sending data
  
  String sub1= binary.substring(0,8);
  String sub2 = binary.substring(8);
  Serial.println(binary);
  Serial.println(sub1+sub2);
  Serial.println(sub1);
  Serial.println(sub2);
  digitalWrite(latchPin, LOW);

  shiftOut(dataPin, clockPin, LSBFIRST, string_to_int(sub2+"0"));
  shiftOut(dataPin, clockPin, LSBFIRST, string_to_int(sub1));
  // set the latchPin to high potential, after sending data
  digitalWrite(latchPin, HIGH);
}

void setup() {
  srand (time(NULL));

  Serial.begin(9600);
  Serial.print(String( uint8_t("0110011")));
  //Serial.print(B11111100);
  // Set latchPin, clockPin, dataPin as output
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(switchlcd, OUTPUT);
  pinMode(hourButton, INPUT);
  pinMode(minuteButton, INPUT);
  //sevenSegWrite();
  std::vector<std::vector<int> > hourPosi; 
  std::vector<std::vector<int> > minutesPosi; 
  std::vector<int> hourCount;
  std::vector<int> minuteCount;
  int hora = 5;
  int minutes = 10;
  int Hpos = 0;
  int Mpos=0;
  vector<int> fibHour;
  vector<int> fibMinute;
  String binary;
  
  minutes=10/5;
  /*posibilities(&hourPosi, &hourCount, &hora, Hpos);
  posibilities(&minutesPosi, &minuteCount, &minutes, Mpos);
  int numberH =rand()%(hourPosi.size());
  int numberM =rand()%(minutesPosi.size());
  Serial.println("Number H:"+String(numberH));
  Serial.println("Number M:"+String(numberM));
  fibHour=hourPosi[numberH];
  fibMinute= minutesPosi[numberM];
  showElection(&fibHour,"Hours:");
  showElection(&fibMinute,"Minutes:");
  binary=createString(fibHour,fibMinute);
  sevenSegWrite(binary);
  Serial.println("Resultado "+ binary);
  Serial.println(String(string_to_int(binary)));
  show(&hourPosi);
  show(&minutesPosi);*/
  
  digitalWrite(switchlcd, HIGH);
  lcd.init();

  //Encender la luz de fondo.
  lcd.backlight();
  //Serial.println("Mostrando Hora");
  //lcd.setCursor(0, 0);
  //lcd.print("Hour: ");
  // Escribimos el Mensaje en el LCD.

}

void loop() {

  // put your main code here, to run repeatedly:
  hourButtonState = digitalRead(hourButton);
  minuteButtonState = digitalRead(minuteButton);
  int timedelay = 300;

  while (hourButtonState == HIGH || minuteButtonState == HIGH ) {
    if (timesWithoutChanges > 20) {
      digitalWrite(switchlcd, HIGH);
      lcd.init();
      lcd.backlight();
      timesWithoutChanges = 0;
      break;
    }
    if (hourButtonState == HIGH) {
      if (hour < 23) {
        hour = hour + 1;
      } else {
        hour = 0;
      }
    }
    if (minuteButtonState == HIGH) {
      if (minute < 59) {
        minute = minute + 1;
      } else {
        minute = 0;
      }
    }

    lcd.clear();
    showTime(hour, minute);
    if (timedelay > 100) {
      timedelay = timedelay - 10;
    }
    delay(timedelay);
    hourButtonState = digitalRead(hourButton);
    minuteButtonState = digitalRead(minuteButton);
    timesWithoutChanges = 0;
  }
  timesWithoutChanges = timesWithoutChanges + 1;
  if (timesWithoutChanges > 20) {
    digitalWrite(switchlcd, LOW);
  }
  showTime(hour, minute);
  hourPosi.clear();
  hourCount.clear();
  hourCount.clear();
  minuteCount.clear();
  Hpos=0;
  Mpos=0;

  minute=minute/5;
  posibilities(&hourPosi, &hourCount, &hour, Hpos);
  posibilities(&minutesPosi, &minuteCount, &minute, Mpos);
  int numberH =rand()%(hourPosi.size());
  int numberM =rand()%(minutesPosi.size());
  Serial.println("Number H:"+String(numberH));
  Serial.println("Number M:"+String(numberM));
  fibHour=hourPosi.size()>0 ? hourPosi[numberH]: vector<int>(1,0);
  fibMinute= minutesPosi.size()>0 ? minutesPosi[numberM]: vector<int>(1,0);
  showElection(&fibHour,"Hours:");
  showElection(&fibMinute,"Minutes:");
  binary=createString(fibHour,fibMinute);
  sevenSegWrite(binary);
  
  
  digitalWrite(latchPin, HIGH);
  
  
  delay(200);
}
