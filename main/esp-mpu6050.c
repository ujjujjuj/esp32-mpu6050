#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mpu6050.h"

const char* TAG = "MPU6050";

void display_readings() {
    float accel_data[3];
    float gyro_data[3];
    for (;;) {
        mpu6050_read_accel(accel_data);
        mpu6050_read_gyro(gyro_data);
        ESP_LOGI(TAG, "Accel X: %12.5f  Y: %12.5f  Z: %12.5f", accel_data[0], accel_data[1], accel_data[2]);
        ESP_LOGI(TAG, "Gyro  X: %12.5f  Y: %12.5f  Z: %12.5f", gyro_data[0], gyro_data[1], gyro_data[2]);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void app_main(void) {
    if(mpu6050_setup() != ESP_OK){
        ESP_LOGE("MPU6050", "Failed to set up device! Check your wiring");
        return;
    }
    display_readings();
}
