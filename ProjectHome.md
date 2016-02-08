# Introduction #
A fancy alarm clock built with the Arduino platform and hardware.

It keeps track of time and can measure temperature, humidity,
dew point, light level, air quality, and sleep patterns.
It's designed as a wooden box with touch sensing for control.

It monitors the quality of air, and general comfort in the room
during sleep and wakes you up at the best possible time by sensing
your sleep state with an accelerometer sensor under the bed.
All data is logged wirelessly to a base station connected to the computer
for latter analysis.

Right now I am providing all [source code](http://code.google.com/p/lolosalarmclock/source/browse/#svn/trunk/AlarmClock) (Arduino) and videos for the project.
Details on the electronics will be provided at some later time.

# Theory #

The more you move during your sleep the easier it is to wake you up.
'Deep sleep' is the worst time to wake up and is characterized by very few
body movements. Any other time is better: in particular just after moving substantially.
For example turning from belly to back side, moving arms, legs, head... This sometimes corresponds (at least for me) to a short moment of consciousness while dreaming or close to waking up.
The alarm clock monitors a time window defined by a wake-up deadline and a preceding monitoring period (configurable). For example you can tell it to wake you up no later than 8h00 but up to one hour before (between 7h00 and 8h00). It will wake you up just after a body movement that exceeded a given intensity threshold.

Below is an example 'Sleepogram':


<img src='http://lolosalarmclock.googlecode.com/svn/trunk/SleepCycle.png' width='640'>


Also interesting is the possibility to monitor the general comfort of the room while sleeping. I usually get fued in the morning if there was not enough air circulation.<br>
The alarm clock can warn me if there is a problem with the air quality. Air contaminants detectable by the Figaro gas sensor (TGS2602) accumulate and are a good indicator of overall air freshness...<br>
<br>
The alarm clock makes 'bird-like' sounds although right now the birds sound very wrong...<br>
A nice idea would be to have bird-like sound that slightly vary randomly thus avoiding the usual repetitive and boring alarm clock buzz.<br>
<br>
<h1>Hardware</h1>

What I have used:<br>
<ul><li><a href='http://arduino.cc/en/Main/ArduinoBoardDuemilanove'>Arduino Dueminalove (alarm-clock)</a>
</li><li><a href='http://www.seeedstudio.com/depot/seeeduino-mega-fully-assembled-p-438.html'>Seeduino Mega (Base-station)</a>
</li><li><a href='http://www.sparkfun.com/commerce/product_info.php?products_id=99'>Real-time clock</a>
</li><li><a href='http://www.sparkfun.com/commerce/product_info.php?products_id=9395'>LCD screen</a>
</li><li><a href='http://www.sparkfun.com/commerce/product_info.php?products_id=8257'>Temperature and humidity sensor</a>
</li><li><a href='http://www.figarosensor.com/products/2602pdf.pdf'>Gas sensor: detection of air contaminants (TGS2602 or TGS2600)</a></li></ul>

<h1>Videos</h1>

Presentation of the project:<br>
<br>
<a href='http://www.youtube.com/watch?feature=player_embedded&v=CphsgESZGcE' target='_blank'><img src='http://img.youtube.com/vi/CphsgESZGcE/0.jpg' width='425' height=344 /></a><br>
<br>
Setting time and alarm:<br>
<br>
<a href='http://www.youtube.com/watch?feature=player_embedded&v=H51m5tOAKqQ' target='_blank'><img src='http://img.youtube.com/vi/H51m5tOAKqQ/0.jpg' width='425' height=344 /></a><br>
<br>
Sensors:<br>
<br>
<a href='http://www.youtube.com/watch?feature=player_embedded&v=9OHXxB8jQfU' target='_blank'><img src='http://img.youtube.com/vi/9OHXxB8jQfU/0.jpg' width='425' height=344 /></a><br>
<br>
Wireless monitoring:<br>
<br>
<a href='http://www.youtube.com/watch?feature=player_embedded&v=pKqmedNijh8' target='_blank'><img src='http://img.youtube.com/vi/pKqmedNijh8/0.jpg' width='425' height=344 /></a><br>
<br>
<br>
<br>
<br>
<h1>Author</h1>

Loic Royer is a French engineer and scientist working on his PhD in the field of Computational Biology. This project was conducted as a hobby.<br>
<br>
Contact: <code>LolosAlarmClock by googlemail.com</code>