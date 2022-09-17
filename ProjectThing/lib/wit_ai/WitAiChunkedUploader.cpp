#include "WitAiChunkedUploader.h"
#include "WiFiClientSecure.h"
#include <ArduinoJson.h>
using namespace std;
#include <iostream>

WitAiChunkedUploader::WitAiChunkedUploader(const char *access_key)
{
    m_wifi_client = new WiFiClientSecure();
    m_wifi_client->setInsecure();
    m_wifi_client->connect("api.wit.ai", 443);
    char authorization_header[100];
    snprintf(authorization_header, 100, "authorization: Bearer %s", access_key);
    m_wifi_client->println("POST /speech?v=20200927 HTTP/1.1");
    m_wifi_client->println("host: api.wit.ai");
    m_wifi_client->println(authorization_header);
    m_wifi_client->println("content-type: audio/raw; encoding=signed-integer; bits=16; rate=16000; endian=little");
    m_wifi_client->println("transfer-encoding: chunked");
    m_wifi_client->println();
}

bool WitAiChunkedUploader::connected()
{
    return m_wifi_client->connected();
}

void WitAiChunkedUploader::startChunk(int size_in_bytes)
{
    m_wifi_client->printf("%X\r\n", size_in_bytes);
}

void WitAiChunkedUploader::sendChunkData(const uint8_t *data, int size_in_bytes)
{
    m_wifi_client->write(data, size_in_bytes);
}

void WitAiChunkedUploader::finishChunk()
{
    m_wifi_client->print("\r\n");
}

Intent WitAiChunkedUploader::getResults()
{
    // finish the chunked request by sending a zero length chunk
    m_wifi_client->print("0\r\n");
    m_wifi_client->print("\r\n");
    // get the headers and the content length
    int status = -1;
    int content_length = 0;
    while (m_wifi_client->connected())
    {
        char buffer[255];
        int read = m_wifi_client->readBytesUntil('\n', buffer, 255);
        if (read > 0)
        {
            buffer[read] = '\0';
            // blank line indicates the end of the headers
            if (buffer[0] == '\r')
            {
                break;
            }
            if (strncmp("HTTP", buffer, 4) == 0)
            {
                sscanf(buffer, "HTTP/1.1 %d", &status);
            }
            else if (strncmp("Content-Length:", buffer, 15) == 0)
            {
                sscanf(buffer, "Content-Length: %d", &content_length);
            }
        }
    }
    Serial.printf("Http status is %d with content length of %d\n", status, content_length);
    if (status == 200)
    {
        StaticJsonDocument<2500> filter;
        filter["entities"]["wit$number:number"][0]["value"] = true;
        filter["entities"]["wit$number:number"][1]["value"] = true;
        filter["entities"]["wit$number:number"][0]["confidence"] = true;
        filter["entities"]["wit$number:number"][1]["confidence"] = true;
        filter["text"] = true;
        filter["intents"][0]["name"] = true;
        filter["intents"][0]["confidence"] = true;

        StaticJsonDocument<2500> doc;
        deserializeJson(doc, *m_wifi_client, DeserializationOption::Filter(filter));
        std::cout<<doc<<endl;
        
        const char *text = doc["text"];
        const char *intent_name = doc["intents"][0]["name"];
        float intent_confidence = doc["intents"][0]["confidence"];
        int number1 = doc["entities"]["wit$number:number"][0]["value"];
        int number2 = doc["entities"]["wit$number:number"][1]["value"];
        float num1_confidence = doc["entities"]["wit$number:number"][0]["confidence"];
        float num2_confidence = doc["entities"]["wit$number:number"][1]["confidence"];

        return Intent{
            .text = (text ? text : ""),
            .intent_name = (intent_name ? intent_name : ""),
            .intent_confidence = intent_confidence,
            .number1 = (number1),
            .number2 = (number2),
            .num1_confidence = num1_confidence,
            .num2_confidence = num2_confidence
            };
    }
    return Intent{};
}

WitAiChunkedUploader::~WitAiChunkedUploader()
{
    delete m_wifi_client;
}
