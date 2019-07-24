

//variables that stores the cores number
static uint8_t taskCoreZero = 0;
static uint8_t taskCoreOne  = 1;

void setup() {

  //Create a task that will be executed in coreTaskZero function, with priority 1 and execution on core 0
  xTaskCreatePinnedToCore(
                    coreTaskZero,   /* function that implemets the task */
                    "coreTaskZero", /* name of the task */
                    10000,      /* number of words to be stored at the task stack */
                    NULL,       /* input task parameter (it can be NULL) */
                    1,          /* priority task (0 to N) */
                    NULL,       /* task reference (it can be NULL) */
                    taskCoreZero);         /* core that will execute the task */
                    
  delay(500); // delay to initialize the task

  //Create a task that will be executed in coreTaskOne function, with priority 2 and execution on core 1
  xTaskCreatePinnedToCore(
                    coreTaskOne,   
                    "coreTaskOne", 
                    10000,      
                    NULL,       
                    2,          
                    NULL,       
                    taskCoreOne);

    delay(500);

   //Create a task that will be executed in coreTaskTwo function, with priority 2 and execution on core 0
   xTaskCreatePinnedToCore(
                    coreTaskTwo, 
                    "coreTaskTwo",
                    10000,      
                    NULL,       
                    2,          
                    NULL,       
                    taskCoreZero); 
   
}

void loop() {
}

void coreTaskZero( void * pvParameters ){
 
}

void coreTaskOne( void * pvParameters ){
}

void coreTaskTwo( void * pvParameters ){
}
