# ESP-MusicEngine

### Credits/references
MusicEngine library ported from mBed to ESP8266 (tested in Arduino IDE v1.6.10)<br>
MusicEngine class / Retro  Music Engine<br>
Original author: Chris Taylor (taylorza)<br>
see https://developer.mbed.org/users/taylorza/code/MusicEngine/<br>
Ported from mBed to ESP8266/Arduino by MMOLE (maxint-rd)

MusicEngine provides a means to play Music Macro Language sequences asynchronously.<br>
Learn more about Music Macro Language (MML) on wikipedia:<br>
   http://en.wikipedia.org/wiki/Music_Macro_Language<br>
   For music see http://www.archeagemmllibrary.com/<br>
Extensive MML reference guide (not all commands supported):<br>
   http://woolyss.com/chipmusic/chipmusic-mml/ppmck_guide.php

================================
### Installation/Usage
The current version can be downloaded as an Arduino library using the Sketch|Library menu. Just add the zipfile library and the enclosed examples should appear in the menu automatically.

Initialisation outside of Setup():
```
  // define pin, include header and initialize class
  #define BUZ_PIN 14
  #include "MusicEngine.h"
  MusicEngine music(BUZ_PIN);
```

Then to play music, call the play method where you want:
```
music.play("T240 L16 O6 C D E F G");
```
Note: the music will keep on playing using a Ticker interrupt. The callback for completion has not been ported yet

### Supported MML Syntax
Short syntax overview:<br>
&nbsp;  Tnnn - Set tempo. Examples: T120, T240<br>
&nbsp;  Lnn  - Set default note length. Examples: L8, L16<br>
&nbsp;  Mx   - Set timing. Mn=default, Ml=legato, Ms=staccato<br>
&nbsp;  On   - Set octave. Examples: O6, O8<br>
&nbsp;  A-G  - Play whole note. Example: C<br>
&nbsp;  Ann-Gnn  - Play note of alternative length. Example: C4, A16<br>
&nbsp;  Nnn  - Play frequency 0-96. Example: N48<br>
&nbsp;  #    - Play sharp note. Example: C#<br>
&nbsp;  &plus;    - Alternative for #<br>
&nbsp;  &minus;    - Play flat note. Example: D-&nbsp; <br>
&nbsp;  R    - Rest. Example:  CDEC r CDEC<br>
&nbsp;  P    - Alternative for R. Example:  CDEC p CDEC<br>
&nbsp;  .    - Longer note. Example: CDEC.&nbsp;<br>
&nbsp;  &gt; - shift octave up.  Example: CDE&gt;CDE.&nbsp;<br>
&nbsp;  &lt; - shift octave down.  Example: CDE&lt;CDE.&nbsp;<br>
The supported MML-commands are a subset that may not completely cover all available music scores.
If notes seem missing, check your score against the syntax above and replace unknown commands by equivalent supported alternatives. The music notation is case-insensitive. Spaces are not required but can be used for readability.
