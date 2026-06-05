#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <ModbusMaster.h>
#include <DHT.h>

// ================= WIFI =================
#define WIFI_SSID "Enter your WIFI SSID here"
#define WIFI_PASSWORD "Enter your wifi password here"

// ================= FIREBASE =================
#define API_KEY "Enter your own API Key here"
#define DATABASE_URL "Enter the database url of your firebase db here"

// ================= pH =================
#define PH_PIN 34

// ================= DHT11 =================
#define DHTPIN 2
#define DHTTYPE DHT11

// ================= RS485 =================
#define MAX485_DE_RE 4
#define RXD2 16
#define TXD2 17

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

DHT dht(DHTPIN, DHTTYPE);
ModbusMaster node;

// ---------------- RS485 ----------------
void preTransmission()
{
digitalWrite(MAX485_DE_RE, HIGH);
}

void postTransmission()
{
digitalWrite(MAX485_DE_RE, LOW);
}

// ---------------- pH ----------------
float readPH()
{
int raw = analogRead(PH_PIN);

float voltage = raw * (3.3 / 4095.0);

float ph = 7 + ((2.5 - voltage) / 0.18);

return ph;
}

// ---------------- NPK ----------------
uint16_t readNitrogen()
{
uint8_t result = node.readHoldingRegisters(0x001E, 1);

if (result == node.ku8MBSuccess)
return node.getResponseBuffer(0);

return 0;
}

uint16_t readPhosphorus()
{
uint8_t result = node.readHoldingRegisters(0x001F, 1);

if (result == node.ku8MBSuccess)
return node.getResponseBuffer(0);

return 0;
}

uint16_t readPotassium()
{
uint8_t result = node.readHoldingRegisters(0x0020, 1);

if (result == node.ku8MBSuccess)
return node.getResponseBuffer(0);

return 0;
}

void setup()
{
Serial.begin(115200);

analogReadResolution(12);

dht.begin();

// RS485
pinMode(MAX485_DE_RE, OUTPUT);
digitalWrite(MAX485_DE_RE, LOW);

Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

node.begin(1, Serial2);
node.preTransmission(preTransmission);
node.postTransmission(postTransmission);

// WiFi
Serial.println("Connecting WiFi...");
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

while (WiFi.status() != WL_CONNECTED)
{
Serial.print(".");
delay(500);
}

Serial.println();
Serial.println("WiFi Connected");
Serial.print("IP: ");
Serial.println(WiFi.localIP());

// Firebase
config.api_key = API_KEY;
config.database_url = DATABASE_URL;

if (Firebase.signUp(&config, &auth, "", ""))
{
Serial.println("Firebase SignUp OK");
}
else
{
Serial.println(config.signer.signupError.message.c_str());
}

Firebase.begin(&config, &auth);
Firebase.reconnectWiFi(true);

Serial.println("Firebase Ready");
}

void loop()
{
if (!Firebase.ready())
{
Serial.println("Firebase not ready");
delay(1000);
return;
}

float ph = readPH();

float temperature = dht.readTemperature();
float humidity = dht.readHumidity();

int nitrogen = readNitrogen();
int phosphorus = readPhosphorus();
int potassium = readPotassium();

Serial.println("-------------------------");

Serial.print("pH: ");
Serial.println(ph);

Serial.print("Temperature: ");
Serial.println(temperature);

Serial.print("Humidity: ");
Serial.println(humidity);

Serial.print("Nitrogen: ");
Serial.println(nitrogen);

Serial.print("Phosphorus: ");
Serial.println(phosphorus);

Serial.print("Potassium: ");
Serial.println(potassium);

Firebase.RTDB.setFloat(&fbdo, "/Agrimind/pH", ph);
Firebase.RTDB.setFloat(&fbdo, "/Agrimind/Temperature", temperature);
Firebase.RTDB.setFloat(&fbdo, "/Agrimind/Humidity", humidity);

Firebase.RTDB.setInt(&fbdo, "/Agrimind/Nitrogen", nitrogen);
Firebase.RTDB.setInt(&fbdo, "/Agrimind/Phosphorus", phosphorus);
Firebase.RTDB.setInt(&fbdo, "/Agrimind/Potassium", potassium);

Serial.println("Uploaded To Firebase");

delay(5000);
}
