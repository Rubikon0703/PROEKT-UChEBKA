

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"


#ifndef CONFIG_SENSOR_SIMULATION_MODE
#define SIMULATION_MODE 0
#else
#define SIMULATION_MODE CONFIG_SENSOR_SIMULATION_MODE
#endif


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
#define PIN_DOOR 15

typedef struct {
    float temperature;
    float light;
    float distance;
    bool rfid_detected;
    uint32_t rfid_uid;
    bool door_open;
    uint64_t timestamp_ms;
} sensor_data_t;


esp_err_t sensors_init(void);


esp_err_t sensors_read(sensor_data_t *out);


bool sensors_rfid_present(void);


void sensors_deinit(void);
