#include <OneWire.h>
#include <DallasTemperature.h>

OneWire pin(4);
DallasTemperature bus(&pin);
DeviceAddress sensor;

// the number of the LED pin
const int ledPin = 16;  // 16 corresponds to GPIO16

#define PinoPotenciometro  34 // Da o Nome de PinoPotenciometro a constante A0.

// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;
int ValorPotenciometro = 0; // Declaração da variável do tipo inteiro chamda de ValorPotenciometro .      
int ValorPWM = 0;
int temp = 0;

//variaveis que indicam o núcleo
static uint8_t taskCoreZero = 0;
static uint8_t taskCoreOne  = 1;

void setup(){

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
  
 
}
 
void loop(){
}

void coreTaskZero( void * pvParameters ){
    while(true){
    bus.requestTemperatures(); 
    temp = bus.getTempC(sensor);
    Serial.println(temp);
    delay(500);                    
    }
}

void coreTaskOne( void * pvParameters ){
    while(true){
    ValorPotenciometro = analogRead(PinoPotenciometro);// Faz a leitura do valor do conersor ADC e joga para variável ValorPotenciometro.
    ValorPWM = map(ValorPotenciometro, 0, 4095, 255, 0); 
    ledcWrite(ledChannel, ValorPWM);
    Serial.println(ValorPWM);
    delay(500);
    }
}
