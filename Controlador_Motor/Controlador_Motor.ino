#include <OneWire.h>
#include <DallasTemperature.h>
#include <ACS712.h>
#include <U8g2lib.h>
#include <SPI.h>

U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, 4, 2, 0, 16); // Enable=6, RW=data=5, RS=4, Rst=17

OneWire pin(15);
DallasTemperature bus(&pin);
DeviceAddress sensor;

//Pin sensor current
#define pinoCorrente = 26;
ACS712 sensorCorrente(ACS712_30A, 26);

volatile byte rpmcount;

unsigned int rpm = 0;
unsigned int rpm2 = 0;

unsigned long timeold = 0;

// the number of the LED pin
const int ledPin = 17;  // 16 corresponds to GPIO16

#define PinoPotenciometro  34 // Da o Nome de PinoPotenciometro a constante A0.

// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;
int ValorPotenciometro = 0; // Declaração da variável do tipo inteiro chamda de ValorPotenciometro .      
int ValorPWM = 0;
int temp = 0;
float P = 0;    

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

  //sensor current
  Serial.begin(9600);
  sensorCorrente.calibrate();

  //hall sensor  
  pinMode(32, INPUT_PULLUP);
  attachInterrupt(32, rpm_fun, RISING);

  u8g2.begin();

  Serial.begin(115200);

  bus.begin();
  bus.getAddress(sensor, 0);
  
   // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin, ledChannel);
  
  //Create a task that will be executed in coreTaskZero function, with priority 1 and execution on core 0
  xTaskCreate(
                    coreTaskZero,   /* função que implementa a tarefa */
                    "coreTaskZero", /* nome da tarefa */
                    10000,      /* número de palavras a serem alocadas para uso com a pilha da tarefa */
                    NULL,       /* parâmetro de entrada para a tarefa (pode ser NULL) */
                    2,          /* prioridade da tarefa (0 a N) */
                    NULL);       /* referência para a tarefa (pode ser NULL) */
                    
  delay(500); // delay to initialize the task

  //coreTaskOne: atualizar as informações do display
  xTaskCreate(
                    coreTaskOne,   /* função que implementa a tarefa */
                    "coreTaskOne", /* nome da tarefa */
                    10000,      /* número de palavras a serem alocadas para uso com a pilha da tarefa */
                    NULL,       /* parâmetro de entrada para a tarefa (pode ser NULL) */
                    3,          /* prioridade da tarefa (0 a N) */
                    NULL);       /* referência para a tarefa (pode ser NULL) */

    delay(500); //tempo para a tarefa iniciar

     //coreTaskOne: atualizar as informações do display
     xTaskCreate(
                    coreTaskTwo,   /* função que implementa a tarefa */
                    "coreTaskTwo", /* nome da tarefa */
                    10000,      /* número de palavras a serem alocadas para uso com a pilha da tarefa */
                    NULL,       /* parâmetro de entrada para a tarefa (pode ser NULL) */
                    2,          /* prioridade da tarefa (0 a N) */
                    NULL);       /* referência para a tarefa (pode ser NULL) */

    delay(500); //tempo para a tarefa iniciar
    
    //coreTaskOne: atualizar as informações do display
     xTaskCreate(
                    coreTaskThree,   /* função que implementa a tarefa */
                    "coreTaskThree", /* nome da tarefa */
                    10000,      /* número de palavras a serem alocadas para uso com a pilha da tarefa */
                    NULL,       /* parâmetro de entrada para a tarefa (pode ser NULL) */
                    2,          /* prioridade da tarefa (0 a N) */
                    NULL);       /* referência para a tarefa (pode ser NULL) */

    delay(500); //tempo para a tarefa iniciar
  
     xTaskCreate(
                    coreTaskFour,   /* função que implementa a tarefa */
                    "coreTaskFour", /* nome da tarefa */
                    10000,      /* número de palavras a serem alocadas para uso com a pilha da tarefa */
                    NULL,       /* parâmetro de entrada para a tarefa (pode ser NULL) */
                    2,          /* prioridade da tarefa (0 a N) */
                    NULL);       /* referência para a tarefa (pode ser NULL) */

    delay(500); //tempo para a tarefa iniciar
 
}
 
void loop(){
}

void coreTaskZero( void * pvParameters ){
    while(true){
    bus.requestTemperatures(); 
    temp = bus.getTempC(sensor);
    delay(500);                    
    }
}

void coreTaskOne( void * pvParameters ){
    while(true){
    ValorPotenciometro = analogRead(PinoPotenciometro);// Faz a leitura do valor do conersor ADC e joga para variável ValorPotenciometro.
    ValorPWM = map(ValorPotenciometro, 0, 4095, 0, 255); 
    ledcWrite(ledChannel, ValorPWM);
    //Serial.println(ValorPWM);
    delay(100);
    }
}

void coreTaskTwo( void * pvParameters ){
    while(true){
    u8g2.clearBuffer();
    //drawLogo();
    drawURL();
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
      //Serial.println(rpm2,DEC);
      }
      delay(10);
   }
}

void coreTaskFour( void * pvParameters ){
    while(true){
        
        float U = 36;

        float I = sensorCorrente.getCurrentDC();

        // To calculate the power we need voltage multiplied by current
        float P = U * I;

        //Serial.println(String("P = ") + P + " Watts");

        delay(1000);
   }
}
void drawLogo(void)
{
    u8g2.setFontMode(1);  // Transparent
#ifdef MINI_LOGO

    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb16_mf);
    u8g2.drawStr(0, 22, "U");
    
    u8g2.setFontDirection(1);
    u8g2.setFont(u8g2_font_inb19_mn);
    u8g2.drawStr(14,8,"8");
    
    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb16_mf);
    u8g2.drawStr(36,22,"g");
    u8g2.drawStr(48,22,"\xb2");
    
    u8g2.drawHLine(2, 25, 34);
    u8g2.drawHLine(3, 26, 34);
    u8g2.drawVLine(32, 22, 12);
    u8g2.drawVLine(33, 23, 12);
#else

    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb24_mf);
    u8g2.drawStr(0, 30, "U");
    
    u8g2.setFontDirection(1);
    u8g2.setFont(u8g2_font_inb30_mn);
    u8g2.drawStr(21,8,"8");
        
    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb24_mf);
    u8g2.drawStr(51,30,"g");
    u8g2.drawStr(67,30,"\xb2");
    
    u8g2.drawHLine(2, 35, 47);
    u8g2.drawHLine(3, 36, 47);
    u8g2.drawVLine(45, 32, 12);
    u8g2.drawVLine(46, 33, 12);
    
#endif
}

void drawURL(void)
{
#ifndef MINI_LOGO
  u8g2.setFont(u8g2_font_inr30_mr);
  if ( u8g2.getDisplayHeight() < 59 )
  {
    //u8g2.drawStr(89,20,"github.com");
    //u8g2.drawStr(73,29,"/olikraus/u8g2");
  }
  else
  {
    u8g2.setFont(u8g2_font_smart_patrol_nbp_tf);
    u8g2.drawStr(1, 12,"km/h");
    u8g2.drawStr(80, 12,"Temp");
    u8g2.drawStr(90, 45,"Pot");
    
    //Velocidade
    u8g2.setFont(u8g2_font_inr30_mr);
    u8g2.setCursor(10,55);
    //u8g2.print((int)((((float)ValorPWM)/255)*100));
    u8g2.print((int)(2*3.1416*0.025*(rpm2/60)*3.6));
    
    //Temperatura
    u8g2.setFont(u8g2_font_6x13_tf);
    u8g2.setCursor(90,27);
    u8g2.print(temp);
    u8g2.drawStr(105, 27,"C");

    //Potencia
    u8g2.setFont(u8g2_font_6x13_tf);
    u8g2.setCursor(90,60);
    u8g2.print((float)P);
    u8g2.drawStr(105, 60,"W");
    
    //Potenciometro
    u8g2.setFont(u8g2_font_6x13_tf);
    u8g2.setCursor(45,62);
    u8g2.print((int)((((float)ValorPWM)/255)*100));
    u8g2.drawStr(60, 62," %");
    

    //line box
    u8g2.drawLine(0,16,75,16);
    u8g2.drawLine(75,0,75,63);
    u8g2.drawLine(75,32,128,32);

    
  }
#endif
}
