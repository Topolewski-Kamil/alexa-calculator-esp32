#ifndef _intent_processor_h_
#define _intent_processor_h_

#include <map>
#include "WitAiChunkedUploader.h"

class Speaker;

enum IntentResult
{
    FAILED,
    SUCCESS,
    SILENT_SUCCESS // success but don't play ok sound
};

class IntentProcessor
{
private:
    std::map<std::string, int> m_device_to_pin;
    IntentResult turnOnDevice(const Intent &intent);
    IntentResult calculate(const Intent &intent);
    IntentResult saveNumber(const Intent &intent);
    IntentResult loadNumber(const Intent &intent);
    IntentResult changeColour(const Intent &intent);
    IntentResult outputNumber(int ind);
    IntentResult okay();
    IntentResult life();

    Speaker *m_speaker;

public:
    IntentProcessor(Speaker *speaker);
    void addDevice(const std::string &name, int gpio_pin);
    void calculate(const std::string &name, int gpio_pin);
    void saveNumber(const std::string &name, int gpio_pin);
    void loadNumber(const std::string &name, int gpio_pin);
    IntentResult processIntent(const Intent &intent);
};

#endif
