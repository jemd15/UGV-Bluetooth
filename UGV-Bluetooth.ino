// Programa android que utiliza:
// Aplicacion Bluetooth RC Controller. Se puede descargar del Store de Android. Gratuito.
// https://play.google.com/store/apps/details?id=braulio.calle.bluetoothRCcontroller
// Optimizado por Carlos Escobar Javier Muñoz y José López

#include <Servo.h> //incluimos la libreria para manejar el servo motor (torreta)

int ENB = 6;   // ENABLE B
int IN4 = 7;   // MOTOR B
int IN3 = 8;   // MOTOR B
int IN2 = 9;   // MOTOR A
int IN1 = 10;   // MOTOR A
int ENA = 11;  // ENABLE A
int laserPin = 5; //Laser
float VEL = 0;   // VELOCIDAD MOTORES
float balanceAdelante; //compensación de avance motores
float balanceAtras; //compensación de avance motores
float viraje = 0.4;
int lucesDelanteras = 12; // pin de salida para 2 luces led delanteras de alto brillo blancas
int lucesTraseras = 2; //pin de salida para luz led inferior de color azul
int modo; // contador para intercambiar el uso de la barra entre velocidad y ángulo de la torreta
int velTorreta; //velocidad de giro de la torreta
char comando; //variable que indica la acción a tomar
int torretaDetenida; // variable que contiene el valor correspondiente a la velocidad 0 de la torreta
Servo torreta; //creamos el servo torreta

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(lucesDelanteras, OUTPUT);
  pinMode(lucesTraseras, OUTPUT);
  pinMode(laserPin, OUTPUT);
  Serial.begin(9600);
  Serial1.begin(9600);
  balanceAdelante = 0.95  ; // VEL * 0.1 solo si es necesario compensar
  balanceAtras = 0.98 ; // VEL * 0.2 solo si es necesario compensar
  torreta.attach(2); //adjuntamos el servo al pin 2{
  modo = 0; 
  Serial.println("Modo Conduccion");
  velTorreta = 20;
  torretaDetenida = 94;
  torreta.write(torretaDetenida);
}


void loop() {
  if (Serial1.available() > 0) {
    comando = Serial1.read();
    switch (comando) {
      case 'F': //Abanzar
        analogWrite(ENA, VEL /* * balanceAdelante*/);
        analogWrite(ENB, VEL);
        adelante();
        Serial.println("adelante");
        break;
      case 'B': //Retroceder
        analogWrite(ENA, VEL * balanceAtras);
        analogWrite(ENB, VEL);
        atras();
        Serial.println("atras");
        break;
      case 'L': //Rotar izquierda
        if(modo == 0){
          izquierda();
          Serial.println("izquierda");
        }else if(modo == 1){
          torretaIzquierda();
        }
        break;
      case 'R': //Rotar derecha
        if(modo == 0){
        derecha();
        Serial.println("derecha");
        }else if (modo == 1){
          torretaDerecha();
        }
        break;
      case 'W': //Encender luces delanteras
        digitalWrite(lucesDelanteras, HIGH);
        break;
      case 'w': //Apagar luces delanteras
        digitalWrite(lucesDelanteras, LOW);
        break;
      case 'U': //Encender luces traseras
        digitalWrite(lucesTraseras, HIGH);
        break;
      case 'u': //Apagar luces traseras
        digitalWrite(lucesTraseras, LOW);
        break;
      case 'X':
        modo = 1;
        Serial.println("Modo Torreta");
        break;
      case 'x':
        modo = 0;
        Serial.println("Modo Conduccion");
        break;
      case 'G': //Adelante Izquierda
        adelanteIzquierda();
        Serial.println("adelante izquierda");
        break;
      case 'I': //Adelante Derecha
        adelanteDerecha();
        Serial.println("adelante derecha");
        break;
      case 'H': //Atras Izquierda
        atrasIzquierda();
        Serial.println("atras izquierda");
        break;
      case 'J': //Atras Derecha
        atrasDerecha();
        Serial.println("atras derecha");
        break;
      case 'V': //Laser Encendido
        digitalWrite(laserPin, HIGH);
        break;
      case 'v': //Laser Apagado
        digitalWrite(laserPin, LOW);
        break;
      case '0':
        VEL = 0;
        break;
      case '1':
        VEL = 75;
        break;
      case '2':
        VEL = 95;
        break;
      case '3':
        VEL = 115;
        break;
      case '4':
        VEL = 135;
        break;
      case '5':
        VEL = 155;
        break;
      case '6':
        VEL = 175;
        break;
      case '7':
        VEL = 195;
        break;
      case '8':
        VEL = 215;
        break;
      case '9':
        VEL = 235;
        break;
      case 'q':
        VEL = 255;
        break;
      case 'S':
        parar();
        break;
    }
  }
}

void apagar() {
  analogWrite(ENB, 0);
  analogWrite(ENA, 0);
}

void parar() {
  digitalWrite(IN1, LOW); // IZQUIERDA DETENIDA
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW); // DERECHA DETENIDA
  digitalWrite(IN4, LOW);
  
  torreta.write(torretaDetenida); //DETENER TORRETA
}

void adelante() {
  digitalWrite(IN1, HIGH); // IZQUIERDA ATRAS
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH); // DERECHA ATRAS
  digitalWrite(IN4, LOW); 
}

void atras() {
  digitalWrite(IN1, LOW); // IZQUIERDA ADELANTE
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW); // DERECHA ADELANTE
  digitalWrite(IN4, HIGH);  
}

void izquierda() {
  analogWrite(ENB, VEL);
  analogWrite(ENA, VEL);
  digitalWrite(IN1, HIGH); // IZQUIERDA ADELANTE
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW); // DERECHA ATRAS
  digitalWrite(IN4, HIGH);  
}

void derecha() {
  analogWrite(ENB, VEL);
  analogWrite(ENA, VEL);
  digitalWrite(IN1, LOW); // IZQUIERDA ATRAS
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH); // DERECHA ADELANTE
  digitalWrite(IN4, LOW);  
}

void adelanteIzquierda(){
  analogWrite(ENB, VEL * viraje);
  analogWrite(ENA, VEL);
  adelante();
}

void adelanteDerecha(){
  analogWrite(ENB, VEL);
  analogWrite(ENA, VEL * viraje);
  adelante();
}

void atrasIzquierda(){
  analogWrite(ENA, VEL);
  analogWrite(ENB, VEL * viraje);
  atras();
}

void atrasDerecha(){
  analogWrite(ENB, VEL);
  analogWrite(ENA, VEL * viraje);
  atras();
}

void sinConexion(){
  torreta.write(torretaDetenida);
  parar();  
  Serial.println("Sin conexion");
  delay(1000);
}

void torretaDerecha(){
  torreta.write(torretaDetenida + velTorreta);
  Serial.println("Torreta a la Derecha");
}

void torretaIzquierda(){
  torreta.write(torretaDetenida - velTorreta);
  Serial.println("Torreta a la izquierda");
}

