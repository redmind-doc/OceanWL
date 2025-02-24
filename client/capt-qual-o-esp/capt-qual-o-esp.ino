float Voltage = 0;
float tdsValue = 0;


void setup() {
  // Initialiser la communication série pour afficher les données
  Serial.begin(115200);

  // Configurer la résolution de l'ADC
  analogReadResolution(12); // Résolution de 12 bits (valeurs entre 0 et 4095)

  // Configurer l'atténuation pour la broche ADC
  //analogSetAttenuation(ADC_11db); // Atténuation pour une plage de tension de 3.3V
}

void loop() {
  // Lire la valeur analogique depuis la broche ADC1_0 (GPIO 1)
  int sensorValue = analogRead(2);

  // Afficher la valeur lue sur le moniteur série
  Serial.print("Valeur du capteur : ");
  Serial.println(sensorValue);


  Voltage = sensorValue*5/4095.0; //Convert analog reading to Voltage
  tdsValue=(133.42*Voltage*Voltage*Voltage - 255.86*Voltage*Voltage + 857.39*Voltage)*0.5;
  Serial.print("TDS value: ");
  Serial.print(tdsValue);
  // Attendre un peu avant la prochaine lecture
  delay(1000);
}