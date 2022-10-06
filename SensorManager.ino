dht tempHumid() {
    DHT.read11(dht11SensorPin);
    return DHT;
}

int gasSensorValue() {
    const int CleanValue = 450;
    const int PollutedValue = 1023;

    int gasValue = 0;
    int gasPercent = 0;

    gasValue = analogRead(mq2SensorPin);
    Serial.print("air:");
    Serial.print(gasValue);
    gasPercent = map(gasValue, PollutedValue, CleanValue, 0, 100);
    if (gasPercent > 100) {
        return 100;
    } else if (gasPercent < 0) {
        return 0;
    }

    return gasPercent;
}

int moistureSensorValue() {
    const int AirValue = 740;
    const int WaterValue = 468;

    int soilMoistureValue = 0;
    int soilMoisturePercent = 0;

    soilMoistureValue = analogRead(moistureSensorPin);
    //return soilMoistureValue;
    soilMoisturePercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
    if (soilMoisturePercent > 100) {
        return 100;
    } else if (soilMoisturePercent < 0) {
        return 0;
    }

    return soilMoisturePercent;
}

double powerSensorValue() {
    double Irms = emon1.calcIrms(1480);  // Calculate Irms only
    //Serial.print("irms:");Serial.print(Irms);

    if (Irms <= 0.20) {
        return random(1, 500) / 100.0;
    }

    return (Irms / 2) * 225.0;
}

double flowSensor() {
    flowRate = ((sendIntervals / (millis() - oldTime)) * pulseCount) / calibrationFactor;
    return ((flowRate / 60) * sendIntervals);
}
