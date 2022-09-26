#include <Stepper.h>

Stepper motor1(2048, 8, 10, 9, 11); // PASOS PARA UNA VUELTA COMPLETO
const int PIN_LUZ = 0;
int velocidad = 4; // En RPM (Valores de  1, 2 o 3  para el modelo 28BYJ-48) y 4 para cuando es de noche 
int potencia = 0;
int luz = 0;
int zona = 1; // Zona donde se está regando actualmente, tomará valores del 1 al 3
int apagado = 0; // Bandera para controlar el estado apagado/encendido
int unaVuelta = 2048;

void setup() 
{
  Serial.begin(9600);
}

void loop()
{
  regar(4096); // 2 vueltas en la zona 1 (aprox 40 segundos)  
  zona = zona + 1;
  delay(10000);  

  regar(6144); // 3 vueltas en la zona 2 (aprox 60 segundos)
  zona = zona + 1;
  delay(10000);
  
  regar(8192); // 4 vueltas en la zona 3 (aprox 80 segundos)
  
  apagarMotor();
  delay(20000); // Una vez completado el ciclo de zonas (cerca del mediodia), el motor volverá a regar 20 segs después
  zona = 1;

  regar(2048); // 1 vuelta en la zona 1 (aprox 20 segundos)
  zona = zona + 1;
  delay(10000);  
  
  regar(4096); // 2 vueltas en la zona 2 (aprox 40 segundos)
  zona = zona + 1;
  delay(10000);
  
  regar(6144); // 3 vueltas en la zona 3 (aprox 60 segundos)
  apagarMotor();
  zona = 1; 
}

void obtenerDatos()
{
  luz = analogRead(PIN_LUZ);
	mapear(luz); 
}

void mapear(int l)
{
  if (l <= 250) {
    velocidad = 1;
  }
  else if (l > 250 && l <= 500) {
    velocidad = 2;
  }
  else if (l > 500 && l <= 750)  {
    velocidad = 3;
  }
  else if (l > 750 && l <= 1023){
    velocidad = 4; // Significa que es de noche
  };  
}

void regar(int pasos) 
{
  for (int i = 0; i < (pasos/unaVuelta); i++ ) // Entre cada vuelta el motor va a tomar una nueva medida de la intensidad de la luz
  {
    while ( velocidad == 4) // Mientras sea de noche el motor permanecerá apagado
    { 
      if (apagado = 0) { // Si el motor está encendido se apagará
        apagarMotor();
      }
	    obtenerDatos(); // El microcontrolador seguirá tomando datos cada 1 segundo hasta que sea de día, entonces comenzará a regar    
      delay(1000);
    } 
    controlarMotor(pasos);
    obtenerDatos();
  }    
  delay(1000);
}

// La velocidad va a ser 4 cuando la fotoresistencia no detecte luz, es decir por la noche. 
// Va a ser 1 cuando la intensidad de la luz sea maxima, es decir en horario cercano al mediodia en condiciones ideales. 
// El motor debe permanecer apagado cuando la velocidad sea 4. 
// A una velocidad de 3 rpm hace 3 vueltas en 1 minuto --> en 60 minutos (1 hora) dará 180 vueltas.
void controlarMotor(int pasos) 
{    
  if (velocidad < 4) { 
    motor1.setSpeed(velocidad); // El motor girará a la velocidad obtenida de mapear los datos recibidos por la fotoresistencia 
    Serial.print("regando zona: ");  
    Serial.println(zona);   
    Serial.print("velocidad de riego: ");
    Serial.print(velocidad);
    Serial.println(" rpms");
    Serial.print("cantidad de vueltas: ");
    Serial.println(pasos/unaVuelta);
    Serial.print("intensidad de luz: ");    
    Serial.println(luz);
    Serial.println("");
    motor1.step(unaVuelta); // El motor realizará la cantidad de pasos equivalentes a una vuelta 
    apagado = 0;     
  }
	else if ( velocidad == 4 ) {
    apagarMotor(); 
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