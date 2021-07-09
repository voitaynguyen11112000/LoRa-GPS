#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "Nguyen"
#define WLAN_PASS       "12345678"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "pepegaswiper69"
#define AIO_KEY         "aio_VvnY35pATsuudFEGTtcYBG0xRPuA"

/******************************************************************************/

#define SIZE_BUFF  128
#define WAIT_HEADER 1
#define GET_BUFF 2
#define SEND_MQTT 3

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish feed = Adafruit_MQTT_Publish(&mqtt, "pepegaswiper69/feeds/loragps");

char buff[SIZE_BUFF];
char msg[SIZE_BUFF];
uint16_t msg_index = 0;
uint16_t pr_index = 0;
uint16_t index_b = 0;
uint8_t state = WAIT_HEADER;

void mqttConnect(void);
void moveToBuffer(void);
void processBuffer(void);


void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
}

uint32_t value = 0;
unsigned long previousMillis = 0; 
const long interval = 100;

void loop() {

  mqttConnect();

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    moveToBuffer();
    processBuffer();
  }
}

void mqttConnect(void) {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}

void moveToBuffer(void){
  while (Serial.available() > 0){
    buff[index_b] = Serial.read();
    index_b = (index_b + 1) % SIZE_BUFF;
  }
}

void processBuffer(void){
  while (pr_index != index_b){
    char c = buff[pr_index];
    pr_index = (pr_index + 1) % SIZE_BUFF;
    switch (state){
      case WAIT_HEADER:
        if (c == 0xfe){
          state = GET_BUFF;
        }
        break;
      case GET_BUFF:
        if (c == 0xff){
          msg[msg_index] = '\0';
          if ((msg_index >= 6) && !strncmp(msg, "$GNRMC", 6)){
            state = SEND_MQTT;
          } else {
            msg_index = 0;
            state = WAIT_HEADER;
          }
        } else {
          msg[msg_index] = c;
          msg_index++;
        }
        break;
      case SEND_MQTT:
        if (feed.publish(msg)){
          Serial.println("Sent to MQTT");
          msg_index = 0;
          state = WAIT_HEADER;
        } else {
          Serial.println("Failed to send, sending again...");
        }
        break;
      default:
        break;
    }
  }
}
