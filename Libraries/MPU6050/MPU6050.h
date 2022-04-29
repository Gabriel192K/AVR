#ifndef MPU6050_H
#define MPU6050_H
#include "TWI.h"

// In work.. //

#define MPU6050_ADDR 0x68
// REGISTERS
// ******************************************************************************************************************
#define MPU6050_ACCEL_CONFIG 0x1C
#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_ACCEL_XOUT_L 0x3C
#define MPU6050_ACCEL_YOUT_H 0x3D
#define MPU6050_ACCEL_YOUT_L 0x3E
#define MPU6050_ACCEL_ZOUT_H 0x3F
#define MPU6050_ACCEL_ZOUT_L 0x40
#define MPU6050_TEMP_OUT_H   0x41
#define MPU6050_TEMP_OUT_L   0x42
#define MPU6050_GYRO_XOUT_H  0x43
#define MPU6050_GYRO_XOUT_L  0x44
#define MPU6050_GYRO_YOUT_H  0x45
#define MPU6050_GYRO_YOUT_L  0x46
#define MPU6050_GYRO_ZOUT_H  0x47
#define MPU6050_GYRO_ZOUT_L  0x48
#define MPU6050_PWR_MGMT_1   0x6B
#define MPU6050_PWR_MGMT_2   0x6C
#define MPU6050_WHO_AM_I     0x75

// MPU6050 ACCELERATION CONFIG BITS
// ******************************************************************************************************************
#define MPU6050_ACCEL_CONFIG_XA_ST_BIT     0x07 // X AXIS SELF TEST
#define MPU6050_ACCEL_CONFIG_YA_ST_BIT     0x06 // Y AXIS SELF TEST
#define MPU6050_ACCEL_CONFIG_ZA_ST_BIT     0x05 // Z AXIS SELF TEST
#define MPU6050_ACCEL_CONFIG_AFS_SEL_BIT_1 0x04 // AFS BIT 1
#define MPU6050_ACCEL_CONFIG_AFS_SEL_BIT_0 0x03 // AFS BIT 0
// MPU6050 POWER MANAGEMENT 1 BITS
// ******************************************************************************************************************
#define MPU6050_PWR_MGMT_1_DEVICE_RESET_BIT 0x07 // RESET DEVICE
#define MPU6050_PWR_MGMT_1_SLEEP_BIT        0x06 // SLEEP
#define MPU6050_PWR_MGMT_1_CYCLE_BIT        0x05 // CYCLE BETWEEN SLEEP AND WAKE
#define MPU6050_PWR_MGMT_1_TEMP_DIS_BIT     0x03 // DISABLE TEMP SENSOR
#define MPU6050_PWR_MGMT_1_CLKSEL_BIT_2     0x02 // CLOCK SELECT BIT 2
#define MPU6050_PWR_MGMT_1_CLKSEL_BIT_1     0x01 // CLOCK SELECT BIT 1
#define MPU6050_PWR_MGMT_1_CLKSEL_BIT_0     0x00 // CLOCK SELECT BIT 0
// MPU6050 POWER MANAGMENT 2 BITS
// ******************************************************************************************************************
#define MPU6050_PWR_MGMT_2_LP_WAKE_CTRL_BIT_1 0x07 // LOW POWER WAKE CONTROL BIT 1
#define MPU6050_PWR_MGMT_2_LP_WAKE_CTRL_BIT_0 0x06 // LOW POWER WAKE CONTROL BIT 0
#define MPU6050_PWR_MGMT_2_STBY_XA_BIT        0x05 // ACCEL X AXIS STANDBY
#define MPU6050_PWR_MGMT_2_STBY_YA_BIT        0x04 // ACCEL Y AXIS STANDBY
#define MPU6050_PWR_MGMT_2_STBY_ZA_BIT        0x03 // ACCEL Z AXIS STANDBY
#define MPU6050_PWR_MGMT_2_STBY_XG_BIT        0x02 // GYRO X AXIS STANDBY
#define MPU6050_PWR_MGMT_2_STBY_YG_BIT        0x01 // GYRO Y AXIS STANDBY
#define MPU6050_PWR_MGMT_2_STBY_ZG_BIT        0x00 // GYRO Z AXIS STANDBY
// MPU6050 WHO AM I BITS
// ******************************************************************************************************************
#define MPU6050_WHO_AM_I_BIT 0x06 // 6 BIT OUT OF THE 7 BIT TWI ADDRESS

// MPU6050 ACCELERATION CONFIG FUNCTIONS
// ******************************************************************************************************************
#define MPU6050_ACCEL_CONFIG_AFS_2G  0x00                                                                                  // +- 2g
#define MPU6050_ACCEL_CONFIG_AFS_4G  (1 << MPU6050_ACCEL_CONFIG_AFS_SEL_BIT_0)                                             // +- 4g
#define MPU6050_ACCEL_CONFIG_AFS_8G  (1 << MPU6050_ACCEL_CONFIG_AFS_SEL_BIT_1)                                             // +- 8g
#define MPU6050_ACCEL_CONFIG_AFS_16G (1 << MPU6050_ACCEL_CONFIG_AFS_SEL_BIT_1) | (1 << MPU6050_ACCEL_CONFIG_AFS_SEL_BIT_0) // +- 16g
// MPU6050 POWER MANAGEMENT 1 FUNCTIONS
#define MPU6050_PWR_MGMT_1_CLKSEL_INT_8_MHz 0x00                                                                                                                     // INTERNAL 8 MHz OSC
#define MPU6050_PWR_MGMT_1_CLKSEL_STOP      (1 << MPU6050_PWR_MGMT_1_CLKSEL_BIT_2) | (1 << MPU6050_PWR_MGMT_1_CLKSEL_BIT_1) | (1 << MPU6050_PWR_MGMT_1_CLKSEL_BIT_0) // STOP CLOCK
// MPU6050 POWER MANAGMENT 2 FUNCTIONS
#define MPU6050_PWR_MGMT_2_LP_WAKE_CTRL_1_25_Hz 0x00                                                                                        // 1.25 Hz
#define MPU6050_PWR_MGMT_2_LP_WAKE_CTRL_5_Hz    (1 << MPU6050_PWR_MGMT_2_LP_WAKE_CTRL_BIT_0)                                                // 5 Hz
#define MPU6050_PWR_MGMT_2_LP_WAKE_CTRL_20_Hz   (1 << MPU6050_PWR_MGMT_2_LP_WAKE_CTRL_BIT_1)                                                // 20 Hz
#define MPU6050_PWR_MGMT_2_LP_WAKE_CTRL_40_Hz   (1 << MPU6050_PWR_MGMT_2_LP_WAKE_CTRL_BIT_1) | (1 << MPU6050_PWR_MGMT_2_LP_WAKE_CTRL_BIT_0) // 40 Hz



void    MPU6050_begin(void);
uint8_t MPU6050_isConnected(void);
void    MPU6050_getAcceleration(int16_t* x, int16_t* y, int16_t* z);
void    MPU6050_getTemperature(int16_t* t);
uint8_t MPU6050_getID(void);

void MPU6050_begin(void)
{
	TWI_beginTransmission(MPU6050_ADDR);
	TWI_writeData(MPU6050_PWR_MGMT_1);
	TWI_writeData(0x00);
	TWI_endTransmission();
}

uint8_t MPU6050_isConnected(void)
{
	uint8_t id = MPU6050_getID();
	return (id == MPU6050_ADDR);
}

void MPU6050_getAcceleration(int16_t* x, int16_t* y, int16_t* z)
{
	TWI_beginTransmission(MPU6050_ADDR);
	TWI_writeData(MPU6050_ACCEL_XOUT_H);
	TWI_requestFrom(MPU6050_ADDR);
	*x = (int16_t)(TWI_read(ACK) << 8 | TWI_read(ACK));
	*y = (int16_t)(TWI_read(ACK) << 8 | TWI_read(ACK));
	*z = (int16_t)(TWI_read(ACK) << 8 | TWI_read(NACK));
	TWI_endTransmission();
}

void MPU6050_getTemperature(int16_t* t)
{
	TWI_beginTransmission(MPU6050_ADDR);
	TWI_writeData(MPU6050_TEMP_OUT_H);
	TWI_requestFrom(MPU6050_ADDR);
	*t = (int16_t)(TWI_read(ACK) << 8 | TWI_read(NACK));
	*t /= 340; *t += 36.53;
	TWI_endTransmission();
}

uint8_t MPU6050_getID(void)
{
	TWI_beginTransmission(MPU6050_ADDR);
	TWI_writeData(MPU6050_WHO_AM_I);
	TWI_requestFrom(MPU6050_ADDR);
	uint8_t id = TWI_read(NACK);
	TWI_endTransmission();
	return id;
}

// 0100 0111 71
// 0110 1000 68
#endif