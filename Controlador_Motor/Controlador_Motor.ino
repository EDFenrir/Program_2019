/*This code is used to ontrol the first microcontroller, the one that takes care of the sensors and the 
  PWM signal creation.   */

#include <OneWire.h>
#include <DallasTemperature.h>
#include <U8g2lib.h>
#include <SPI.h>

U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, 4, 2, 0, 16); // Enable=6, RW=data=5, RS=4, Rst=17

OneWire pin(39);
DallasTemperature bus(&pin);
DeviceAddress sensor;
//variables responsible for the rpm measure
volatile byte rpmcount;

unsigned int rpm = 0;
unsigned int rpm2 = 0;

unsigned long timeold = 0;

// the number of the LED pin
const int ledPin = 17;  // 16 corresponds to GPIO16

#define Potpin  34 // Defines Potpin as pin 34

// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;
int PotValue = 0;      
int PWMValue = 0;
int temp = 0;   
 
//variaveis que indicam o núcleo
static uint8_t taskCoreZero = 0;
static uint8_t taskCoreOne  = 1;


void IRAM_ATTR rpm_fun()
{
  rpmcount++;
  Serial.println(rpmcount);
  //Each rotation, this interrupt function is run twice
}

void setup(){

  //hall sensor  
  pinMode(15, INPUT_PULLUP);
  //Interruption sensor attached to pin 15, that stops the program in case rising edge detection
  attachInterrupt(15, rpm_fun, RISING);

  u8g2.begin();

  Serial.begin(115200);

  bus.begin();
  bus.getAddress(sensor, 0);
  
   // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin, ledChannel);
  
  //coreTaskZero: used for the temperature sensor
  xTaskCreate(
                    coreTaskZero,   // function that implements the task
                    "coreTaskZero", //task name
                    10000,          //number of words alocated for the task stack      
                    NULL,           //default input task
                    2,              //task priority
                    NULL);       //task reference
                    
  delay(500); // delay to begin the next task

  //coreTaskOne: used for reading the potentiometer 
  xTaskCreate(
                    coreTaskOne,   
                    "coreTaskOne", 
                    10000,      
                    NULL,       
                    3,          
                    NULL);       

    delay(500); 
     //coreTaskTwo: used for update the display data
     xTaskCreate(
                    coreTaskTwo,   
                    "coreTaskTwo", 
                    10000,      
                    NULL,       
                    2,          
                    NULL);      

    delay(500); 
    
    //coreTaskThree: speed sensor 
     xTaskCreate(
                    coreTaskThree,   
                    "coreTaskThree", 
                    10000,      
                    NULL,       
                    2,          
                    NULL);       

    delay(500); 
  
 
}
 
void loop(){
}

void coreTaskZero( void * pvParameters ){
    while(true){
    bus.requestTemperatures(); 
    temp = bus.getTempC(sensor); //read temperature
    delay(500);                    
    }
}

void coreTaskOne( void * pvParameters ){
    while(true){
    PotValue = analogRead(Potpin);// Make the reading of the ADC converter
    PWMValue = map(PotValue, 0, 4095, 0, 255); //map the potentiometer
    ledcWrite(ledChannel, PWMValue); //writes the PWM signal
    delay(100);
    }
}

void coreTaskTwo( void * pvParameters ){
    while(true){
    u8g2.clearBuffer();
    drawDisplay();//draws the display
    u8g2.sendBuffer();
    delay(500);
    }
}

void coreTaskThree( void * pvParameters ){
    while(true){
     if (rpmcount >= 10) { 
      //Update RPM every 20 counts, increase this for better RPM resolution,
      //decrease for faster update
      rpm = 30*1000/(millis() - timeold)*rpmcount;
      timeold = millis();
      rpmcount = 0;
      rpm2 = rpm * 2;
      }
      delay(10);
   }
}

void drawDisplay(void)      //writes data to the display
{
     

    //Speed    
    u8g2.setFont(u8g2_font_6x13_tf); //set the font for a height and width of 6x13 pixels
    u8g2.drawStr(35,25,"km/h");
    u8g2.setFont(u8g2_font_smart_patrol_nbp_tf);
    u8g2.setCursor(10,15);
    u8g2.print((int) rpm2);
    u8g2.setCursor(10,25);
    u8g2.print((int)(2*3.1416*0.25*(rpm2/60)*3.6));//convert RPM to km/h

    //Temperature
    u8g2.setFont(u8g2_font_6x13_tf);
    u8g2.setCursor(75,27);
    u8g2.print(temp);
    u8g2.drawStr(110, 27,"C");

    //Potentiometer
    u8g2.setFont(u8g2_font_6x13_tf);
    u8g2.setCursor(25,62);
    u8g2.print((int)(100-((((float)PWMValue)/255)*100));
    u8g2.drawStr(40, 62," %");


    //draw lines to define the boxes on the display
    u8g2.drawLine(0,31,128,31);
    u8g2.drawLine(0,45,128,45);
    u8g2.drawLine(64,0,64,31);
    u8g2.drawLine(64,45,64,63);
    u8g2.drawLine(75,32,128,32);

    
  }
}
