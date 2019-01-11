//Programa : Rover ASG-1
//Autor : Murilo Henrique
#include 
#define SERVO 6 // Porta Digital 6 PWM

//Define os pinos dos motores:
int IN1 = 2;
int IN2 = 3;
int IN3 = 4;
int IN4 = 5;

//Define os pinos do sensor de ulstrassom:
const int echoPin=8;  //cabo roxo
const int trigPin=7;  //cabo azul

unsigned long int distancia;
Servo s; // Variável Servo
int pos; // Posição Servo
int lastTime=0;
int periodo=0;
int detectedGraus_dist[18];   //Array que armazena a distância de cada posição do sensor
int i;

void setup()
{
  //Define os pinos dos INs como saida
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
 
  //Define os pinos do sensor como entrada e saída
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  s.attach(SERVO);    //Define o pino do servomotor
  s.write(90); //Inicia sensor na posição 90º (pra frente)
  int pos=0;  //Variável do contador do sensor

  //Inicializa a serial
  Serial.begin(9600);

  //Teste de motores
  digitalWrite(IN1, HIGH);digitalWrite(IN2, LOW);digitalWrite(IN3, LOW);digitalWrite(IN4, HIGH);  //Gira pra esquerda
  delay(300);   //Espera 0.3 s
  digitalWrite(IN1, LOW);digitalWrite(IN2, LOW);digitalWrite(IN3, LOW);digitalWrite(IN4, LOW);    //Pára
  s.write(90);   //Coloca o sensor na posição 90º (pra frente) 
  delay(100);
  Serial.println("Robô inicializado - Alexander Sergei Gusev 01");
}

void loop()
{
        Serial.println("Fazendo varredura");
        s.write(0);delay(20);    //Coloca o leitor da posição 0
        pos=0;
        while(pos<=180)    //Faz varredura com o servo e guarda as distancias no array detectedGraus_dist[]
        {
            s.write(pos);delay(100);    //Coloca o servo na posição atual
            digitalWrite(trigPin, HIGH);    //Ativa a leitura do sensor
            delayMicroseconds(11);    //Aguarda o sensor calcular a distância
            digitalWrite(trigPin, LOW);   //Desativa a leitura do sensor
            distancia=pulseIn(echoPin, HIGH)/200;   //Distância(cm) = tempo do ciclo / 200
            if(distancia>=4 && distancia <=200)   //Se a distância calculada pelo sensor estiver entre 4cm e 200cm
              detectedGraus_dist[pos/10]=distancia;   //Guarda o valor da distância detectada na posição em graus lida
            else    //Caso contrário
              detectedGraus_dist[pos/10]=200;       //Descarta o valor calculado (0)
            delay(20);
            pos+=10;    //Avança o sensor para a próxima posição (+10º)
        }
        //Após a varredura de todas as posições, deixa o servo virado pra frente:
        s.write(90);delay(200);   
        Serial.println("Resultado da varredura:");
        i=0;
        while(i<=18)    //Analisa cada uma das 18 direções varridas (0º a 180º)
        {
          //Escreve as distâncias em cada direção na porta serial
          Serial.print(i*10);Serial.print(" graus: objeto a ");Serial.print(detectedGraus_dist[i]);Serial.println(" cm");
          i+=1;
        }
        //Decidindo em que direção o robô deverá se mover
        //Se não houver nada a menos de 40 cm da frente
        if((detectedGraus_dist[5])>40&&(detectedGraus_dist[6])>40&&(detectedGraus_dist[7])>40&&(detectedGraus_dist[8])>40&&(detectedGraus_dist[9])>40&&
        (detectedGraus_dist[10])>40&&(detectedGraus_dist[11])>40&&(detectedGraus_dist[12])>40&&(detectedGraus_dist[13])>40) 
        {
          Serial.println("Avançando");
          digitalWrite(IN1, LOW);digitalWrite(IN2, HIGH);digitalWrite(IN3, LOW);digitalWrite(IN4, HIGH);delay(500);   //Avança pra frente
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
