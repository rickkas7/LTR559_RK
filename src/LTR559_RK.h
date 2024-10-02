#ifndef __LTR559_RK_H
#define __LTR559_RK_H

#include "Particle.h"

/**
 * @brief LTR559 light and proximity sensor class
 * 
 * Since the chip has a single fixed I2C address of 0x23 you generally only have one per system
 * unless you have multiple I2C busses or an I2C MUX. Typically you would create one object
 * as a global variable.
 */
class LTR559_RK {
public:
    /**
     * @brief Construct a sensor. You will typically have one as a global variable.
     */
    LTR559_RK();

    /**
     * @brief Since this object is typically a global variable, it will never be deleted.
     */
    virtual ~LTR559_RK();


    /**
     * @brief Enable interrupts. This must be done before alsBegin() or psBegin()!
     * 
     * @param activeHigh 
     * @param alsEnable 
     * @param psEnable 
     * @return true if the I2C write succeeded, or false if an error occurred
     * 
     * Interrupt code is not fully tested!
     */
    bool enableInterrupts(bool alsEnable, bool psEnable, bool activeHigh = false);

    /**
     * @brief Disable interrupts
     * 
     * @return true if the I2C write succeeded, or false if an error occurred
     */
    bool disableInterrupts();

    /**
     * @brief Get the status of the ALS and PS interrupts (and clears the interrupt)
     * 
     * @param alsInterrupt 
     * @param psInterrupt 
     * @return true if the I2C write succeeded, or false if an error occurred
     * 
     * Interrupt code is not fully tested!
     */
    bool getInterruptStatus(bool &alsInterrupt, bool &psInterrupt);

    /**
     * @brief Begin using the ALS (automatic light sensor). The default is off.
     * 
     * @param gain Gain constant. ALS_GAIN_1X is the default.
     * @return true if the I2C write succeeded, or false if an error occurred
     */
    bool alsBegin(uint8_t gain);

    /**
     * @brief Put the ALS back in standby (off) mode
     * 
     * @return true if the I2C write succeeded, or false if an error occurred
     */
    bool alsStandby();

    /**
     * @brief Read the current ALS (automatic light sensor) values
     * 
     * @param data Filled in with the light value (0 = dark, 0xffff = maximum brightness)
     * @return true if the I2C write succeeded, or false if an error occurred
     * 
     * This works the same as the version that takes data0 and data1, just takes
     * the mean of the two values for you.
     */
    bool alsReadData(uint16_t &data);

    /**
     * @brief Read the current ALS (automatic light sensor) channel values
     * 
     * @param data0 Filled in with the light value (0 = dark, 0xffff = maximum brightness)
     * @param data1 Filled in with the light value (0 = dark, 0xffff = maximum brightness)
     * @return true if the I2C write succeeded, or false if an error occurred
     * 
     * There are two independent channels. You can just take the mean of the two values.
     * Or at least I think that's how it works.
     */
    bool alsReadData(uint16_t &data0, uint16_t &data1);

    /**
     * @brief Sets the upper and lower thresholds for ALS interrupts
     * 
     * @param alsLower 
     * @param alsUpper 
     * @return true if the I2C write succeeded, or false if an error occurred
     * 
     * Interrupt code is not fully tested!
     */
    bool alsSetThreshold(uint16_t alsLower, uint16_t alsUpper);


    /**
     * @brief Begin using PS (proximity sensor) mode. The default is off.
     * 
     * @param enableSaturationIndicator True to set the PS saturation flag when reading data
     * @return true if the I2C write succeeded, or false if an error occurred
     */
    bool psBegin(bool enableSaturationIndicator);

    /**
     * @brief Put the PS sensor back in standby (off) mode
     * 
     * @return true if the I2C write succeeded, or false if an error occurred
     */
    bool psStandby();

    /**
     * @brief Read the proximity sensor data
     * 
     * @param data 
     * @param saturationIndicator 
     * @return true if the I2C write succeeded, or false if an error occurred
     */
    bool psReadData(uint16_t &data, bool &saturationIndicator);

    /**
     * @brief Sets the upper and lower thresholds for PS interrupts
     * 
     * @param psLower 
     * @param psUpper 
     * @return true if the I2C write succeeded, or false if an error occurred
     * 
     * Interrupt code is not fully tested!
     */
    bool psSetThreshold(uint16_t psLower, uint16_t psUpper);

    /**
     * @brief Write to a register
     * 
     * @param reg Register number (see REG_ constants)
     * @param value Value to set
     * @return true if the I2C write succeeded, or false if an error occurred
     */
    bool writeRegister(uint8_t reg, uint8_t value);

    /**
     * @brief Read a register
     * 
     * @param reg Register number (see REG_ constants)
     * @param value Filled in with the value retrieved
     * @return true if the I2C write succeeded, or false if an error occurred
     */
    bool readRegister(uint8_t reg, uint8_t &value);

    static const uint8_t DEFAULT_I2C_ADDRESS = 0x23; //!< Default I2C address (chip is not configurable)

    static const uint8_t ALS_GAIN_1X = 0b000; //!< Light sensor gain 1x (1 lux to 64K lux, default)
    static const uint8_t ALS_GAIN_2X = 0b001; //!< Light sensor gain x2 (0.5 lux to 32K lux)
    static const uint8_t ALS_GAIN_4X = 0b010; //!< Light sensor gain x4 (0.25 lux to 16K lux)
    static const uint8_t ALS_GAIN_8X = 0b011; //!< Light sensor gain x8 (0.125 lux to 8K lux)
    static const uint8_t ALS_GAIN_48X = 0b110; //!< Light sensor gain x48 (0.02 lux to 1.3K lux)
    static const uint8_t ALS_GAIN_96X = 0b111; //!< Light sensor gain x96 (0.01 lux to 600 lux)

    static const uint8_t REG_ALS_CONTR = 0x80; //!< Ambient light sensor control register
    static const uint8_t REG_PS_CONTR = 0x81; //!< Proximity sensor control register
    static const uint8_t REG_PS_LED = 0x82;
    static const uint8_t REG_PS_N_PULSES = 0x83;
    static const uint8_t REG_PS_MEAS_RATE = 0x84;
    static const uint8_t REG_ALS_MEAS_RATE = 0x85;
    static const uint8_t REG_PART_ID = 0x86;
    static const uint8_t REG_MANUFAC_ID = 0x87;
    static const uint8_t REG_ALS_DATA_CH1_0 = 0x88;
    static const uint8_t REG_ALS_DATA_CH1_1 = 0x89;
    static const uint8_t REG_ALS_DATA_CH0_0 = 0x8a;
    static const uint8_t REG_ALS_DATA_CH0_1 = 0x8b;
    static const uint8_t REG_ALS_PS_STATUS = 0x8c;
    static const uint8_t REG_PS_DATA_0 = 0x8d;
    static const uint8_t REG_PS_DATA_1 = 0x8e;
    static const uint8_t REG_INTERRUPT = 0x8f;
    static const uint8_t REG_PS_THRES_UP_0 = 0x90;
    static const uint8_t REG_PS_THRES_UP_1 = 0x91;
    static const uint8_t REG_PS_THRES_LOW_0 = 0x92;
    static const uint8_t REG_PS_THRES_LOW_1 = 0x93;
    static const uint8_t REG_OFFSET_1 = 0x94;
    static const uint8_t REG_OFFSET_0 = 0x95;
    static const uint8_t REG_ALS_THRES_UP_0 = 0x97;
    static const uint8_t REG_ALS_THRES_UP_1 = 0x98;
    static const uint8_t REG_ALS_THRES_LOW_0 = 0x99;
    static const uint8_t REG_ALS_THRES_LOW_1 = 0x9a;
    static const uint8_t REG_INTERRUPT_PERSIST = 0x9e;

protected:
    uint8_t i2cAddress = DEFAULT_I2C_ADDRESS; //!< I2C address to use for readRegister and writeRegister
    TwoWire &wire = Wire;  //!< Wire interface to use. Default is Wire (primary I2C).
};


#endif // __LTR559_RK_H