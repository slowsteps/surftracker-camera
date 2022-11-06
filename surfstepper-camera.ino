//#include <Stepper.h>
#include <ArduinoBLE.h>
#include <AccelStepper.h>

const int stepsPerRevolution = 2038;  // change this to fit the number of steps per revolution
// for your motor
int dir = 1;
int turndegrees = 0;
int turnsteps = 0;
//TODO rename from LED ?
BLEService ledService("180A");
BLEByteCharacteristic switchCharacteristic("2A57", BLERead | BLEWrite);
BLEStringCharacteristic degreesCharacteristic("1A57", BLERead | BLEWrite, 20); 

AccelStepper stepper(AccelStepper::FULL4WIRE, 4, 5, 6, 7);


void setup() {
  //TODO What if not connected to IDE
  Serial.begin(9600);
  while (!Serial);
  Serial.println("starting");
  
  stepper.setMaxSpeed(500.0);
  stepper.setAcceleration(50.0);
  stepper.setCurrentPosition(0);

  
  //BLE
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy failed!");
    while (1);
  }
  else {
    //Serial.println("Serial ready");
   }

  BLE.setDeviceName("Nano");
  BLE.setLocalName("Nano33BLE2");
  BLE.setAdvertisedService(ledService);
  ledService.addCharacteristic(switchCharacteristic);
  ledService.addCharacteristic(degreesCharacteristic);
  BLE.addService(ledService);
  switchCharacteristic.writeValue(0);
  degreesCharacteristic.writeValue("hoi");
  BLE.advertise();
  //BLE END
}



void loop() {
 
 handleBLE();
  
}



void handleBLE() {


  
  BLEDevice central = BLE.central();

  int turnsteps = 0;
  
  if (central) {
    Serial.println("Connected to central: ");
    digitalWrite(LED_BUILTIN, HIGH);    
         
    while (central.connected()) {

       //incoming value from phone
      if (degreesCharacteristic.written()) {
            Serial.println("received turn degrees");
            Serial.println(degreesCharacteristic.value());
            turndegrees = degreesCharacteristic.value().toInt();
            turnsteps = round(stepsPerRevolution*turndegrees/360);
            
            stepper.setCurrentPosition(0);
            

            Serial.println("turnsteps:");
            Serial.println(turnsteps);
            //stepper.move(turnsteps);  
            stepper.moveTo(turnsteps);
        }

      stepper.run();
      
      }
      Serial.println("central not connected");
    }
    else {
      //when ble disconnects make sure it does not carry on with old data
      Serial.println("No central");
      digitalWrite(LED_BUILTIN, LOW);     
      stepper.setCurrentPosition(0);
      turnsteps = 0;
      
    }

        
  
}
