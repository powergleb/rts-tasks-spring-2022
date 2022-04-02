String getValuesCommand = "<< GET-VALUES <<";

// Отправка команды на получение значений при запуске
void setup()
{
    Serial.begin(9600);
    Serial.print(getValuesCommand);
}

// Вывод всех полученных значений
void loop()
{
    while(Serial.available()) {
        Serial.println(Serial.readString());
    }
}
