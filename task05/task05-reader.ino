#include <MFRC522.h>
#include <SPI.h>

template<typename T> class Array 
{
public:
    T* content;
    long length;
    Array(T* content, long length): content(content), length(length) {}
};

template<typename TKey, typename TValue> class Node
{
public:
    TKey key;
    TValue value;
    Node* next;
    Node(TKey key, TValue value): key(key), value(value), next(NULL) {}
};

template<typename TKey, typename TValue> class Dictionary
{
private:
    Node<TKey, TValue>* first;
    long length = 0;
public:
    void Set(TKey key, TValue value);
    TValue Get(TKey key, TValue defaultValue);
    Array<TKey> AllKeys();
    void Clear();
};

/*
* Устанавливает значение по ключу.
* Если ключ уже устанавливался, его значение перезаписывается.
* В ином случае добавляется новый ключ с переданным значением.
*/
template<typename TKey, typename TValue> void Dictionary<TKey, TValue>::Set(TKey key, TValue value) {
    if(!this->first) {
        this->first = new Node<TKey, TValue>(key, value);
        this->length++;
        return;
    }

    Node<TKey, TValue>* prev;
    Node<TKey, TValue>* cur = this->first;
    while(cur) {
        if(cur->key == key) {
            cur->value = value;
            return;
        }
        prev = cur;
        cur = cur->next;
    }

    prev->next = new Node<TKey, TValue>(key, value);
    this->length++;
}

/*
* Получение значения по ключу.
* Если Словарь содержит искомый ключ, то возвращается его значение.
* В ином случае позвращается значение по умолчанию (второй аргумент).
*/
template<typename TKey, typename TValue> TValue Dictionary<TKey, TValue>::Get(TKey key, TValue defaultValue) {
    Node<TKey, TValue>* cur = this->first;
    while(cur) {
        if(cur->key == key) {
            return cur->value;
        }
        cur = cur->next;
    }

    return defaultValue;
}

/*
* Получение списка всех ключей, хранящихся в Словаре.
*/
template<typename TKey, typename TValue> Array<TKey> Dictionary<TKey, TValue>::AllKeys() {
    TKey* content = new TKey[this->length];
    int i = 0;
    Node<TKey, TValue>* cur = this->first;
    while(cur) {
        content[i] = cur->key;
        cur = cur->next;
        i++;
    }
    return Array<TKey>(content, this->length);
}

/*
* Удаление всех ключей, хранящихся в Словаре.
*/
template<typename TKey, typename TValue> void Dictionary<TKey, TValue>::Clear() {
    Node<TKey, TValue>* cur = this->first;
    while(cur) {
        Node<TKey, TValue>* next = cur->next;
        delete cur;
        cur = next;
    }
    this->first = NULL;
    this->length = 0;
}

Dictionary<String, long>* dict = new Dictionary<String, long>();

#define RST_PIN 9
#define SS_PIN 10

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup()
{
    Serial.begin(9600);
    while (!Serial);
    SPI.begin(); 
    mfrc522.PCD_Init();
    ShowReaderDetails(); 
    Serial.println("START RDR");
}

String getValuesCommand = "<< GET-VALUES <<";

/*
* Преобразование считанного UID в строку
* для последующего хранения в словаре
*/
String UidToStr() {
    typedef struct {
        byte size; // Number of bytes in the UID. 4, 7 or 10.
        byte uidByte[10];
        byte sak; // The SAK (Select acknowledge) byte returned from the PICC after successful selection.
    } Uid;
    Uid* temp = (Uid*)&mfrc522.uid;

    String result = String("({") + temp->size + F("}{");
    for(int i = 0; i < 9; i++) {
      result = result + temp->uidByte[i] + F("-");
    }
    result = result + temp->uidByte[9] + F("}{") + temp->sak + F("})");

    return result;
}

void loop()
{
    // Если в буфере есть данные
    if(Serial.available()) {
        // Считываем их
        String command = Serial.readString();
        // Если это команда получения значений
        if(command.equals(getValuesCommand)) {
            Serial.println();
            Serial.println(">> VALUES >>");

            // Выводим все сохраненные ключи со значениями
            Array<String> keys = dict->AllKeys();
            for(int i = 0; i < keys.length; i++) {
                String key = keys.content[i];
                Serial.print(key);
                Serial.print(" -> ");
                Serial.println(dict->Get(key, 0));
            }

            Serial.println(">> END >>");
            // Очищаем Словарь
            dict->Clear();
        }
        // Иначе ничего не делаем
    }

    // Look for new cards
    if(!mfrc522.PICC_IsNewCardPresent()) {
        return;
    }

    // Select one of the cards
    if(!mfrc522.PICC_ReadCardSerial()) {
        return;
    }

    // Сообщаем о считывании нового устройства
    String res = UidToStr();
    Serial.print("Detected: ");
    Serial.println(res);
    // Увеличиваем количество его считываний в словаре на 1
    long cnt =  dict->Get(res, 0) + 1;
    dict->Set(res, cnt);

    // Вставляем небольшую задержку, чтобы
    // не считывать одно устройство много раз
    delay(1000);
}

void ShowReaderDetails() {
    // Get the MFRC522 software version
    byte v = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
    Serial.print(F("MFRC522 Software Version: 0x"));
    Serial.print(v, HEX);
    if (v == 0x91) {
        Serial.print(F(" = v1.0"));
    } else if (v == 0x92) {
        Serial.print(F(" = v2.0"));
    } else {
        Serial.print(F(" (unknown)"));
    }
    Serial.println("");
    // When 0x00 or 0xFF is returned, communication probably failed
    if ((v == 0x00) || (v == 0xFF)) {
        Serial.println(F("WARNING: Communication failure, is the MFRC522 properly connected?"));
    }
}
