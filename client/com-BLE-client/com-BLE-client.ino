#include <BLEDevice.h>
#include <BLEClient.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

// Définir les UUIDs pour le service et la caractéristique
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9331914b8"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLEClient* pClient;
BLERemoteCharacteristic* pRemoteCharacteristic;
bool connectToServer(BLEAdvertisedDevice* pServer);

class MyClientCallback : public BLEClientCallbacks {
    void onConnect(BLEClient* pClient) {
      Serial.println("Connected to server");
    }

    void onDisconnect(BLEClient* pClient) {
      Serial.println("Disconnected from server");
    }
};

bool connectToServer(BLEAdvertisedDevice* pServer) {
  Serial.print("Connecting to ");
  Serial.println(pServer->getAddress().toString().c_str());

  pClient = BLEDevice::createClient();
  pClient->setClientCallbacks(new MyClientCallback());

  pClient->connect(pServer);

  BLERemoteService* pRemoteService = pClient->getService(SERVICE_UUID);
  if (pRemoteService == nullptr) {
    Serial.print("Failed to find service UUID: ");
    Serial.println(SERVICE_UUID);
    pClient->disconnect();
    return false;
  }

  pRemoteCharacteristic = pRemoteService->getCharacteristic(CHARACTERISTIC_UUID);
  if (pRemoteCharacteristic == nullptr) {
    Serial.print("Failed to find characteristic UUID: ");
    Serial.println(CHARACTERISTIC_UUID);
    pClient->disconnect();
    return false;
  }

  return true;
}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      Serial.print("Advertised Device: ");
      Serial.println(advertisedDevice.getAddress().toString().c_str());

      if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(SERVICE_UUID)) {
        BLEDevice::getScan()->stop();
        connectToServer(&advertisedDevice);
      }
    }
};

void setup() {
  Serial.begin(115200);
  BLEDevice::init("");
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1345);
  pBLEScan->setWindow(43);
  pBLEScan->start(5, false);
}

void loop() {
  if (pClient && pClient->isConnected()) {
    pRemoteCharacteristic->writeValue("Hello Raspberry Pi");
    delay(1000);
  }
}
