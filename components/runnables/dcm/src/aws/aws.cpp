#include "delegate/aws/aws.h"

namespace runnable {
namespace dcm {
namespace aws {
bool CAWSDelegate::shadowUpdateInProgress = false;
bool CAWSDelegate::ota_update_done = false;

}  // namespace aws
}  // namespace dcm
}  // namespace runnable