from bluezero import Peripheral, Service, Characteristic, Descriptor
from bluezero.server import BluetoothService

# Définir les UUIDs pour le service et la caractéristique
SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9331914b8"
CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8"

class MyService(BluetoothService):
    def __init__(self, bus_name, index):
        BluetoothService.__init__(self, bus_name, index, SERVICE_UUID, True)
        self.add_characteristic(MyCharacteristic(self))

class MyCharacteristic(Characteristic):
    def __init__(self, service):
        Characteristic.__init__(
            self, service, CHARACTERISTIC_UUID,
            ['read', 'write', 'notify'],
        )
        self.value = b"Hello ESP32"

    def ReadValueCallback(self, options):
        print("Value read: ", self.value)
        return self.value

    def WriteValueCallback(self, value, options):
        print("Value received: ", value)
        self.value = value

# Créer et démarrer le périphérique BLE
peripheral = Peripheral("RaspberryPi_BLE", ["1234"])
service = MyService(peripheral.bus_name, 1)
peripheral.add_service(service)
peripheral.run()
