#include "../DuckDetect.h"

void DuckDetect::setupWithDefaults(String ssid, String password) {
  Duck::setupWithDefaults(ssid, password);
  setupRadio();
}

void DuckDetect::run() {
  handleOtaUpdate();
  if (getReceiveFlag()) {
    setReceiveFlag(false);
    duckutils::setDuckInterrupt(false);
    int pSize = duckLora->handlePacket();
    if (pSize > 0) {
      for (int i = 0; i < pSize; i++) {
        if (duckLora->getTransmitedByte(i) == iamhere_B) {
          Serial.println("[DuckDetect] run() - got ping response!");
          rssiCb(duckLora->getRSSI());
        }
      }
    }
    duckutils::setDuckInterrupt(true);
    startReceive();
  }
}

void DuckDetect::sendPing(bool startReceive) {
  duckLora->couple(ping_B, "0");
  startTransmit();

  if (startReceive) {
    this->startReceive();
  }
}