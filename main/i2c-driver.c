#include "i2c-driver.h"

esp_err_t i2c_setup(i2c_port_t i2c_port) {
    i2c_config_t i2c_conf = {
        .mode = I2C_MODE_MASTER,
        .scl_io_num = SCL_PIN,
        .sda_io_num = SDA_PIN,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_HZ,
        .clk_flags = 0};

    i2c_param_config(i2c_port, &i2c_conf);
    return i2c_driver_install(i2c_port, I2C_MODE_MASTER, RX_BUFLEN, TX_BUFLEN, 0);
}

esp_err_t i2c_write_slave_reg(i2c_port_t i2c_port, uint8_t i2c_slave_addr, uint8_t reg, uint8_t* bytes, size_t size) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    i2c_master_start(cmd);                                                            // start bit
    i2c_master_write_byte(cmd, (i2c_slave_addr << 1) | I2C_MASTER_WRITE, ACK_CHECK);  // send slave address and write = 1
    i2c_master_write_byte(cmd, reg, ACK_CHECK);                                       // send register

    i2c_master_write(cmd, bytes, size, ACK_CHECK);  // and byte

    i2c_master_stop(cmd);  // stop bit
    esp_err_t err = i2c_master_cmd_begin(i2c_port, cmd, MAX_WAIT);
    i2c_cmd_link_delete(cmd);
    return err;
}

esp_err_t i2c_read_slave_reg(i2c_port_t i2c_port, uint8_t i2c_slave_addr, uint8_t reg, uint8_t* data, size_t size) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    i2c_master_start(cmd);                                                            // start
    i2c_master_write_byte(cmd, (i2c_slave_addr << 1) | I2C_MASTER_WRITE, ACK_CHECK);  // send slave address and write = 1
    i2c_master_write_byte(cmd, reg, ACK_CHECK);                                       // send register

    i2c_master_start(cmd);                                                           // send repeated start
    i2c_master_write_byte(cmd, (i2c_slave_addr << 1) | I2C_MASTER_READ, ACK_CHECK);  // set read bit = 1 with address

    if (size > 1) i2c_master_read(cmd, data, size - 1, I2C_MASTER_ACK);  // read size - 1 bytes and send ack after each byte
    i2c_master_read_byte(cmd, data + size - 1, I2C_MASTER_NACK);         // send nack after the final byte to end transaction

    i2c_master_stop(cmd);
    esp_err_t err = i2c_master_cmd_begin(i2c_port, cmd, MAX_WAIT);
    i2c_cmd_link_delete(cmd);
    return err;
}