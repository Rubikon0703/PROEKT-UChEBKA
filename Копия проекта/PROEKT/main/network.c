/*
 * network.c - Network status implementation
 *
 * Uses ESP-NETIF API to check connection status.
 * Network initialization is delegated to protocol_examples_common.
 */

#include "network.h"
#include "esp_netif.h"
#include "esp_log.h"

static const char *TAG = "network";

bool network_is_connected(void) {
    esp_netif_ip_info_t ip_info;

    /* Try WiFi STA interface first */
    esp_netif_t *netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    if (netif != NULL) {
        if (esp_netif_get_ip_info(netif, &ip_info) == ESP_OK) {
            if (ip_info.ip.addr != 0) {
                return true;
            }
        }
    }

    /* Try Ethernet interface as fallback */
    netif = esp_netif_get_handle_from_ifkey("ETH_DEF");
    if (netif != NULL) {
        if (esp_netif_get_ip_info(netif, &ip_info) == ESP_OK) {
            if (ip_info.ip.addr != 0) {
                return true;
            }
        }
    }

    return false;
}
