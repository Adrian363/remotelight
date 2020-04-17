#include <TheThingsNetwork.h>
#include <CayenneLPP.h>
// Set your AppEUI and AppKey
const char *appEui = "70B3D57ED002D629";
const char *appKey = "DA03698F2D463E8C570B8C418B0AEF2E";

#define loraSerial Serial1
#define debugSerial Serial
#define outPin 2

// Replace REPLACE_ME with TTN_FP_EU868 or TTN_FP_US915
#define freqPlan TTN_FP_US915

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

CayenneLPP lpp(3);

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

  pinMode(outPin, OUTPUT);
  
  lpp.reset(); // remet à 0 le buffer
  lpp.addDigitalOutput(1, 0);
  ttn.sendBytes(lpp.getBuffer(), lpp.getSize());
  
}

void loop()
{
  debugSerial.println("-- LOOP");
 /* CayenneLPP lpp(8);*/
  // Send single byte to poll for incoming messages

  /*lpp.reset(); // remet à 0 le buffer
  lpp.addDigitalOutput(1, 0);
  ttn.sendBytes(lpp.getBuffer(), lpp.getSize());*/
  ttn.poll();

  delay(10000);
}

void message(const uint8_t *payload, size_t size, port_t port)
{
  debugSerial.println("-- MESSAGE");
  debugSerial.print("Received " + String(size) + " bytes on port " + String(port) + ":");
  //debugSerial.println( String(payload[3]));
  if(payload[2]==100){
    
     digitalWrite(outPin, HIGH);
     lpp.reset(); // remet à 0 le buffer
     lpp.addDigitalOutput(1, 1); // permet de renvoyer l'état de la lampe après avoir reçu un messsage
     ttn.sendBytes(lpp.getBuffer(), lpp.getSize());
  }
  else{
    digitalWrite(outPin, LOW);
    lpp.reset(); // remet à 0 le buffer
    lpp.addDigitalOutput(1, 0);
    ttn.sendBytes(lpp.getBuffer(), lpp.getSize());
  }

  debugSerial.println();
}
