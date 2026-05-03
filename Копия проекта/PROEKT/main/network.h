/*
 * network.h - Network status interface
 *
 * Provides simple API to check network connectivity.
 * Actual network initialization is handled by protocol_examples_common.
 */

#pragma once

#include <stdbool.h>

/**
 * @brief Check if device has active network connection with IP address
 *
 * @return true if connected and has valid IP, false otherwise
 */
bool network_is_connected(void);
