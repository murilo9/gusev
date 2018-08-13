//Programa : Rover ASG-1
//Autor : Murilo Henrique
#include 
#define SERVO 6 // Porta Digital 6 PWM

//Define os pinos dos motores
int IN1 = 2;
int IN2 = 3;
int IN3 = 4;
int IN4 = 5;

const int echoPin=8;
const int trigPin=7;
//roxo, azul

unsigned long int distancia;
Servo s; // Variável Servo
int pos; // Posição Servo
int lastTime=0;
int periodo=0;
int detectedGraus_dist[18];
int i;

void setup()
{
  //Define os pinos dos INs como saida
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
 
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  s.attach(SERVO);
  s.write(90); // Inicia motor posição 90
  int pos=0;

  //Inicializa a serial
  Serial.begin(9600);

  //Teste de motores
  digitalWrite(IN1, HIGH);digitalWrite(IN2, LOW);digitalWrite(IN3, LOW);digitalWrite(IN4, HIGH);  //Gira pra esquerda
  delay(300);
  digitalWrite(IN1, LOW);digitalWrite(IN2, LOW);digitalWrite(IN3, LOW);digitalWrite(IN4, LOW);
  s.write(90);
  delay(100);
  Serial.println("Robô inicializado - Alexander Sergei Gusev 01");
  Serial.println("FORA TEMER");
  //delay 500 = virada em 45 graus
  // servo 0 graus -> direita
  // servo 90 graus -> frente
  // servo 180 graus -> esquerda
}

void loop()
{
            /*delay(100);
            digitalWrite(trigPin, HIGH);
            delayMicroseconds(11);
            digitalWrite(trigPin, LOW);
            distancia=pulseIn(echoPin, HIGH)/100*2;
            Serial.print("Atualmente em: ");Serial.print(pos);Serial.print(" graus");
            Serial.print("Distancia: ");Serial.print(distancia);Serial.println(" cm");*/
        Serial.println("Fazendo varredura");
        s.write(0);delay(20);    //Coloca o leitor da posição 0
        pos=0;
        while(pos<=180)    //Faz varredura com servo e guarda as distancias no array detectedGraus_dist[]
        {
            s.write(pos);delay(100);
            digitalWrite(trigPin, HIGH);
            delayMicroseconds(11);
            digitalWrite(trigPin, LOW);
            distancia=pulseIn(echoPin, HIGH)/100*2;
            if(distancia>=4 && distancia <=200)
              detectedGraus_dist[pos/10]=distancia;   //Guarda o valor da distância detectada na posição em graus lida
            else
              detectedGraus_dist[pos/10]=200;       //Se nada for detectado nesta posição em graus, guarda o valor 0
            delay(20);
            pos+=10;
        }
        s.write(90);delay(200);   //Após a varredura, deixa o leitor virado pra frente
        Serial.println("Resultado da varredura:");
        i=0;
        while(i<=18)
        {
          //Escreve as distâncias em cada posição em graus na porta serial
          Serial.print(i*10);Serial.print(" graus: objeto a ");Serial.print(detectedGraus_dist[i]);Serial.println(" cm");
          //Transmite a distância desta posição
          i+=1;
        }
        //Efetuando próximo movimento
        //Se não houver nada a menos de 40 cm da frente
        if((detectedGraus_dist[5])>40&&(detectedGraus_dist[6])>40&&(detectedGraus_dist[7])>40&&(detectedGraus_dist[8])>40&&(detectedGraus_dist[9])>40&&
        (detectedGraus_dist[10])>40&&(detectedGraus_dist[11])>40&&(detectedGraus_dist[12])>40&&(detectedGraus_dist[13])>40) 
        {
          Serial.println("Avançando");
          digitalWrite(IN1, LOW);digitalWrite(IN2, HIGH);digitalWrite(IN3, LOW);digitalWrite(IN4, HIGH);delay(500);   //Avança
          digitalWrite(IN1, LOW);digitalWrite(IN2, LOW);digitalWrite(IN3, LOW);digitalWrite(IN4, LOW);delay(100);          
        }
        //Caso contrário, verifica se não há nenhum objeto a menos de 40 cm da esquerda
        else if((detectedGraus_dist[14])>40&&(detectedGraus_dist[15])>40&&(detectedGraus_dist[16])>40&&(detectedGraus_dist[17])>40&&(detectedGraus_dist[18])>40)
        {
          Serial.println("Virando a esquerda");
          digitalWrite(IN1, HIGH);digitalWrite(IN2, LOW);digitalWrite(IN3, LOW);digitalWrite(IN4, HIGH);delay(500);   //Gira 45 graus pra esquerda
          digitalWrite(IN1, LOW);digitalWrite(IN2, LOW);digitalWrite(IN3, LOW);digitalWrite(IN4, LOW);delay(100);
          digitalWrite(IN1, LOW);digitalWrite(IN2, HIGH);digitalWrite(IN3, LOW);digitalWrite(IN4, HIGH);delay(500);   //Avança
          digitalWrite(IN1, LOW);digitalWrite(IN2, LOW);digitalWrite(IN3, LOW);digitalWrite(IN4, LOW);delay(100);
        }
        //Caso contrário, verifica se não há nenhum objeto a menos de 40 cm da direita
        else if((detectedGraus_dist[4])>40&&(detectedGraus_dist[3])>40&&(detectedGraus_dist[2])>40&&(detectedGraus_dist[1])>40&&(detectedGraus_dist[0])>40)
        {
          Serial.println("Virando a direita");
          digitalWrite(IN1, LOW);digitalWrite(IN2, HIGH);digitalWrite(IN3, HIGH);digitalWrite(IN4, LOW);delay(500);   //Gira 45 graus pra direita
          digitalWrite(IN1, LOW);digitalWrite(IN2, LOW);digitalWrite(IN3, LOW);digitalWrite(IN4, LOW);delay(100);
          digitalWrite(IN1, LOW);digitalWrite(IN2, HIGH);digitalWrite(IN3, LOW);digitalWrite(IN4, HIGH);delay(500);   //Avança
          digitalWrite(IN1, LOW);digitalWrite(IN2, LOW);digitalWrite(IN3, LOW);digitalWrite(IN4, LOW);delay(100);
        }
        //Caso contrário (se todas as direções tiverem obstáculos), dá meia-volta
        else
        {
          Serial.println("Dando meia-volta");
          digitalWrite(IN1, HIGH);digitalWrite(IN2, LOW);digitalWrite(IN3, LOW);digitalWrite(IN4, HIGH);delay(2000);   //Gira 180 graus pra esquerda (meia-volta)
          digitalWrite(IN1, LOW);digitalWrite(IN2, LOW);digitalWrite(IN3, LOW);digitalWrite(IN4, LOW);delay(100);
          digitalWrite(IN1, LOW);digitalWrite(IN2, HIGH);digitalWrite(IN3, LOW);digitalWrite(IN4, HIGH);delay(500);   //Avança
          digitalWrite(IN1, LOW);digitalWrite(IN2, LOW);digitalWrite(IN3, LOW);digitalWrite(IN4, LOW);delay(100);
        }
}

