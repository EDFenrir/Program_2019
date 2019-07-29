// the number of the LED pin
const int ledPin = 16;  // 16 corresponds to GPIO16

#define PinoPotenciometro  34 // Da o Nome de PinoPotenciometro a constante A0.

// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;
int ValorPotenciometro = 0; // Declaração da variável do tipo inteiro chamda de ValorPotenciometro .      
int ValorPWM = 0;        
 
void setup(){
  // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin, ledChannel);
}
 
void loop(){
    
     ValorPotenciometro = analogRead(PinoPotenciometro);// Faz a leitura do valor do conersor ADC e joga para variável ValorPotenciometro.
     ValorPWM = map(ValorPotenciometro, 0, 4095, 255, 0); 
     ledcWrite(ledChannel, ValorPWM);
   

}
