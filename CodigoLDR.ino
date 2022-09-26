#include <Stepper.h>

Stepper motor1(2048, 8, 10, 9, 11); // PASOS PARA UNA VUELTA COMPLETO
const int PIN_LUZ = 0;
int velocidad = 4; // En RPM (Valores de  1, 2 o 3  para el modelo 28BYJ-48) y 4 para cuando es de noche 
int potencia = 0;
int luz = 0;
int zona = 1; // Zona donde se está regando actualmente, tomará valores del 1 al 3
int apagado = 0; // Bandera para controlar el estado apagado/encendido
int giro = 512;
int vueltas = 1;
unsigned long tiempo = 0;
long tiempoUltimoCambio = 0;

void setup() 
{
  Serial.begin(9600);
}

void loop()
{
  tiempo = millis();
  tiempoUltimoCambio = tiempo;
  regar(20000); // Riega zona 1 durante 20segs.
  zona++;
  vueltas=1;  
  delay(5000);  
  
  tiempoUltimoCambio = tiempo;
  regar(25000); // Riega zona 2 durante 25segs.
  zona++;
  vueltas=1;
  delay(5000);

  tiempoUltimoCambio = tiempo;
  regar(30000); // Riega zona 3 durante 30segs.
  
  apagarMotor();
  delay(20000); // Una vez completado el ciclo de zonas (cerca del mediodia), el motor volverá a regar 20 segs después
  zona = 1;
  vueltas=1;
  
  tiempoUltimoCambio = tiempo;
  regar(20000); // Riega zona 1 durante 20segs.
  zona++;
  vueltas=1;
  delay(5000);  
  
  tiempoUltimoCambio = tiempo;
  regar(25000); // Riega zona 2 durante 25segs.
  zona++;
  vueltas=1;
  delay(5000);
  
  tiempoUltimoCambio = tiempo;
  regar(30000); // Riega zona 3 durante 30segs.
  apagarMotor();
  zona = 1; 
  vueltas=1;
  delay(20000);  
}

void obtenerDatos()
{
  luz = analogRead(PIN_LUZ);
	velocidad = map(luz, 0, 1023, 1, 4);
}

void regar(int duracion) 
{
  while ( tiempo - tiempoUltimoCambio < duracion) // Controla que el motor gire durante el tiempo pasado por parametro 
  {                                                 // (al ser una duracion pequeña para la demo, puede pasarse unos 7 segs.)
    obtenerDatos();
    controlarMotor(); 
    tiempo=millis();
    delay(1000);
  };  
}

// La velocidad va a ser 4 cuando la fotoresistencia no detecte luz, es decir por la noche. 
// Va a ser 1 cuando la intensidad de la luz sea maxima, es decir en horario cercano al mediodia en condiciones ideales. 
// El motor debe permanecer apagado cuando la velocidad sea 4. 
void controlarMotor() 
{    
  if (velocidad < 4) { 
    motor1.setSpeed(velocidad); // El motor girará a la velocidad obtenida de mapear los datos recibidos por la fotoresistencia 
    Serial.print("regando zona: ");  
    Serial.println(zona);   
    Serial.print("velocidad de riego: ");
    Serial.print(velocidad);
    Serial.println(" rpms");
    Serial.print("cantidad de vueltas: ");
    Serial.println(vueltas);
    Serial.print("lectura de luz: ");    
    Serial.println(luz);
    Serial.println("");
    motor1.step(giro); // El motor realizará la cantidad de pasos pasada por parametros (512, 1/4 de vuelta)
    apagado = 0;
    vueltas++;     
  }
	else if ( velocidad == 4 ) {
    if (apagado = 0) { // Si el motor está encendido se apagará
      apagarMotor();
    }; 
    tiempo = tiempo - 1000;
    Serial.println("Motor apagado");  
    Serial.print("lectura de luz: ");    
    Serial.println(luz); 
    Serial.println("");
  } 
}

void apagarMotor() 
{    
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  apagado = 1;
}