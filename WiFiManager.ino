

//struct HTTPHeaderKeyValuePair {
//    String Key, Value;
//
//    HTTPHeaderKeyValuePair(String key, String value = "") {
//        Key = key;
//        Value = value;
//    }
//
//    String toString() {
//        if (Value == "")
//            return Key;
//        return Key + ": " + Value + "\r\n";
//    }
//
//
//};
//
//struct HTTPRequest {
//    String headers = "";
//
//    HTTPRequest(HTTPHeaderKeyValuePair headers[]) {
//        int max = sizeof(headers);
//
//        for (int i = 0; i < max; i++) {
//            this->headers += headers[i].toString();
//        }
//
//    }
//
//    String toString() {
//        return headers;
//    }
//};

String sendHTTPRequest(String uri, String query_parameters = "", String method = "GET") {

    uint8_t buffer[1024] = {0};

    String request = method + " " + uri + "?api_token=" + api_token + "&" + query_parameters + " HTTP/1.1\r\n";
    request += "User-Agent: Mugonix IoT/1.0 (compatible; Safari; MacOS)\r\n";
    request += "Host: ";
    request += HOST_NAME;
    request += "\r\n";
    request += "Accept-Language: en-us\r\n";
    request += "Accept-Encoding: gzip, deflate\r\n";
    request += "Connection: Keep-Alive\r\n\r\n";

    const char *req_c = request.c_str();
    wifi.send((const uint8_t *) req_c, strlen(req_c));

    String resp = "";

    uint32_t len = wifi.recv(buffer, sizeof(buffer), 10000);
    if (len > 0) {
        for (uint32_t i = 0; i < len; i++) {
            resp += (char) buffer[i];
        }
    }

    return resp;
}

void connectToWiFi() {
    if (wifi.joinAP(SSID, PASSWORD)) {
        Serial.print("Join AP success\r\n");
    } else {
        Serial.print("Join AP failure\r\n");
    }

    if (wifi.disableMUX()) {
        Serial.print("single ok\r\n");
    } else {
        Serial.print("single err\r\n");
    }
}

void createTCPConnection() {
    if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
        Serial.print("create tcp ok\r\n");
    } else {
        Serial.print("create tcp err\r\n");
    }

}

void releaseTCP() {
    if (wifi.releaseTCP()) {
        Serial.print("release tcp ok\r\n");
    } else {
        Serial.print("release tcp err\r\n");
    }
}
