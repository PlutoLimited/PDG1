#include "delegate/aws/aws.h"

namespace runnable {
namespace dcm {
namespace aws {
bool CAWSDelegate::shadowUpdateInProgress = false;
bool CAWSDelegate::ota_update_done = false;
uint8_t CAWSDelegate::light_state = 0U;
uint8_t CAWSDelegate::desired_light_state = 0U;

}  // namespace aws
}  // namespace dcm
}  // namespace runnable