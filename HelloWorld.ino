// LIBRERIAS
#include <LiquidCrystal.h>
#include <dht_nonblocking.h>

// DEFINICION DE PINES
static const int DHT_SENSOR_PIN = 2; //DHT11 esta conectado al pin 2
#define DHT_SENSOR_TYPE DHT_TYPE_11 //DHT11 es un sensor de tipo DHT11
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE ); //Declaramos el sensor a partir de las constantes
#define ENABLE 5 //pin que controla la actividad del motor
#define DIRA 3 //pin que controla el sentido (horario) del motor
LiquidCrystal lcd(7, 8, 9, 10, 11, 12); //pines que controlan la pantalla lcd

// iniciamos la librría a partir de los pins en los que se encuentran los sensores/actuadores 
void setup() {
    Serial.begin(9600); //Definimos el ratio 9600 bits/segundo de bits que envia la placa a la consola 
  //Establecemos el numero de columnas (16) y filas (2) que tiene nuestro LCD:
  lcd.begin(16, 2);
  // Imprimimos el mensaje Hello World para comprobar que el lcd funciona
  lcd.print("Hello, World!");
  pinMode(ENABLE,OUTPUT); //Definimos el pin 5 y 3 como actuadores del motor
  pinMode(DIRA,OUTPUT);
}

/*Aqui tenemos una funcion que cuenta con un temporizador en el que cuando este supera los 3 segundos intenta conseguir la lectura de temperatura y humedad. 
En caso de conseguir la medida devuelve true y en caso contrario, false. Se trata de una funcion estatica para guardar los valores del tiempo que pasa*/
static bool measure_environment( float *temperature, float *humidity )
{
  static unsigned long measurement_timestamp = millis( );
  //
  if( millis( ) - measurement_timestamp > 3000ul )
  {
    if( dht_sensor.measure( temperature, humidity ) == true )
    {
      measurement_timestamp = millis( );
      return( true );
    }
  }
  return(false);
}
/*Esto se trata del bucle principal que va recibiendo los valores de temperatura y humedad a partir del sensor DHT11 y lo va imprimiendo en la pantalla 
del lcd en caso de que la lectura sea correcta (para ello llama a la funcion anterior)*/
void loop() { 
  float temperature; 
  float humidity;

  if( measure_environment( &temperature, &humidity ) == true )
  {
    //Imprimimos las variables en la consola
    Serial.print( "Temp = " );
    Serial.print( temperature, 1 );
    Serial.print( " Cº,");
    Serial.print(" Humidity = " );
    Serial.print( humidity, 1 );
    Serial.println("%");
    //Imprimimos las variables en el lcd
    lcd.setCursor(0, 0);
    lcd.print( "Temp = " );
    lcd.print( temperature, 1 );
    lcd.print( " Cº,");
    lcd.setCursor(0, 1);
    lcd.print("Humidity = " );
    lcd.print( humidity, 1 );
    lcd.print("%"); 
  }
  //Por ultimo encontramos un ultimo bucle donde, en caso de que la temperatura supere los 27 grados activa la accion del motor que actuara como ventilador
  if (temperature > 27.0){
    digitalWrite(ENABLE,HIGH); //enable on
    digitalWrite(DIRA,HIGH);
    }
    else {
    digitalWrite(DIRA,LOW);
    }
}

