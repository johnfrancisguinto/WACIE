void setup(void) {
  Serial.begin(115200);
  Serial.println("-----------------");
  uint8_t macBT[6];
  esp_read_mac(macBT, ESP_MAC_BT);
}

void loop() {
  uint8_t macBT[6];
  esp_read_mac(macBT, ESP_MAC_BT);
  Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X\r\n", macBT[0], macBT[1], macBT[2], macBT[3], macBT[4], macBT[5]);
  delay(10000);  
}
