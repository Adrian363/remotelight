#include <TheThingsNetwork.h>

// Set your AppEUI and AppKey
const char *appEui = "70B3D57ED002D629";
const char *appKey = "DA03698F2D463E8C570B8C418B0AEF2E";

#define loraSerial Serial1
#define debugSerial Serial

// Replace REPLACE_ME with TTN_FP_EU868 or TTN_FP_US915
#define freqPlan TTN_FP_US915

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

#define pinOutPut 1
void setup()
{
  loraSerial.begin(57600);
  debugSerial.begin(9600);

  
  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000)
    ;

  // Set callback for incoming messages
  ttn.onMessage(message);

  debugSerial.println("-- STATUS");
  ttn.showStatus();

  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);

  pinMode(pinOutPut, OUTPUT);
}

void loop()
{
  debugSerial.println("-- LOOP");

  digitalWrite(pinOutPut, HIGH); // Met la sortie à 1 donc la lampe s'allume
  Serial.println("Lampe allumée");
  delay(10000);
  digitalWrite(pinOutPut, LOW); // Met la sortie à 0 donc la lampe s'éteind
  delay (10000);

  // Send single byte to poll for incoming messages
  ttn.poll();
  
}

void message(const uint8_t *payload, size_t size, port_t port)
{
  debugSerial.println("-- MESSAGE");
  debugSerial.print("Received " + String(size) + " bytes on port " + String(port) + ":");

  if (payload[0]==1){
    digitalWrite(pinOutPut, HIGH); // Met la sortie à 1 donc la lampe s'allume
    Serial.println('Lampe allumée');
  }
  else{
    digitalWrite(pinOutPut, LOW); // Met la sortie à 1 donc la lampe s'allume
    Serial.println('Lampe éteinte');
  }

  debugSerial.println();
}
