void manageActuator(DynamicJsonDocument json) {
    bool success = json["success"];
    if (success) {
        String heater = json["actuator"]["heater"];
        String exhaust_fan = json["actuator"]["exhaust_fan"];
        uint8_t heaterPinState = digitalRead(heaterSwitch);
        uint8_t fanPinState = digitalRead(fanSwitch);

        if (heater == "ON" && heaterPinState == HIGH) {
            digitalWrite(heaterSwitch, LOW);
        }

        if (heater == "OFF" && heaterPinState == LOW) {
            digitalWrite(heaterSwitch, HIGH);
        }

        if (exhaust_fan == "ON" && fanPinState == HIGH) {
            digitalWrite(fanSwitch, LOW);
        }

        if (exhaust_fan == "OFF" && fanPinState == LOW) {
            digitalWrite(fanSwitch, HIGH);
        }

    }
}
