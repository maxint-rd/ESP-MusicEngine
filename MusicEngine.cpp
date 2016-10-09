// MusicEngine class
// Retro  Music Engine
// Author: Chris Taylor (taylorza)
// see https://developer.mbed.org/users/taylorza/code/MusicEngine/
// Ported from mBed to ESP8266/Arduino by MMOLE (mm_rd@maxint.nl)
/** MusicEngine provides a means to play Music Macro Language sequences asynchronously.
 *  Learn more about Music Macro Language (MML) on wikipedia
 *  http://en.wikipedia.org/wiki/Music_Macro_Language
 *  For music see http://www.archeagemmllibrary.com/
*/


#include <ESP8266WiFi.h>
#include "MusicEngine.h"
#include <cctype>

const float MusicEngine::WHOLE_NOTE_DURATION      = 1.0f;
const float MusicEngine::QUARTER_NOTE_DURATION    = MusicEngine::WHOLE_NOTE_DURATION / 4.0f;
const float MusicEngine::QUARTER_NOTES_PER_MINUTE = 60.0f / MusicEngine::QUARTER_NOTE_DURATION;

const float MusicEngine::DEFAULT_TIMING           = 7.0f / 8.0f;
const float MusicEngine::LEGATO_TIMING            = 1.0f;
const float MusicEngine::STACCATO_TIMING          = 3.0f / 4.0f;

const int MusicEngine::NOTE_REST                  = 0;
const int MusicEngine::NOTE_C                     = 1;
const int MusicEngine::NOTE_CS                    = 2;
const int MusicEngine::NOTE_D                     = 3;
const int MusicEngine::NOTE_DS                    = 4;
const int MusicEngine::NOTE_E                     = 5;
const int MusicEngine::NOTE_F                     = 6;
const int MusicEngine::NOTE_FS                    = 7;
const int MusicEngine::NOTE_G                     = 8;
const int MusicEngine::NOTE_GS                    = 9;
const int MusicEngine::NOTE_A                     = 10;
const int MusicEngine::NOTE_AS                    = 11;
const int MusicEngine::NOTE_B                     = 12;

MusicEngine* __thisMusicEngine=NULL;

void musicTickerCallback()
{
  if(__thisMusicEngine)
    __thisMusicEngine->executeCommand();
}

const float MusicEngine::PERIOD_TABLE[] = 
{
    0,
  //1                2                3                4                5                6                7                8                9                10               11               12
  //C                C#               D                D#               E                F                F#               G                G#               A                A#               B
    1.0f / 16.35f,   1.0f / 17.32f,   1.0f / 18.35f,   1.0f / 19.45f,   1.0f / 20.60f,   1.0f / 21.83f,   1.0f / 23.12f,   1.0f / 24.50f,   1.0f / 25.96f,   1.0f / 27.50f,   1.0f / 29.14f,   1.0f / 30.87f,   // Octave 0
    1.0f / 32.70f,   1.0f / 34.65f,   1.0f / 36.71f,   1.0f / 38.89f,   1.0f / 41.20f,   1.0f / 43.65f,   1.0f / 46.25f,   1.0f / 49.00f,   1.0f / 51.91f,   1.0f / 55.00f,   1.0f / 58.27f,   1.0f / 61.74f,   // Octave 1
    1.0f / 65.41f,   1.0f / 69.30f,   1.0f / 73.42f,   1.0f / 77.78f,   1.0f / 82.41f,   1.0f / 87.31f,   1.0f / 92.50f,   1.0f / 98.00f,   1.0f / 103.83f,  1.0f / 110.00f,  1.0f / 116.54f,  1.0f / 123.47f,  // Octave 2
    1.0f / 130.81f,  1.0f / 138.59f,  1.0f / 146.83f,  1.0f / 155.56f,  1.0f / 164.81f,  1.0f / 174.62f,  1.0f / 185.00f,  1.0f / 196.00f,  1.0f / 207.65f,  1.0f / 220.00f,  1.0f / 233.08f,  1.0f / 246.94f,  // Octave 3
    1.0f / 261.63f,  1.0f / 277.18f,  1.0f / 293.67f,  1.0f / 311.13f,  1.0f / 329.63f,  1.0f / 349.23f,  1.0f / 370.00f,  1.0f / 392.00f,  1.0f / 415.31f,  1.0f / 440.00f,  1.0f / 466.17f,  1.0f / 493.89f,  // Octave 4
    1.0f / 523.25f,  1.0f / 554.37f,  1.0f / 587.33f,  1.0f / 622.26f,  1.0f / 659.26f,  1.0f / 698.46f,  1.0f / 739.99f,  1.0f / 783.99f,  1.0f / 830.61f,  1.0f / 880.00f,  1.0f / 932.33f,  1.0f / 987.77f,  // Octave 5
    1.0f / 1046.51f, 1.0f / 1108.74f, 1.0f / 1174.67f, 1.0f / 1244.51f, 1.0f / 1318.52f, 1.0f / 1396.92f, 1.0f / 1479.99f, 1.0f / 1567.99f, 1.0f / 1661.23f, 1.0f / 1760.01f, 1.0f / 1864.66f, 1.0f / 1975.54f, // Octave 6
    1.0f / 2093.02f, 1.0f / 2217.47f, 1.0f / 2349.33f, 1.0f / 2489.03f, 1.0f / 2637.03f, 1.0f / 2793.84f, 1.0f / 2959.97f, 1.0f / 3135.98f, 1.0f / 3322.45f, 1.0f / 3520.02f, 1.0f / 3729.33f, 1.0f / 3951.09f, // Octave 7
};

MusicEngine::MusicEngine(int pin) :
    _pinPwm(pin),
    _isPlaying(false)
{
  __thisMusicEngine=this;
  pinMode(_pinPwm, OUTPUT);
  //_pwm.period_ms(1);
  analogWriteFreq(1000);
  //_pwm.write(0.0);
  analogWrite(_pinPwm, 0);
  //Serial.println("MusicEngine constructor");
}

/*
MusicEngine::MusicEngine(PinName pin) :
    _pwm(pin),
    _isPlaying(false)
{
    _pwm.period_ms(1);
    _pwm.write(0.0);
}
*/


void MusicEngine::Setup(void)
{
  //Serial.println("MusicEngine Setup");
}

void MusicEngine::play(char *mml)
{
//    __disable_irq();
    _isPlaying = false;
    _mml = mml;
    _mmlIndex = 0;
    _octave = 4;    
    _duration = QUARTER_NOTE_DURATION;
    _durationRatio = DEFAULT_TIMING;
    _tempo = 120;              
    //_pwm.period(0); 
    analogWriteFreq(1000);  // Note: analogWriteFreq(0);  gives a spontaneous WDT reset 
    //_pwm = 0.5f;
    analogWrite(_pinPwm, 512);  // default range is 1024
    _pause = 0;
    _isPlaying = true;    
//    __enable_irq();
    MusicEngine::executeCommand();
}

void MusicEngine::stop()
{
//    __disable_irq();
    _isPlaying = false;
//    __enable_irq();
}

void MusicEngine::executeCommand()
{   
    //Serial.println("MusicEngine executeCommand 0");
//    _scheduler.detach();
    if (_pause < 0) _pause=0;
    if (_pause != 0)
    {
        //Serial.print("MusicEngine executeCommand Pause:");
        //Serial.print(_pause);
        //_pwm.period(0); 
        //_pwm = 0.0f;
        //Serial.println("MusicEngine executeCommand 1");
        analogWriteFreq(1000);  // Note: analogWriteFreq(0);  gives a spontaneous WDT reset 
        analogWrite(_pinPwm, 0);  // default range is 1024
//        _scheduler.attach(this, &MusicEngine::executeCommand, _pause);
         _scheduler.once(_pause, musicTickerCallback);
        _pause = 0;        
        //Serial.println("MusicEngine executeCommand 2");
    }
    else
    {
        int freqIndex = -1;    
        do
        {
            skipWhiteSpace();
            switch(getChar())
            {
                case 'a': freqIndex = NOTE_A; break;
                case 'b': freqIndex = NOTE_B; break;
                case 'c': freqIndex = NOTE_C; break;
                case 'd': freqIndex = NOTE_D; break;
                case 'e': freqIndex = NOTE_E; break;
                case 'f': freqIndex = NOTE_F; break;
                case 'g': freqIndex = NOTE_G; break;
                
                case 'p':
                case 'r': freqIndex = NOTE_REST; break;
                
                case 'l': if (isdigit(peekChar())) _duration = (float)WHOLE_NOTE_DURATION / getNumber(1, 64); break;
                case 'o': if (isdigit(peekChar())) _octave = getNumber(0, 7); break;
                case 't': if (isdigit(peekChar())) _tempo = getNumber(32, 255); break;
                case 'm':
                    switch(getChar())
                    {
                        case 'n': _durationRatio = DEFAULT_TIMING; break;
                        case 'l': _durationRatio = LEGATO_TIMING; break;
                        case 's': _durationRatio = STACCATO_TIMING; break;
                    }
                    break;
                    
                case 'n': if (isdigit(peekChar())) freqIndex = getNumber(0, 96); break;
                case '<': --_octave; if (_octave < 0) _octave = 0; break;
                case '>': ++_octave; if (_octave > 7) _octave = 7; break;
                
                case '\0':                     
                    _isPlaying = false;
                    break;
            }
            
            if (!_isPlaying)
            {
                //Serial.println("MusicEngine done playing");
                //_pwm.period_ms(1);
                //_pwm.write(0.0);
                analogWriteFreq(1000);
                analogWrite(_pinPwm, 0);  // default range is 1024
//TODO                _completionCallback.call();
                return;
            }
                
            float durationRatio = _durationRatio;
            float duration = _duration;
            
            if (freqIndex != -1)
            {
                switch(getChar())
                {
                    case '+':
                    case '#': ++freqIndex; break;
                    
                    case '-': --freqIndex; break;
                    case '.': 
                        durationRatio = 3.0f / 2.0f; 
                        while(peekChar() == '.')
                        {
                            durationRatio *= 3.0f / 2.0f;
                            getChar();
                        }
                        break;
                    default: 
                        rewind(); 
                        break;
                }
                
                if (isdigit(peekChar()))
                {
                    duration = WHOLE_NOTE_DURATION / getNumber(1, 64);
                }
                
                if (freqIndex != NOTE_REST)
                {
                    //_pwm.period(PERIOD_TABLE[freqIndex + (_octave * 12)]);    
                    //_pwm = 0.5;
                    float ftFreq=1.0/PERIOD_TABLE[freqIndex + (_octave * 12)];
                    int nFreq=(int)ftFreq;
                    //Serial.print("MusicEngine executeCommand freq:");
                    //Serial.println(ftFreq);
                    
                    //Serial.printf("MusicEngine executeCommand 5: n%d|o%d=%d-%d\n", freqIndex, _octave, nFreq, 1.0/PERIOD_TABLE[freqIndex + (_octave * 12)]);

                    analogWriteFreq(nFreq);
                    //Serial.println("MusicEngine executeCommand 6");
                    analogWrite(_pinPwm, 512);  // default range is 1024

                //Serial.printf("MusicEngine busy(%d):", nFreq);
                }
                //Serial.println("MusicEngine executeCommand 7");
                duration *= (QUARTER_NOTES_PER_MINUTE / _tempo);
//                _scheduler.attach(this, &MusicEngine::executeCommand, duration * durationRatio);
                _pause=duration * durationRatio;
                _scheduler.once(_pause, musicTickerCallback);
                _pause = duration * (1 - durationRatio);      // TODO: why 1-ratio? Can result in negative pause!
            }
        } while (freqIndex == -1);
    }  
}

int MusicEngine::getNumber(int min, int max)
{
    char ch;
    int  value = 0;
    while ((ch = getChar()) != 0)
    {
        if (!isdigit(ch))
        {
            rewind();
            break;
        }
        int digit = (int)ch - 48;
        value = (value * 10) + digit;
    }
    value = value < min ? min : value > max ? max : value;
    return value;
}

void MusicEngine::skipWhiteSpace()
{
    while (isspace(peekChar())) getChar();
}

char MusicEngine::getChar()
{    
    return tolower(_mml[_mmlIndex++]);    
}

char MusicEngine::peekChar()
{
    return tolower(_mml[_mmlIndex]);    
}

void MusicEngine::rewind()
{
    --_mmlIndex;
}



