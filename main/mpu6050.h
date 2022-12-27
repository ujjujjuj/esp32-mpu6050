#include "esp_log.h"
#include "freertos/FreeRTOS.h"

#define MPU6050_I2C_ADDRESS 0x68
#define ACCEL_SENSITIVITY 16384
#define GYRO_SENSITIVITY 131

#define REG_ACCEL 0x3b
#define REG_GYRO 0x43
#define REG_POWER_1 0x6b
#define REG_WHOAMI 0x75
#define REG_ACCELCONF 0x1b
#define REG_GYROCONF 0x1c

#define I2C_PORT I2C_NUM_0

esp_err_t mpu6050_setup();
esp_err_t mpu6050_read_accel(float* normalized);
esp_err_t mpu6050_read_gyro(float* normalized);