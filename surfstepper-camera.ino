//#include <Stepper.h>
#include <ArduinoBLE.h>
#include <AccelStepper.h>

const int stepsPerRevolution = 2038;  // change this to fit the number of steps per revolution
// for your motor
int dir = 1;
int turndegrees = 0;
int turnsteps = 1019;
//TODO rename from LED ?
BLEService ledService("180A");
BLEByteCharacteristic switchCharacteristic("2A57", BLERead | BLEWrite);
BLEStringCharacteristic degreesCharacteristic("1A57", BLERead | BLEWrite, 20); 

// initialize the stepper library on pins 8 through 11:
//Stepper myStepper(stepsPerRevolution, 4, 5, 6, 7);
//AccelStepper stepper(4,5,6,7);
AccelStepper stepper(AccelStepper::FULL4WIRE, 4, 5, 6, 7);


void setup() {
  //What if not connected to IDE
  Serial.begin(9600);
  while (!Serial);
  Serial.println("starting");
  
  //myStepper.setSpeed(5);
  
  stepper.setMaxSpeed(200.0);
  stepper.setAcceleration(50.0);
  stepper.moveTo(1000);
  
  //BLE
  if (!BLE.begin()) {
    //Serial.println("starting Bluetooth® Low Energy failed!");
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
  

 //turnMotor();
 handleBLE();
  
  
}

void turnMotor(int steps) {
  //Serial.println(steps);
  //myStepper.step(steps);
  //stepper.runToNewPosition(steps);

  //delay(1000);
}

void handleBLE() {


  
BLEDevice central = BLE.central();

  int turnsteps = 0;
  
  if (central) {
    Serial.println("Connected to central: ");
  
    while (central.connected()) {

      //STRING
      if (degreesCharacteristic.written()) {
            Serial.println("received raw value Degrees");
            Serial.println(degreesCharacteristic.value());
            turndegrees = degreesCharacteristic.value().toInt();
            turnsteps = round(stepsPerRevolution*turndegrees/360);
        }

    stepper.moveTo(turnsteps);
    stepper.run();
      
      }
    }

//    stepper.moveTo(turnsteps);
//    stepper.run();

    // when the central disconnects, print it out:
    //Serial.print(F("Disconnected from central: "));
    //Serial.println(central.address());
    digitalWrite(LED_BUILTIN, LOW);         // will turn the LED off
  
}
