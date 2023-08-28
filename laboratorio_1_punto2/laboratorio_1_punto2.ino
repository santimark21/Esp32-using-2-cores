
#define BUILTIN_LED_PIN 2

static SemaphoreHandle_t semaforito; //declaracion de variable semaforo
const int portPin = 34; //inicializacion del pin para leer
const int button = 0; // pin del boton de usuario
int buttonState = HIGH; // inicializamos el estado del boton en 1
int aux1 = 0; // variable para el antirebote
int StopTask = 0; // variable para parar el Task1

// tarea designada al core 0
void Task1code( void * parameter) {
  double postValor = 0; //Declaracion de valor inicial

  //double arreglo[10000];
  while(1){


    if (StopTask == 0){
    xSemaphoreTake(semaforito,portMAX_DELAY);
    
    Serial.print("Task running on core "); // texto del core utilizado
    Serial.print(xPortGetCoreID()); // funcion para identificar el core que se esta ejecutando

    postValor = analogRead(portPin)*3.3/4095; // lectura del pin

    Serial.print(" El valor de voltaje es: ");
    Serial.println(postValor); // impresion del valor del pin
    }
    xSemaphoreGive(semaforito);

    vTaskDelay(1000/portTICK_PERIOD_MS); 
    
  }
  }

// tarea designada al core 1
void Task2code( void * parameter) {
  
  while(1){
    xSemaphoreTake(semaforito,portMAX_DELAY);
    Serial.print("Task running on core "); // texto del core utilizado
    Serial.print(xPortGetCoreID()); // funcion para identificar el core que se esta ejecutando
    
    digitalWrite(BUILTIN_LED_PIN,!digitalRead(BUILTIN_LED_PIN));
    buttonState = !digitalRead(button);

    if (buttonState == 1) {
      aux1 = 1;
      
    }
    if(aux1 == 1 && buttonState == 0){
      StopTask =!StopTask;
      aux1 = 0;

    }
    Serial.print(" status: ");
    Serial.println(buttonState);
    xSemaphoreGive(semaforito);

    vTaskDelay(50/portTICK_PERIOD_MS);
    
  }
  }

void setup() {

  TaskHandle_t Task1,Task2;
  
  Serial.begin(115200);
  // Built in led pint as outpt
  pinMode(BUILTIN_LED_PIN,OUTPUT);  
  Serial.print("setup() running on core ");
  Serial.println(xPortGetCoreID());
 

  // Create mutex before starting tasks
  semaforito = xSemaphoreCreateMutex();

  xTaskCreatePinnedToCore(
      Task1code, /* Function to implement the task */
      "Task1", /* Name of the task */
      10000,  /* Stack size in words */
      NULL,  /* Task input parameter */
      10,  /* Priority of the task */
      &Task1,  /* Task handle. */
      0); /* Core where the task should run */

  xTaskCreatePinnedToCore(
      Task2code, /* Function to implement the task */
      "Task2", /* Name of the task */
      10000,  /* Stack size in words */
      NULL,  /* Task input parameter */
      10,  /* Priority of the task */
      &Task2,  /* Task handle. */
      1); /* Core where the task should run */
}

void loop() {
 // Clears loop function from OS
 vTaskDelete(NULL);
}