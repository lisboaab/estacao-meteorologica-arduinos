// Inclusão das bibliotecas do sensor DHT22 e Do display LCD
#include "Arduino.h"
#include "BTHC05.h"
#include <LiquidCrystal_I2C.h>;
#include <DHT.h>;

#include <virtuabotixRTC.h>           
//Determina os pinos ligados ao modulo
//myRTC(clock, data, rst)

virtuabotixRTC myRTC(10, 11, 12); //pinos onde está conectado o relógio
//Informacoes iniciais de data e hora
  //Apos setar as informacoes, comente a linha abaixo
  //(segundos, minutos, hora, dia da semana, dia do mes, mes, ano)
  //myRTC.setDS1302Time(00, 40, 16, 05, 19, 01, 2023);

// Define o pino 8 do arduino como o do sensor DHT11
#define DHT_PIN 8

// Define o modelo do sensor DHT
#define DHTTYPE DHT11

// Define um parâmetro com valor 5000
#define INTERVALO 5000

#define BTHC05_PIN_TXD    5
#define BTHC05_PIN_RXD    6

BTHC05 bthc05(BTHC05_PIN_RXD,BTHC05_PIN_TXD);

int ldr = A0;//Atribui A0 a variável ldr
int valorldr = 0;//Declara a variável valorldr como inteiro

// Definição de lista que contém os números dos pinos digitais onde ficam os LEDs
const int LED[] = {2, 3, 4};
// 2 - Vermelho, 3 - Azul, 4 - Verde

float tempMaxima = 25.00;
float tempMinima = 20.00;

// Cria um objeto para o sensor DHT22 e para o DIsplay LCD
DHT dht(DHT_PIN, DHTTYPE);

// Define instance for LCD I2C display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Função executada apenas uma vez durante o funcionamento
void setup() {
  pinMode(ldr, INPUT);//Define ldr (pino analógico A0) como saída

  for(int x = 0; x < 3; x++) {
  pinMode(LED[x], OUTPUT);

  // Inicializa a serial do arduino e printa a mensagem
  Serial.begin(9600);
  Serial.println("INICIANDO SISTEMA");  

  // Inicializa o bluetooth

  bthc05.begin(9600);
  bthc05.println("Bluetooth On....");

  dht.begin();
  
  // Inicia o Display e limpa todos os caracteres
  lcd.init();
  lcd.clear();
  lcd.backlight();
  // Posiciona o ponteiro na segunda coluna, primeira linha do display
  lcd.setCursor(2, 0); 
  
  // Apresenta a mensagem no display LCD.
  lcd.print("INIT SYSTEM"); 
  // Espera por 3 segundos 
  delay(1000);
}

}

//Função que fica em loop durante a execução do programa
void loop() {

  // Cria variáveis que irão receber os dados do sensor
  float h = 0, t = 0, f = 0;

  // Recebe a umidade
  h = dht.readHumidity();
  // Recebe a temperatura em graus Celsius
  t = dht.readTemperature();
  // Fórmula para converter a temperatura para graus Farenheit
  f = dht.readTemperature(true);

  valorldr=analogRead(ldr);//Lê o valor do sensor ldr e armazena na variável valorldr
  Serial.print("Valor lido pelo LDR = ");//Imprime na serial a mensagem Valor lido pelo LDR
  Serial.println(valorldr);//Imprime na serial os dados de valorldr

  // Lógica para eliminar resultados indesejados
  if(isnan(h) || isnan(t) || t == 0 || h == 0){
    Serial.println(F("Falha na leitura do sensor"));
  }
  // Se estiver tudo ok com a leitura, transmite os dados via bluetooth
  else{
    // Temperatura
    Serial.print(t);
    Serial.print(",");

    // Umidade
    Serial.print(h);
    Serial.print(",");

    // Temperatura em Farenheit
    Serial.print(f);
    Serial.print(",");
    Serial.println();

    // Exibe as informações no display LCD
    lcd.clear();
    lcd.setCursor(2, 0); 
    lcd.print("MONITORANDO");  
    lcd.setCursor(0, 1); 
    lcd.print(t);
    lcd.print("C  ");
    lcd.print(h);
    lcd.print("%RH  ");
    } 

    if (t < tempMinima) {
      // Frio: vermelho
          digitalWrite(LED[0], HIGH);
          digitalWrite(LED[1], LOW);
          digitalWrite(LED[2], LOW);
    } else if (t >= tempMinima && t < tempMaxima) {
      // Temperatura média: azul
        digitalWrite(LED[0], LOW);
        digitalWrite(LED[1], HIGH);
        digitalWrite(LED[2], LOW);
    } else {
      // Temperatura alta: verde
      digitalWrite(LED[0], LOW);
      digitalWrite(LED[1], LOW);
      digitalWrite(LED[2], HIGH);
    }

    //Send sensor data to Bluetooth device  
    myRTC.updateTime(); 

    bthc05.println("Temperatura: " + String(t));
    bthc05.println("Humidade: " + String(h));
    bthc05.println("Dia: " + String(myRTC.dayofmonth) + "," + String(myRTC.month) + "/" + String(myRTC.year));
    bthc05.println("Horário: " + String(myRTC.hours) + ":" + String(myRTC.minutes) + ":" + String(myRTC.seconds));
  // Espera o intervalo definido para fazer a próxima leitura
  delay(INTERVALO);  
}