#pragma once

#include "driver/i2c.h"

#define SCL_PIN 23
#define SDA_PIN 22
#define I2C_HZ 100000

#define ACK_CHECK 1
#define TX_BUFLEN 0
#define RX_BUFLEN 0
#define MAX_WAIT (1000 / portTICK_PERIOD_MS)

esp_err_t i2c_setup(i2c_port_t i2c_port);
esp_err_t i2c_write_slave_reg(i2c_port_t i2c_port, uint8_t i2c_slave_addr, uint8_t reg, uint8_t* bytes, size_t size);
esp_err_t i2c_read_slave_reg(i2c_port_t i2c_port, uint8_t i2c_slave_addr, uint8_t reg, uint8_t* data, size_t size);