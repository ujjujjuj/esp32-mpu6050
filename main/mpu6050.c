#include "mpu6050.h"

#include "i2c-driver.h"

static inline void swap_bytes(uint8_t* b1, uint8_t* b2) {
    uint8_t tmp = *b1;
    *b1 = *b2;
    *b2 = tmp;
}

esp_err_t mpu6050_setup() {
    esp_err_t err;

    err = i2c_setup(I2C_PORT);

    uint8_t byte_w = 0;
    uint8_t byte_r;

    i2c_read_slave_reg(I2C_PORT, MPU6050_I2C_ADDRESS, REG_WHOAMI, &byte_r, 1);  // read the "who am i" register
    if (byte_r != MPU6050_I2C_ADDRESS) return ESP_FAIL;

    i2c_write_slave_reg(I2C_PORT, MPU6050_I2C_ADDRESS, REG_POWER_1, &byte_w, 1);    // wake up from sleep
    i2c_write_slave_reg(I2C_PORT, MPU6050_I2C_ADDRESS, REG_ACCELCONF, &byte_w, 1);  // set accel config to +/-2g
    i2c_write_slave_reg(I2C_PORT, MPU6050_I2C_ADDRESS, REG_GYROCONF, &byte_w, 1);   // set gyro config to +/-250deg
    return err;
}

esp_err_t mpu6050_read_accel(float* normalized) {
    uint8_t data[6];
    esp_err_t err = i2c_read_slave_reg(I2C_PORT, MPU6050_I2C_ADDRESS, REG_ACCEL, data, 6);

    // fix endianness
    swap_bytes(data, data + 1);
    swap_bytes(data + 2, data + 3);
    swap_bytes(data + 4, data + 5);

    int16_t* short_data = (int16_t*)data;
    normalized[0] = (float)short_data[0] / ACCEL_SENSITIVITY;
    normalized[1] = (float)short_data[1] / ACCEL_SENSITIVITY;
    normalized[2] = (float)short_data[2] / ACCEL_SENSITIVITY;

    return err;
}

esp_err_t mpu6050_read_gyro(float* normalized) {
    uint8_t data[6];
    esp_err_t err = i2c_read_slave_reg(I2C_PORT, MPU6050_I2C_ADDRESS, REG_GYRO, data, 6);

    swap_bytes(data, data + 1);
    swap_bytes(data + 2, data + 3);
    swap_bytes(data + 4, data + 5);

    int16_t* short_data = (int16_t*)data;
    normalized[0] = (float)short_data[0] / GYRO_SENSITIVITY;
    normalized[1] = (float)short_data[1] / GYRO_SENSITIVITY;
    normalized[2] = (float)short_data[2] / GYRO_SENSITIVITY;

    return err;
}