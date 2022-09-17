#include "Speaker.h"
#include "I2SOutput.h"
#include "WAVFileReader.h"

Speaker::Speaker(I2SOutput *i2s_output)
{
    m_i2s_output = i2s_output;
    m_ok = new WAVFileReader("/ok.wav");
    m_ready_ping = new WAVFileReader("/ready_ping.wav");
    m_cantdo = new WAVFileReader("/cantdo.wav");
    // m_life = new WAVFileReader("/life.wav");
    // numbers_arr[0] = new WAVFileReader("/zero.wav");
    numbers_arr[1] = new WAVFileReader("/one.wav");
    numbers_arr[2] = new WAVFileReader("/two.wav");
    numbers_arr[3] = new WAVFileReader("/three.wav");
    numbers_arr[4] = new WAVFileReader("/four.wav");
    numbers_arr[5] = new WAVFileReader("/five.wav");
    numbers_arr[6] = new WAVFileReader("/six.wav");
    numbers_arr[7] = new WAVFileReader("/seven.wav");
    numbers_arr[8] = new WAVFileReader("/eight.wav");
    // numbers_arr[9] = new WAVFileReader("/nine.wav");
    // numbers_arr[10] = new WAVFileReader("/ten.wav");
    // numbers_arr[2] = new WAVFileReader("/ready_ping.wav");
    // numbers_arr[3] = new WAVFileReader("/ready_ping.wav");
}

Speaker::~Speaker()
{
    delete m_ok;
    delete m_ready_ping;
    delete m_cantdo;
    delete m_life;
    delete numbers_arr[0];
    delete numbers_arr[1];
    delete numbers_arr[2];
    delete numbers_arr[3];
    delete numbers_arr[4];
    delete numbers_arr[5];
    delete numbers_arr[6];
    delete numbers_arr[7];
    delete numbers_arr[8];
    delete numbers_arr[9];
    delete numbers_arr[10];
}

void Speaker::playOK()
{
    m_ok->reset();
    m_i2s_output->setSampleGenerator(m_ok);
}

void Speaker::playNumber()
{
    m_ok->reset();
    m_i2s_output->setSampleGenerator(m_ok);
}

void Speaker::playReady()
{
    m_ready_ping->reset();
    m_i2s_output->setSampleGenerator(m_ready_ping);
}

void Speaker::playCantDo()
{
    m_cantdo->reset();
    m_i2s_output->setSampleGenerator(m_cantdo);
}

void Speaker::playNumber(int ind)
{
    numbers_arr[ind]->reset();
    m_i2s_output->setSampleGenerator(numbers_arr[ind]);
}

void Speaker::playLife()
{
    m_life->reset();
    m_i2s_output->setSampleGenerator(m_life);
}