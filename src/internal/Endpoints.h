#ifndef CONSENTIUM_ENDPOINTS_H
#define CONSENTIUM_ENDPOINTS_H

// Internal API endpoints. constexpr arrays have internal linkage, so this
// header is safe to include from multiple translation units.

namespace consentium {
namespace endpoints {

// Sensor data update and receive URLs
constexpr char kSend[] = "https://api.consentiumiot.com/v2/updateData?";
constexpr char kReceive[] = "https://api.consentiumiot.com/getData?";

// Board OTA essential URLs
// constexpr char kVersion[] = "https://api.consentiumiot.com/firmware/version?";
// constexpr char kFirmware[] = "https://api.consentiumiot.com/firmware/bin?";

// TODO: Switch to api server for OTA
// Board OTA essential URLs (for app server)
constexpr char kVersion[] = "https://app.consentiumiot.com/firmware/version?";
constexpr char kFirmware[] = "https://app.consentiumiot.com/firmware/bin?";

} // namespace endpoints
} // namespace consentium

#endif // CONSENTIUM_ENDPOINTS_H

