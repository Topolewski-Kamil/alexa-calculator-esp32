#include <Arduino.h>
#include "IntentProcessor.h"
#include "Speaker.h"
#include <dotstar_wing.h>
#include <EEPROM.h>

IntentProcessor::IntentProcessor(Speaker *speaker)
{
    m_speaker = speaker;
}

IntentResult IntentProcessor::turnOnDevice(const Intent &intent)
{

    return SUCCESS;
}

IntentResult IntentProcessor::saveNumber(const Intent &intent)
{
    Serial.printf(
      "saving: confidence=%.f%%; number1=%d; index=%d; num1_confidence=%.f%%; num2_confidence=%.f%%\n",
      100 * intent.intent_confidence, intent.number1, intent.number2, 100 * intent.num1_confidence, 100 * intent.num2_confidence
    );

    if (intent.intent_confidence < 0.4)
    {
        Serial.printf("Only %.f%% certain on intent\n", 100 * intent.intent_confidence);
        return FAILED;
    }

    if (intent.num1_confidence < 0.4 || intent.num2_confidence < 0.4 )
    {
        Serial.printf("Only %.f%% certain on number\n", 100 * intent.num1_confidence);
        return FAILED;
    }

    // loads the content of 10 bytes of flash into a 10 bytes long array in RAM
    EEPROM.begin(10);

    // write number1 on address number2 in flash
    EEPROM.write(intent.number2, intent.number1);

    // save changes
    EEPROM.commit();

    Serial.printf("\nStored %d at index %d \n", intent.number1, intent.number2);

    return SUCCESS;
}

IntentResult IntentProcessor::loadNumber(const Intent &intent)
{
    Serial.printf(
      "saving: confidence=%.f%%; number1=%d; index=%d; num1_confidence=%.f%%\n",
      100 * intent.intent_confidence, intent.number1, intent.number2, 100 * intent.num1_confidence
    );

    if (intent.intent_confidence < 0.4)
    {
        Serial.printf("Only %.f%% certain on intent\n", 100 * intent.intent_confidence);
        return FAILED;
    }

    if (intent.num1_confidence < 0.4)
    {
        Serial.printf("Only %.f%% certain on number\n", 100 * intent.num1_confidence);
        return FAILED;
    }

    // loads the content of 10 bytes of flash into a 10 bytes long array in RAM
    EEPROM.begin(10);

    // read result at index
    int result = EEPROM.read(intent.number1);


    Serial.printf("\nFound %d at index %d \n", result, intent.number1);
    outputNumber(result);

    return SILENT_SUCCESS;
}

IntentResult IntentProcessor::calculate(const Intent &intent)
{
    Serial.printf(
      "addition: confidence=%.f%%; number1=%d; number2=%d; num1_confidence=%.f%%; num2_confidence=%.f%%\n",
      100 * intent.intent_confidence, intent.number1, intent.number2, 100 * intent.num1_confidence, 100 * intent.num2_confidence
    );

    if (intent.intent_confidence < 0.4)
    {
        Serial.printf("Only %.f%% certain on intent\n", 100 * intent.intent_confidence);
        return FAILED;
    }

    if (intent.num1_confidence < 0.4 || intent.num2_confidence < 0.4 )
    {
        Serial.printf("Only %.f%% certain on number\n", 100 * intent.num1_confidence);
        return FAILED;
    }

    int result = 0;

    if (intent.intent_name == "addition"){
        result = intent.number1 + intent.number2;
    } else if (intent.intent_name == "multiply"){
        result = intent.number1 * intent.number2;
    } else if (intent.intent_name == "division"){
        result = intent.number1 / intent.number2;
    } else if (intent.intent_name == "minus"){
        result = intent.number2 - intent.number1;
    }

    Serial.printf("\nResult %d", result);
    outputNumber(result);

    return SILENT_SUCCESS;
}

IntentResult IntentProcessor::okay()
{
    m_speaker->playOK();
    return SILENT_SUCCESS;
}

IntentResult IntentProcessor::outputNumber(int ind)
{
    m_speaker->playNumber(ind);
    return SILENT_SUCCESS;
}

IntentResult IntentProcessor::life()
{
    m_speaker->playLife();
    return SILENT_SUCCESS;
}

IntentResult IntentProcessor::processIntent(const Intent &intent)
{
    Serial.printf(
      "processIntent: name=%s; confidence=%.f%%; trait_value=%s; trait_confidence=%.f%%\n",
      intent.intent_name.c_str(), 100 * intent.intent_confidence,
      intent.trait_value.c_str(), 100 * intent.trait_confidence
    );

    if (intent.text.empty())
    {
        Serial.println("No text recognised");
        return FAILED;
    }
    Serial.printf("I heard \"%s\"\n", intent.text.c_str());
    if (intent.intent_name.empty())
    {
        Serial.println("Can't work out what you want to do with the device...");
        return FAILED;
    }
    Serial.printf("Intent is %s\n", intent.intent_name.c_str());

    
    if (intent.intent_name == "save")
    {
        return saveNumber(intent);
    }

    if (intent.intent_name == "load")
    {
        return loadNumber(intent);
    }

    if (!intent.intent_name.empty())
    {
        return calculate(intent);
    }

    return FAILED;
}