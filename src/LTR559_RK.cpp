#include "LTR559_RK.h"

static Logger logLTR558("app.LTR559");

LTR559_RK::LTR559_RK() {

}

LTR559_RK::~LTR559_RK() {

}


bool LTR559_RK::enableInterrupts(bool alsEnable, bool psEnable, bool activeHigh) {
    bool bResult = false;

    uint8_t value = 0;
    if (activeHigh) {
        value |= 0b100;
    }
    if (psEnable) {
        value |= 0b001;
    }
    if (alsEnable) {
        value |= 0b010;
    }

    bResult = writeRegister(REG_INTERRUPT, value);

    return bResult;
}

bool LTR559_RK::disableInterrupts() {
    return writeRegister(REG_INTERRUPT, 0x00);
}
    
bool LTR559_RK::getInterruptStatus(bool &alsInterrupt, bool &psInterrupt) {
    uint8_t status = 0;
    bool bResult;

    bResult = readRegister(REG_ALS_PS_STATUS, status);

    alsInterrupt = (bool)((status & 0b1000) != 0);
    psInterrupt = (bool)((status & 0b0010) != 0);

    return bResult;
}


bool LTR559_RK::alsBegin(uint8_t gain) {
    return writeRegister(REG_ALS_CONTR, 0x1 | (gain << 2));
}

bool LTR559_RK::alsStandby() {
    return writeRegister(REG_ALS_CONTR, 0x00);
}

bool LTR559_RK::alsReadData(uint16_t &data) {    
    bool bResult;

    uint16_t data0 = 0;
    uint16_t data1 = 0;
    
    bResult = alsReadData(data0, data1);
    
    data = (uint16_t) (((uint32_t)data0 + (uint32_t)data1) / 2);

    return bResult;
}


bool LTR559_RK::alsReadData(uint16_t &data0, uint16_t &data1) {    
    bool bResult = false;
    uint8_t value[4];

    data0 = data1 = 0;

    for(uint8_t ii = 0; ii < 4; ii++) {
        bResult = readRegister(REG_ALS_DATA_CH1_0 + ii, value[ii]);
        if (!bResult) {
            break;
        }        
    }
    data1 = (uint16_t)(value[0] | (value[1] << 8));
    data0 = (uint16_t)(value[2] | (value[3] << 8));

    // logLTR558.trace("alsReadData data0=%d data1=%d bResult=%d", (int)data0, (int)data1, (int)bResult);

    return bResult;
}

bool LTR559_RK::alsSetThreshold(uint16_t alsLower, uint16_t alsUpper) {
    uint8_t values[4];
    bool bResult = false;

    values[0] = (uint8_t) alsUpper;
    values[1] = (uint8_t) (alsUpper >> 8);
    values[2] = (uint8_t) alsLower;
    values[3] = (uint8_t) ((alsLower >> 8) & 0x0f);

    for(uint8_t ii = 0; ii < 4; ii++) {
        bResult = writeRegister(REG_ALS_THRES_UP_0 + ii, values[ii]);
        if (!bResult) {
            break;
        }
    }
    return bResult;
}


bool LTR559_RK::psBegin(bool enableSaturationIndicator) {
    uint8_t value = 0b11; // Active mode

    if (enableSaturationIndicator) {
        value |= 0b00100000; // 0x20
    }

    return writeRegister(REG_PS_CONTR, value);
}

bool LTR559_RK::psStandby() {
    return writeRegister(REG_PS_CONTR, 0x00);
}

bool LTR559_RK::psReadData(uint16_t &data, bool &saturationIndicator) {
    bool bResult = false;
    uint8_t value[2];

    data = 0;
    saturationIndicator = false;

    for(uint8_t ii = 0; ii < 2; ii++) {
        bResult = readRegister(REG_PS_DATA_0 + ii, value[ii]);
        if (!bResult) {
            break;
        }        
    }
    data = (uint16_t) (value[0] | ((value[1] & 0x07) << 8));
    if (value[1] & 0x80) {
        saturationIndicator = true;
    }

    return bResult;
}

bool LTR559_RK::psSetThreshold(uint16_t psLower, uint16_t psUpper) {
    uint8_t values[4];
    bool bResult = false;

    values[0] = (uint8_t) psUpper;
    values[1] = (uint8_t) ((psUpper >> 8) & 0x0f);
    values[2] = (uint8_t) psLower;
    values[3] = (uint8_t) ((psLower >> 8) & 0x0f);

    for(uint8_t ii = 0; ii < 4; ii++) {
        bResult = writeRegister(REG_PS_THRES_UP_0 + ii, values[ii]);
        if (!bResult) {
            break;
        }
    }

    return bResult;
}


bool LTR559_RK::writeRegister(uint8_t reg, uint8_t value) {
    wire.lock();

    wire.beginTransmission(i2cAddress);

    // logLTR558.trace("write i2cAddress=0x%02x reg=0x%02x value=%02x", (int)i2cAddress, (int)reg, (int)value);

    wire.write(reg);
    wire.write(value);
    int stat = wire.endTransmission(true); 

    wire.unlock();

    if (stat != 0) {
        logLTR558.info("write failed stat=%d i2cAddress=%d reg=%d", stat, (int)i2cAddress, (int)reg);
    }

    return (stat == 0);
}

bool LTR559_RK::readRegister(uint8_t reg, uint8_t &value) {
    value = 0;

    wire.lock();

    wire.beginTransmission(i2cAddress);
    wire.write(reg);
    wire.write(value);
    int stat = wire.endTransmission(false); 

    if (stat == 0) {
        wire.requestFrom(WireTransmission(i2cAddress).quantity(1).stop(true));
        if (wire.available()) {
            value = wire.read();
        }
        else {
            stat = 4; // data byte transfer timeout
        }
    }

    wire.unlock();

    if (stat != 0) {
        logLTR558.info("read failed stat=%d i2cAddress=%d reg=%d", stat, (int)i2cAddress, (int)reg);
    }
    else {
        // logLTR558.trace("read i2cAddress=0x%02x reg=0x%02x value=%02x", (int)i2cAddress, (int)reg, (int)value);
    }

    return (stat == 0);
}
