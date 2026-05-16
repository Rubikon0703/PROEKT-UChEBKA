
#include "mqtt_manager.h"
#include "esp_log.h"
#include "esp_mqtt_client.h"
#include "cJSON.h"
#include <string.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "mqtt";


static esp_mqtt_client_handle_t mqtt_client = NULL;
static bool mqtt_connected = false;


extern const uint8_t mosquitto_org_crt_start[] asm("_binary_mosquitto_org_crt_start");
extern const uint8_t mosquitto_org_crt_end[] asm("_binary_mosquitto_org_crt_end");

static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event) {
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT connected to broker");
            mqtt_connected = true;
            break;

        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGW(TAG, "MQTT disconnected");
            mqtt_connected = false;
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "Subscribed to topic, msg_id=%d", event->msg_id);
            break;

        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "Unsubscribed, msg_id=%d", event->msg_id);
            break;

        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "Published successfully, msg_id=%d", event->msg_id);
            break;

        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "Received: topic=%.*s, data=%.*s",
                     event->topic_len, event->topic,
                     event->data_len, (char *)event->data);
            break;

        case MQTT_EVENT_ERROR:
            ESP_LOGE(TAG, "MQTT error event");
            mqtt_connected = false;
            break;

        default:
            ESP_LOGD(TAG, "MQTT event id=%d", event->event_id);
            break;
    }
    return ESP_OK;
}

esp_err_t mqtt_init(const char *broker_uri, const char *client_id) {
    if (mqtt_client != NULL) {
        ESP_LOGW(TAG, "MQTT client already initialized");
        return ESP_OK;
    }

    if (broker_uri == NULL || client_id == NULL) {
        ESP_LOGE(TAG, "Invalid parameters");
        return ESP_ERR_INVALID_ARG;
    }


    size_t cert_size = (size_t)(mosquitto_org_crt_end - mosquitto_org_crt_start);


    esp_mqtt_client_config_t mqtt_cfg = {
        .broker = {
            .address = {
                .uri = broker_uri,
            },
            .verification = {
                .certificate = (const char *)mosquitto_org_crt_start,
            },
        },
        .credentials = {
            .client_id = client_id,
            .authentication = {
                .username = NULL,
                .password = NULL,
            },
        },
        .network = {
            .timeout_ms = 10000,
            .reconnect_timeout_ms = 5000,
        },
        .session = {
            .last_will = {
                .topic = "sensors/status",
                .msg = "offline",
                .qos = 1,
                .retain = true,
            },
        },
    };


    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    if (mqtt_client == NULL) {
        ESP_LOGE(TAG, "Failed to initialize MQTT client");
        return ESP_FAIL;
    }


    esp_err_t ret = esp_mqtt_client_register_event(
        mqtt_client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to register MQTT event handler");
        esp_mqtt_client_destroy(mqtt_client);
        mqtt_client = NULL;
        return ret;
    }

    ESP_LOGI(TAG, "MQTT client initialized");
    return ESP_OK;
}

esp_err_t mqtt_connect(void) {
    if (mqtt_client == NULL) {
        ESP_LOGE(TAG, "MQTT client not initialized");
        return ESP_ERR_INVALID_STATE;
    }


    esp_err_t ret = esp_mqtt_client_start(mqtt_client);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start MQTT client: %d", ret);
        return ret;
    }


    int timeout_count = 50;
    while (!mqtt_connected && timeout_count > 0) {
        vTaskDelay(pdMS_TO_TICKS(100));
        timeout_count--;
    }

    if (!mqtt_connected) {
        ESP_LOGE(TAG, "MQTT connection timeout");
        return ESP_ERR_TIMEOUT;
    }


    esp_mqtt_client_publish(mqtt_client, "sensors/status", "online", 0, 1, true);

    ESP_LOGI(TAG, "MQTT connected successfully");
    return ESP_OK;
}

esp_err_t mqtt_publish_sensor_data(const sensor_data_t *data, const char *topic) {
    if (mqtt_client == NULL || !mqtt_connected) {
        return ESP_ERR_INVALID_STATE;
    }
    if (data == NULL || topic == NULL) {
        return ESP_ERR_INVALID_ARG;
    }


    cJSON *root = cJSON_CreateObject();
    if (root == NULL) {
        return ESP_ERR_NO_MEM;
    }

    cJSON_AddNumberToObject(root, "timestamp_ms", (double)data->timestamp_ms);
    cJSON_AddNumberToObject(root, "temperature", (double)data->temperature);
    cJSON_AddNumberToObject(root, "light", (double)data->light);
    cJSON_AddNumberToObject(root, "distance", (double)data->distance);
    cJSON_AddBoolToObject(root, "rfid_detected", data->rfid_detected);

    if (data->rfid_detected) {
        cJSON_AddNumberToObject(root, "rfid_uid", (double)data->rfid_uid);
    }


    char *json_str = cJSON_PrintUnformatted(root);
    if (json_str == NULL) {
        cJSON_Delete(root);
        return ESP_ERR_NO_MEM;
    }


    int msg_id = esp_mqtt_client_publish(
        mqtt_client, topic, json_str, 0, 1, false);


    cJSON_free(json_str);
    cJSON_Delete(root);

    if (msg_id < 0) {
        ESP_LOGE(TAG, "Failed to publish message");
        return ESP_FAIL;
    }

    return ESP_OK;
}

void mqtt_disconnect(void) {
    if (mqtt_client != NULL && mqtt_connected) {

        esp_mqtt_client_publish(mqtt_client, "sensors/status", "offline", 0, 1, true);


        esp_mqtt_client_stop(mqtt_client);
        mqtt_connected = false;
        ESP_LOGI(TAG, "MQTT disconnected");
    }
}

void mqtt_deinit(void) {
    if (mqtt_client != NULL) {
        esp_mqtt_client_destroy(mqtt_client);
        mqtt_client = NULL;
        mqtt_connected = false;
        ESP_LOGI(TAG, "MQTT deinitialized");
    }
}
