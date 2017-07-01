#ifndef __MUSICENGINE_H__
#define __MUSICENGINE_H__
#include <Arduino.h>
#if defined(ARDUINO_ARCH_ESP8266)
#include <Ticker.h>
#else
// for ATmega we'll use redefinition of Timer2 (on 328/168) and portmanipulation
#endif

class MusicEngine {
public:
    /** Creates an instance of the MusicEngine
      * @param pin pin used to generate the note frequencies
    */
    MusicEngine(int pin);

    /** Support for Arduino tone() replacements, needed to use Timer2 on ATmega
    */
    void tone(unsigned int frequency, unsigned long length=0);
	  void noTone();
	  void waitTone(unsigned long length=0);


    /** Starts playing a new MML sequence. If one is already playing it is stopped and the new
     * sequences started.
      * @param mml string of MML commands to be played
     */
    void play(const char* mml);

    /** Stop a currently playing sequence */
    void stop();

    /** Query the engine to determine if a MML sequence is currently being played. */
    bool getIsPlaying() const
    {
        return _isPlaying;
    }

    /** Setup a callback function that will be executed when the music sequence ends. */
    void setCompletionCallback(void (*function)(void))
    {
        _completionCallback = function;
    }

#if !defined(ARDUINO_ARCH_ESP8266)
		// Timer2 is used for atmega
		void _toneTim2(uint8_t pin, unsigned int frequency = 0, unsigned long length = 0);
		void _noToneTim2(void);
		void _waitToneTim2(unsigned long length);
		void _executeCommandTim2(void);
#endif


private:
    void executeCommand();
    int getNumber(int min, int max);
    void skipWhiteSpace();
    char getChar();
    char peekChar();
    void rewind();

private:
    int _pinPwm;
    bool _isPlaying;
    const char* _mml;
    int _mmlIndex;
    int _octave;
    float _duration;
    float _durationRatio;
    float _pause;
    int _tempo;
    int _volume;
#if defined (ARDUINO_ARCH_ESP8266)
    Ticker     _scheduler;
#else
    //MsTimer2   _scheduler;
#endif

    void (*_completionCallback)(void);
    static void musicTickerCallback(MusicEngine*);

    static const float WHOLE_NOTE_DURATION;
    static const float QUARTER_NOTE_DURATION;
    static const float QUARTER_NOTES_PER_MINUTE;

    static const float DEFAULT_TIMING;
    static const float LEGATO_TIMING;
    static const float STACCATO_TIMING;

    static const int NOTE_REST;
    static const int NOTE_C;
    static const int NOTE_CS;
    static const int NOTE_D;
    static const int NOTE_DS;
    static const int NOTE_E;
    static const int NOTE_F;
    static const int NOTE_FS;
    static const int NOTE_G;
    static const int NOTE_GS;
    static const int NOTE_A;
    static const int NOTE_AS;
    static const int NOTE_B;

    static const float PERIOD_TABLE[];
};
#endif //__MUSICENGINE_H__
