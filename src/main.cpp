#include "main.h"

//Global variable for current program state
ColorState *state;
//Global variable for retentive storage
Preferences *prefs;

class BLECallbacks : public BLECharacteristicCallbacks 
{ 
  //Bluetooth message recieve handler
  void onWrite(BLECharacteristic *pCharacteristic) 
  { 
    std::string value = pCharacteristic->getValue();
    const char *value_data = value.data();
    uint16_t str_len = value.length();
    
    //returns 1 if message was parsed correctly otherwise 0
    uint8_t msg_valid = state->setState((uint8_t*) value_data, str_len);

    //if message was parsed correctly place current program state to retentive storage
    if(msg_valid)
    {
      std::vector<uint8_t> state_data = state->getData();
      prefs->putBytes("data", state_data.data(), state_data.size());
    }
  } 
};


void setup()
{
  // Serial.begin(115200);

  // Pull down led cos why not.
  pinMode(PIN, INPUT_PULLDOWN);

  // Add letter order to led control object
  pixels.setText(TEXT);
  pixels.begin();

  // initialize globals
  state = new ColorState();
  prefs = new Preferences();
  prefs->begin("store", false);

  // read bytes from storage
  uint8_t temp_data[100];
  uint8_t bytes_read = prefs->getBytes("data", temp_data, 100);

  // place bytes to program state
  state->setState(temp_data, bytes_read);

  // Bluetooth setup function call
  BLE_Setup();
}

// Main loop
void loop()
{
  // get time since program start
  uint32_t current_time = millis();

  // clear led cache
  pixels.clear();

  if(state->mode == 1)
  {
    uint16_t count = state->colorCount();
    for(int i = 0; i < count; i++)
    {
      // Set each letter according to the current state
      pixels.setLetter(i, pixels.ColorHSV(state->getColor(i),255,state->brightness));
    }
  }
  else if(state->mode == 3)
  {
    // get 6000ms rolling number 0 - 255
    uint8_t val = current_time%(6001)/(float)6000*255;

    // generate sine wave depending on current brightness 
    uint16_t val2 = (int)(sine_table[val]/260.0*(float)state->brightness);
    pixels.fill(pixels.ColorHSV(state->getColor(0),255,val2+5));
  }

  //Display leds
  pixels.show();
}


//Bluetooth setup stuff
void BLE_Setup()
{
  BLEDevice::init(BLE_NAME);
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

  pCharacteristic->setCallbacks(new BLECallbacks()); 
  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(10);
  BLEDevice::startAdvertising();
}