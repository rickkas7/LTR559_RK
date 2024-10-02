#include "LTR559_RK.h"

SerialLogHandler logHandler(LOG_LEVEL_TRACE);
SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(AUTOMATIC);

LTR559_RK lightSensor;

unsigned long lastReport = 0;
const std::chrono::milliseconds reportPeriod = 1s;

bool interruptFlag;

void setup() {
    // Uncomment this to see debugging messages
    waitFor(Serial.isConnected, 10000); delay(2000);

    Wire.begin();
    
    bool bResult;

    bResult = lightSensor.alsBegin(LTR559_RK::ALS_GAIN_1X);
    Log.info("alsBegin=%d", (int)bResult);

    bResult = lightSensor.psBegin(false);
    Log.info("psBegin=%d", (int)bResult);
}

void loop() {

    if (millis() - lastReport >= reportPeriod.count()) {
        lastReport = millis();

        uint16_t alsData;
        bool bResult;

        bResult = lightSensor.alsReadData(alsData);
        if (bResult) {
            uint16_t psData;
            bool saturationIndicator;
            bResult = lightSensor.psReadData(psData, saturationIndicator);
            if (bResult) {
                Log.info("alsData=%d psData=%d", alsData, psData);
            }
        }
    }

}
