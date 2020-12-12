#pragma once

#include <string>

namespace runnable {
namespace dcm {

extern const uint8_t upgrade_server_cert_pem_start[] asm(
    "_binary_github_server_cert_start");
extern const uint8_t upgrade_server_cert_pem_end[] asm(
    "_binary_github_server_cert_end");

}  // namespace dcm
}  // namespace runnable