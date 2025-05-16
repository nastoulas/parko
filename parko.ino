#include <Adafruit_GFX.h>         // Βιβλιοθήκη γραφικών
#include <Adafruit_ST7735.h>      // Βιβλιοθήκη ST7735
#include <SPI.h>                  // Βιβλιοθήκη σειριακής επικοινωνίας
#include <SoftwareSerial.h>       // Βιβλιοθήκη σειριακής επικοινωνίας 
#include <DFRobotDFPlayerMini.h>  // Βιβλιοθήκη player mp3 
#include <DHT11.h>                // Βιβλιοθήκη αισθητήρα θερμοκρασίας και υγρασίας (dht11) 

//Παραμετροποίηση οθόνης
#define TFT_CS         9
#define TFT_RST        8 
#define TFT_DC         7
#define TFT_MOSI       5  
#define TFT_SCLK       6  
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
#define ST7735_BLACK ST77XX_BLACK
#define ST7735_WHITE ST77XX_WHITE
#define ST7735_RED ST77XX_RED
#define ST7735_GREEN ST77XX_GREEN
#define ST7735_BLUE ST77XX_BLUE
#define ST7735_CYAN ST77XX_CYAN
#define ST7735_MAGENTA ST77XX_MAGENTA
#define ST7735_YELLOW ST77XX_YELLOW
#define ST7735_ORANGE ST77XX_ORANGE

SoftwareSerial softwareSerial(11, 10);  // Ορισμός εισόδου-εξόδου player mp3 10-ΤΧ, 11-RΧ 
DFRobotDFPlayerMini player;             // Ορισμός αντικειμένου player mp3 
DHT11 dht11(2);                         // Ορισμός αντικειμένου καταγραφής Θερμοκρασίας και Υγρασίας από την είσοδο 2 

const int pinLights = 3;                // Διαχείριση φώτων πάρκου
const int pinWaters = 4;                // Διαχείριση συστήματος ποτίσματος

const int pinSensorUV = A0;             // Αισθητήρας υπεριώδους ακτινοβολίας
const int pinMoisture = A1;             // Αισθητήρας υγρασίας χώματος
const int pinBrightness = A2;           // Αισθητήρας φωτεινότητας εξωτερικού χώρου
const int pinMusic  = A3;               // Κουμπί μουσικής

float outTemperature;               // Τιμή Θερμοκρασίας
float outHumidity;                  // Τιμή Υγρασίας
float outBrightness;                // Τιμή Φωτεινότητας
float outMoisture;                  // Τιμή Υγρασίας εδάφους
int outUVindex;                     // Τιμή Δείκτη υπεριώδους ακτινοβολίας


void setup () {
  tft.initR(INITR_BLACKTAB);      // ενεργοποίηση οθόνης
  tft.fillScreen(ST7735_BLACK);

  pinMode(LED_BUILTIN, OUTPUT); 
  digitalWrite(LED_BUILTIN, LOW); // σβήσιμο εσωτερικού led 
  
  pinMode (pinBrightness, INPUT); 
  pinMode (pinMoisture, INPUT); 
  pinMode (pinSensorUV, INPUT); 
  pinMode (pinMusic, INPUT); 
  pinMode (pinLights, OUTPUT); 
  pinMode (pinWaters, OUTPUT); 

  Serial.begin(57600);
  softwareSerial.begin (9600);
  
  if (player.begin(softwareSerial)) player.volume(60);  //καθορισμός έντασης ήχου
}

void loop () {
  Lights();
  Waters();
  EnviromentData();
  Ultraviolet();
  DisplayData();
  Music();
  delay(2000);
}

void Music () {                                   // Διαχείριση μουσικής στο πάρκο
 if (analogRead(pinMusic) < 50 ) player.play(1);
 Serial.println(analogRead(pinMusic));
}

void Waters () {                                  // Διαχείριση νερών πάρκου 
    outMoisture = analogRead(pinMoisture); 
    if (outMoisture < 700) { 
      digitalWrite(pinWaters, HIGH); 
    } 
    else { 
      digitalWrite(pinWaters, LOW); 
    } 
  } 
void Lights () {                                  // Διαχείριση φωτισμού πάρκου 
    outBrightness = analogRead(pinBrightness); 
    if (outBrightness < 400) { 
      digitalWrite(pinLights, HIGH); 
    } 
    else { 
      digitalWrite(pinLights, LOW); 
    } 
  } 

void EnviromentData () {                           // Καταγραφή περιβαλλοντικών ενδείξεων (Θερμοκρασία & Υγρασία)
  outTemperature = dht11.readTemperature(); 
  outHumidity = dht11.readHumidity(); 
}

void Ultraviolet () {                             // Καταγραφή υπεριώδους ακτινοβολίας
  outUVindex = analogRead(pinSensorUV); 
}

void DisplayData() {                              // Εμφάνιση Δεδομένων
  //tft.setRotation(tft.getRotation()+1);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_YELLOW);
  tft.setCursor(5, 5);
  tft.println("TEMPERATURE   : ");
  tft.setCursor(5, 20);  
  tft.println("HUMIDITY    : ");
  tft.setCursor(5, 35);  
  tft.println("MOISTURE    : ");
  tft.setCursor(5, 50);  
  tft.println("BRIGHTNESS : ");
  tft.setCursor(5, 65);  
  tft.println("ULTRAVIOLET : ");


  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(85, 5);
  tft.println(outTemperature);
  tft.setCursor(85, 20);  
  tft.println(outHumidity);
  tft.setCursor(85, 35);  
  tft.println(outMoisture);
  tft.setCursor(85, 50);  
  tft.println(outBrightness);
  tft.setCursor(85, 65);  
  tft.println(outUVindex);

  if  (outUVindex >= 0 && outUVindex <= 7) {
    tft.setTextColor(ST77XX_GREEN);
    tft.setTextSize(2);    
    tft.setCursor(5, 85);  
    tft.println("UV Normal");   
  }
  else if  (outUVindex > 7 && outUVindex <= 9) {
    tft.setTextColor(ST77XX_ORANGE);
    tft.setTextSize(2);    
    tft.setCursor(5, 85);  
    tft.println("UV Attention");
  }
  else {
    tft.setTextColor(ST77XX_RED);
    tft.setTextSize(2);  
    tft.setCursor(5, 85);  
    tft.println("UV Danger");
    player.play(2);
  }
}