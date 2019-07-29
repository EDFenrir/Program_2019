// the number of the LED pin
const int ledPin = 16;  // 16 corresponds to GPIO16

#define PinoPotenciometro  34 // Da o Nome de PinoPotenciometro a constante A0.

// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;
int ValorPotenciometro = 0; // Declaração da variável do tipo inteiro chamda de ValorPotenciometro .      
int ValorPWM = 0;        
 


void setup() {

  //Create a task that will be executed in coreTaskZero function, with priority 1 and execution on core 0
  xTaskCreate(
                    coreTaskZero,   /* function that implemets the task */
                    "coreTaskZero", /* name of the task */
                    10000,      /* number of words to be stored at the task stack */
                    NULL,       /* input task parameter (it can be NULL) */
                    1,          /* priority task (0 to N) */
                    NULL);       /* task reference (it can be NULL) */
                   
  delay(500); // delay to initialize the task

  //Create a task that will be executed in coreTaskOne function, with priority 2 and execution on core 1
  xTaskCreate(
                    coreTaskOne,   
                    "coreTaskOne", 
                    10000,      
                    NULL,       
                    2,          
                    NULL);

    delay(500);

   //Create a task that will be executed in coreTaskTwo function, with priority 2 and execution on core 0
   xTaskCreate(
                    coreTaskTwo, 
                    "coreTaskTwo",
                    10000,      
                    NULL,       
                    2,          
                    NULL); 

  // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin, ledChannel);

  pinMode(2, OUTPUT);
   
}

void loop() {
    delay(500);
}

void coreTaskZero( void * pvParameters ){
    ValorPotenciometro = analogRead(PinoPotenciometro);// Faz a leitura do valor do conersor ADC e joga para variável ValorPotenciometro.
    ValorPWM = map(ValorPotenciometro, 0, 4095, 255, 0); 
    ledcWrite(ledChannel, ValorPWM);
    delay(15);
}

void coreTaskOne( void * pvParameters ){
   digitalWrite(2, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                       // wait for a second
    digitalWrite(2, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);                       // wait for a second
}

void coreTaskTwo( void * pvParameters ){
}
