// Sensor data update and receive URLs
const char* send_url = "https://api.consentiumiot.com/v2/updateData?";
const char* receive_url = "https://api.consentiumiot.com/getData?";

// Board OTA essential URLs
const char* versionURL = "https://api.consentiumiot.com/firmware/version?";
const char* firmwareURL = "https://api.consentiumiot.com/firmware/bin?";

// Board shared variables
bool otaFlag = false;
char macAddr[18];
