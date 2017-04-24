# Arduino-Speech-Recognition
Speech Recognition and Synthesis using Arduino.

The project synthesizes speech using the Arduino DUE digital-to-analog converter (DAC), and a BitVoicer Server to process the audio stream.

## How it works:
1.	Audio waves will be captured and amplified by the Sparkfun Electret Breakout board;
2.	The amplified signal will be digitalized and buffered in the Arduino using its analog-to-digital converter (ADC);
3.	The audio samples will be streamed to BitVoicer Server using the Arduino serial port;
4.	BitVoicer Server will process the audio stream and recognize the speech it contains;
5.	The recognized speech will be mapped to predefined commands that will be sent back to the Arduino. If one of the commands consists in synthesizing speech, BitVoicer Server will prepare the audio stream and send it to the Arduino;
6.	The Arduino will identify the commands and perform the appropriate action. If an audio stream is received, it will be queued into the BVSSpeaker class and played using the DUE DAC and DMA.
7.	The SparkFun Mono Audio Amp will amplify the DAC signal so it can drive an 8 Ohm speaker.
