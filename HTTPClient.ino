#include <WString.h>
#include <HardwareSerial.h>

struct HTTPParser {
    String raw_response, head, body;


    HTTPParser(String resp) {
        this->raw_response = resp;
        initialize();
    }

    void initialize() {
        int count = this->raw_response.indexOf("\r\n\r\n");

        head = raw_response.substring(0, (count + 2));
        body = raw_response;
        body.remove(0, (count + 4));
    }

    DynamicJsonDocument toJson() {
        DynamicJsonDocument doc(200);

        DeserializationError error = deserializeJson(doc, body);

        // Test if parsing succeeds.
        if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.c_str());
        }

        return doc;
    }

};


DynamicJsonDocument updateAndGetInstr(String parameters = "") {
    createTCPConnection();

    Serial.println(parameters);
    String resp = sendHTTPRequest("/web-app/public/api/store-metric", parameters);

    HTTPParser parser(resp);

    Serial.println("resp: ");
    Serial.println(parser.body);

    releaseTCP();

    return parser.toJson();

}


  
