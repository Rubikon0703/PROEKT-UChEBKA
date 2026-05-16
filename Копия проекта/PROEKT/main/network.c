
#include "network.h"
#include "esp_netif.h"
#include "esp_log.h"

static const char *TAG = "network";

bool network_is_connected(void) {
    esp_netif_ip_info_t ip_info;


    esp_netif_t *netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    if (netif != NULL) {
        if (esp_netif_get_ip_info(netif, &ip_info) == ESP_OK) {
            if (ip_info.ip.addr != 0) {
                return true;
            }
        }
    }


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
