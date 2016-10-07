# ESP-MusicEngine
MusicEngine library ported from mBed to ESP8266<br>
MusicEngine class / Retro  Music Engine<br>
Original author: Chris Taylor (taylorza)<br>
see https://developer.mbed.org/users/taylorza/code/MusicEngine/<br>
Ported from mBed to ESP8266/Arduino by MMOLE (maxint-rd)

MusicEngine provides a means to play Music Macro Language sequences asynchronously.<br>Learn more about Music Macro Language (MML) on wikipedia:<br>
   http://en.wikipedia.org/wiki/Music_Macro_Language<br>
   For music see http://www.archeagemmllibrary.com/

================================

Note: the current version is not built as an Arduino library yet. Just add the files to your project.

Initialisation outside of Setup():<br>
  #define BUZ_PIN 14<br>
  #include "MusicEngine.h"<br>
  MusicEngine music(BUZ_PIN);<br>
  
Then to play music where you want:<br>
  music.play("T240 L16 O6 C D E F G");
  
Short syntax overview:<br>
  Tnnn - Set tempo. Examples: T120, T240<br>
  Lnn  - Set note length. Examples: L8, L16<br>
  Mx   - Set timing. Mn=default, Ml=legato, Ms=staccato<br>
  On   - Set octave. Examples: O6, O8<br>
  A-C  - Play whole note. Example: C<br>
  Nnn  - Play frequency 0-96. Example: N48<br>
  #    - Play sharp note. Example: C#<br>
  +    - Alternative for #<br>
  -    - Play flat note. Example: D-<br>
  R    - Rest. Example:  CDEC r CDEC<br>
  P    - Alternative for R. Example:  CDEC p CDEC<br>
  .    - Longer note. Example: CDEC.<br>
  &gt; - shift actave up.  Example: CDE&gt;CDE.<br>
  &lt; - shift actave down.  Example: CDE&lt;CDE.<br>
 Music notation is case-insensitive. Spaces can be used.
