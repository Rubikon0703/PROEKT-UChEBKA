/*
 * mqtt_manager.h - MQTT client interface
 *
 * Handles connection and publishing to MQTT broker with TLS support.
 */

#pragma once

#include "esp_err.h"
#include "sensors.h"

/**
 * @brief Initialize MQTT client with broker settings
 *
 * @param broker_uri Full URI of MQTT broker (e.g., "ssl://broker:8883")
 * @param client_id Unique client identifier for this device
 * @return esp_err_t ESP_OK on success, error code otherwise
 */
esp_err_t mqtt_init(const char *broker_uri, const char *client_id);

/**
 * @brief Connect to MQTT broker
 *
 * @return esp_err_t ESP_OK on successful connection, error code otherwise
 */
esp_err_t mqtt_connect(void);

/**
 * @brief Publish sensor data to specified topic
 *
 * @param data Pointer to sensor_data_t structure
 * @param topic MQTT topic string for publishing
 * @return esp_err_t ESP_OK on success, error code otherwise
 */
esp_err_t mqtt_publish_sensor_data(const sensor_data_t *data, const char *topic);

/**
 * @brief Gracefully disconnect from MQTT broker
 */
void mqtt_disconnect(void);

/**
 * @brief Full deinitialization of MQTT client
 */
void mqtt_deinit(void);
