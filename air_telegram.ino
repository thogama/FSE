#include <LiquidCrystal.h>
#include <UniversalTelegramBot.h>
#include <WiFi.h> // Biblioteca para ESP32

const char* ssid = "IDMNET-WIFI_AJU"; // usar dados moveis
const char* password = "oliveira987";


LiquidCrystal lcd(21, 22, 16, 17, 18, 19); 
UniversalTelegramBot bot("7141004931:AAE-mAGt3XA9NqwKWiqlY3y6I3uSNuQtEZM");  // Fireman323Bot

int sensor = 36; // Pino analógico para o sensor
int val = 0;
int limit = 40;

void setup() {
  Serial.begin(9600);
  // declaring LEDs pins as OUTPUTS Pins and turned them off at the beginning.
  pinMode(2, OUTPUT); // Pino para LED Vermelho
  pinMode(4, OUTPUT); // Pino para LED Verde

  digitalWrite(4, LOW); // Desliga o LED Verde
  digitalWrite(2, LOW); // Desliga o LED Vermelho

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Air Monitoring");
  lcd.setCursor(0, 1);
  lcd.print("System");
  delay(500); 
  lcd.clear();

  // Conectar ao Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  val = analogRead(sensor);         // Inicializa o valor da leitura do sensor.
  val = map(val, 306, 750, 0, 100); // Mapeia a leitura do sensor para valores de 0 a 100.
  Serial.println(val);              // Imprime a leitura do sensor no Monitor Serial.

  // Se um gás prejudicial for detectado.
  if (val > limit) {
    lcd.setCursor(0, 0);
    lcd.print("Harmful Gas is");  // Imprime uma mensagem no LCD.
    lcd.setCursor(0, 1);
    lcd.print("Detected");
    digitalWrite(2, HIGH);        // Liga o LED Vermelho.
    digitalWrite(4, LOW);         // Desliga o LED Verde.
    tone(25, 1000);               // Ativa o Buzzer.
    delay(100);                   // Som ativado por 100 ms.
    noTone(25);                   // Desativa o Buzzer.
    delay(100);
    
    // Envia uma mensagem para o Telegram
    String message = "Harmful gas detected! Air Quality: ";
    message += val;
    bot.sendMessage("1454854984", message); // Substitua "ID_DO_CHAT" pelo ID do chat
  }
  else { 
    lcd.clear();                  // Limpa a mensagem do LCD.
    noTone(25);                   // Desativa o Buzzer.
    digitalWrite(2, LOW);         // Desliga o LED Vermelho.
    digitalWrite(4, HIGH);        // Liga o LED Verde.
  } 
}
