#pragma once

#include <string>

namespace runnable {
namespace dcm {

#define MAX_LENGTH_OF_UPDATE_JSON_BUFFER 200
static const char *TAG = "cloud";

#define MAX_DESIRED_PARAM 2
#define MAX_REPORTED_PARAM 3
#define MAX_LENGTH_URL 256
#define FW_VERSION "V1.0"

/* The event group allows multiple bits for each event,
   but we only care about one event - are we connected
   to the AP with an IP? */
// const int CONNECTED_BIT = BIT0;

/* CA Root certificate, device ("Thing") certificate and device
 * ("Thing") key.
   Example can be configured one of two ways:
   "Embedded Certs" are loaded from files in "certs/" and embedded into the app
 binary. "Filesystem Certs" are loaded from the filesystem (SD card, etc.) See
 example README for more details.
*/
extern const uint8_t aws_root_ca_pem_start[] asm(
    "_binary_aws_root_ca_pem_start");
extern const uint8_t aws_root_ca_pem_end[] asm("_binary_aws_root_ca_pem_end");
extern const uint8_t certificate_pem_crt_start[] asm(
    "_binary_certificate_pem_crt_start");
extern const uint8_t certificate_pem_crt_end[] asm(
    "_binary_certificate_pem_crt_end");
extern const uint8_t private_pem_key_start[] asm(
    "_binary_private_pem_key_start");
extern const uint8_t private_pem_key_end[] asm("_binary_private_pem_key_end");

extern const uint8_t deviceid_txt_start[] asm("_binary_deviceid_txt_start");
extern const uint8_t deviceid_txt_end[] asm("_binary_deviceid_txt_end");

/* AWS IoT Endpoint specific to account and region */
extern const uint8_t endpoint_txt_start[] asm("_binary_endpoint_txt_start");
extern const uint8_t endpoint_txt_end[] asm("_binary_endpoint_txt_end");
// Enable Auto Reconnect functionality. Minimum and
// Maximum time of Exponential backoff are set in
// aws_iot_config.h
//  #AWS_IOT_MQTT_MIN_RECONNECT_WAIT_INTERVAL
//  #AWS_IOT_MQTT_MAX_RECONNECT_WAIT_INTERVAL

}  // namespace dcm
}  // namespace runnable