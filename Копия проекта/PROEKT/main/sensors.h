/*
 * sensors.h - Sensor interface header
 *
 * Provides unified API for reading multiple sensors:
 * - DS18B20 temperature sensor (1-Wire)
 * - BH1750 light sensor (I2C)
 * - HC-SR04 ultrasonic distance sensor (GPIO)
 * - RC522 RFID reader (SPI)
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

/* Simulation mode toggle */
#ifndef CONFIG_SENSOR_SIMULATION_MODE
#define SIMULATION_MODE 0
#else
#define SIMULATION_MODE CONFIG_SENSOR_SIMULATION_MODE
#endif

/* Pin definitions - can be overridden via Kconfig */
#ifndef CONFIG_SENSOR_PIN_TRIG
#define PIN_TRIG 40
#else
#define PIN_TRIG CONFIG_SENSOR_PIN_TRIG
#endif

#ifndef CONFIG_SENSOR_PIN_ECHO
#define PIN_ECHO 41
#else
#define PIN_ECHO CONFIG_SENSOR_PIN_ECHO
#endif

#ifndef CONFIG_SENSOR_PIN_OW
#define PIN_OW 14
#else
#define PIN_OW CONFIG_SENSOR_PIN_OW
#endif

#ifndef CONFIG_SENSOR_I2C_SDA
#define I2C_SDA 8
#else
#define I2C_SDA CONFIG_SENSOR_I2C_SDA
#endif

#ifndef CONFIG_SENSOR_I2C_SCL
#define I2C_SCL 7
#else
#define I2C_SCL CONFIG_SENSOR_I2C_SCL
#endif

#ifndef CONFIG_SENSOR_SPI_SCLK
#define SPI_SCLK 12
#else
#define SPI_SCLK CONFIG_SENSOR_SPI_SCLK
#endif

#ifndef CONFIG_SENSOR_SPI_MISO
#define SPI_MISO 13
#else
#define SPI_MISO CONFIG_SENSOR_SPI_MISO
#endif

#ifndef CONFIG_SENSOR_SPI_MOSI
#define SPI_MOSI 11
#else
#define SPI_MOSI CONFIG_SENSOR_SPI_MOSI
#endif

#ifndef CONFIG_SENSOR_SPI_CS
#define SPI_CS 10
#else
#define SPI_CS CONFIG_SENSOR_SPI_CS
#endif

#ifndef CONFIG_SENSOR_SPI_RST
#define SPI_RST 9
#else
#define SPI_RST CONFIG_SENSOR_SPI_RST
#endif

/* Data structure for sensor readings */
typedef struct {
    float temperature;        /**< Temperature in Celsius (DS18B20) */
    float light;              /**< Light intensity in Lux (BH1750) */
    float distance;           /**< Distance in centimeters (HC-SR04) */
    bool rfid_detected;       /**< True if RFID card detected (RC522) */
    uint32_t rfid_uid;        /**< UID of detected RFID card (placeholder) */
    uint64_t timestamp_ms;    /**< Timestamp in milliseconds */
} sensor_data_t;

/**
 * @brief Initialize all sensors and their communication interfaces
 *
 * @return esp_err_t ESP_OK on success, error code otherwise
 */
esp_err_t sensors_init(void);

/**
 * @brief Read all sensors and populate data structure
 *
 * @param out Pointer to sensor_data_t structure to fill
 * @return esp_err_t ESP_OK on success, error code otherwise
 */
esp_err_t sensors_read(sensor_data_t *out);

/**
 * @brief Check if RFID card is present (convenience function)
 *
 * @return true if card detected, false otherwise
 */
bool sensors_rfid_present(void);

/**
 * @brief Deinitialize sensors and free resources
 */
void sensors_deinit(void);
