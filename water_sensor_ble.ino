#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define WATER_SENSOR 15
#define SERVICE_UUID        "12345678-1234-5678-1234-56789abcdef0"
#define CHARACTERISTIC_UUID "abcdef12-3456-7890-abcd-ef1234567890"

BLECharacteristic *pCharacteristic;

void setup() {
    Serial.begin(9600);
    pinMode(WATER_SENSOR, INPUT);

    // Création du serveur BLE
    BLEDevice::init("ESP32-WaterSensor");
    BLEServer *pServer = BLEDevice::createServer();
    
    // Création du service BLE
    BLEService *pService = pServer->createService(SERVICE_UUID);
    
    // Création de la caractéristique BLE
    pCharacteristic = pService->createCharacteristic(
                        CHARACTERISTIC_UUID,
                        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
                      );
    
    // Ajout de la caractéristique au service et démarrage du service
    pService->start();
    
    // Démarrage de la publicité BLE
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    BLEDevice::startAdvertising();
    
    Serial.println("Serveur BLE prêt !");
}

void loop() {
    // Lire la valeur du capteur
    int mouillance = analogRead(WATER_SENSOR);
    String state;

    if (mouillance > 4000) {
        state = "sec";
    } else if (mouillance < 1500) {
        state = "partiellement mouillé";
    } else {
        state = "immergé";
    }

    // Mise à jour de la caractéristique BLE
    pCharacteristic->setValue(state.c_str());
    pCharacteristic->notify();  // Envoie la nouvelle valeur aux clients connectés
    Serial.println(state);

    delay(500);
}
