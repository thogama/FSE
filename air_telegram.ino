#include <WiFi.h>
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>

const char* ssid = "IDMNET-WIFI_AJU"; // Nome da rede WiFi
const char* password = "oliveira987"; // Senha da rede WiFi
const char *telegramToken = "7141004931:AAE-mAGt3XA9NqwKWiqlY3y6I3uSNuQtEZM"; // Token do bot no Telegram

WiFiClientSecure secured_client; // Cliente WiFi seguro para conexão com o Telegram
UniversalTelegramBot bot(telegramToken, secured_client); // Objeto do bot do Telegram
bool mensagemEnviada = false; // Flag para verificar se a mensagem foi enviada

int sensor = 36; // Pino analógico para o sensor de gás
int limit = 40; // Limite para considerar a detecção de gás prejudicial

void setup() {
  Serial.begin(9600); // Inicialização da comunicação serial
  connectToWiFi(); // Conectar-se à rede WiFi
  sendMessageToTelegram(); // Enviar mensagem de inicialização para o Telegram
}

void loop() {
  int val = analogRead(sensor); // Realiza a leitura do sensor de gás
  val = map(val, 306, 750, 0, 100); // Mapeia a leitura do sensor para valores de 0 a 100

  if (val > limit) { // Se um gás prejudicial for detectado
    String chat_id = "1454854984"; // ID do chat do Telegram para enviar a mensagem
    String text = "Harmful gas detected! Air Quality: " + String(val); // Texto da mensagem a ser enviada
    sendMessageToTelegram(chat_id, text); // Enviar mensagem para o Telegram
    delay(5000); // Aguardar 5 segundos para evitar envio repetido de mensagens
  }

  delay(1000); // Esperar 1 segundo antes da próxima leitura do sensor
}

void connectToWiFi() {
  WiFi.begin(ssid, password); // Iniciar conexão com a rede WiFi
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Adicionar certificado raiz para api.telegram.org
  while (WiFi.status() != WL_CONNECTED) { // Aguardar até que a conexão seja estabelecida
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado ao WiFi!"); // Indicar que a conexão WiFi foi estabelecida
}

void sendMessageToTelegram(String chat_id, String text) {
  bool mensagemEnviada = bot.sendMessage(chat_id, text); // Tentar enviar a mensagem para o Telegram
  if (mensagemEnviada) {
    Serial.println("Mensagem enviada com sucesso!"); // Indicar que a mensagem foi enviada com sucesso
  } else {
    Serial.println("Falha ao enviar mensagem!"); // Indicar que houve uma falha ao enviar a mensagem
  }
}
