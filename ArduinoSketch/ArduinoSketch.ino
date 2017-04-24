#include <BVSP.h>
#include <BVSMic.h>
#include <BVSSpeaker.h>
#include <DAC.h>

#define BVSM_AUDIO_INPUT 7

// Defines the LED pins
#define RED_LED_PIN 6
#define YELLOW_LED_PIN 9
#define GREEN_LED_PIN 10

const unsigned long STATUS_REQUEST_TIMEOUT = 3000;
const unsigned long STATUS_REQUEST_INTERVAL = 4000;

const int MIC_BUFFER_SIZE = 64;

const int SPEAKER_BUFFER_SIZE = 128;
const int RECEIVE_BUFFER_SIZE = 2;
BVSP bvsp = BVSP();

BVSMic bvsm = BVSMic(); 
BVSSpeaker bvss = BVSSpeaker();

byte micBuffer[MIC_BUFFER_SIZE];
byte speakerBuffer[SPEAKER_BUFFER_SIZE];

byte receiveBuffer[RECEIVE_BUFFER_SIZE];

bool playLEDNotes = false;
unsigned int playStartTime = 0;

void setup() 
{
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);

  // LED PINS
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(YELLOW_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
  
  Serial.begin(115200); 
 
  bvsp.begin(Serial, STATUS_REQUEST_TIMEOUT, STATUS_REQUEST_INTERVAL);
    
  bvsp.frameReceived = BVSP_frameReceived;

  bvsp.modeChanged = BVSP_modeChanged; 
  
  bvsp.streamReceived = BVSP_streamReceived;
  
  bvsm.begin();

  bvss.begin(DAC);
}

void loop() 
{
  bvsp.keepAlive();
   
  bvsp.receive();

  if (bvsp.isSREAvailable()) 
  {
    if (!bvsm.isRecording)
    {
      bvsm.setAudioInput(BVSM_AUDIO_INPUT, DEFAULT); 
      bvsm.startRecording();
    }
    if (bvsm.available)
    {
      if (bvsp.inboundMode == FRAMED_MODE)
        bvsp.setInboundMode(STREAM_MODE); 
      int bytesRead = bvsm.read(micBuffer, MIC_BUFFER_SIZE);
      
      bvsp.sendStream(micBuffer, bytesRead);
    }
  }
  else
  {
    if (bvsm.isRecording)
      bvsm.stopRecording();
  }
  bvss.play();

  if (playLEDNotes)
    playNextLEDNote();
}

void BVSP_frameReceived(byte dataType, int payloadSize) 
{
  if (dataType == DATA_TYPE_BINARY)
  {
    if (bvsp.getReceivedBytes(receiveBuffer, RECEIVE_BUFFER_SIZE) == 
      RECEIVE_BUFFER_SIZE)
    {
      analogWrite(receiveBuffer[0], receiveBuffer[1]);
    }
  }
  else if (dataType == DATA_TYPE_BYTE)
  {   
    if (bvsp.getReceivedByte() == 255)
    {
      playLEDNotes = true;
      playStartTime = millis();
    }
  }
}
void BVSP_modeChanged() 
{ 
  if (bvsp.outboundMode == FRAMED_MODE)
    bvss.finishPlaying();
} 

void BVSP_streamReceived(int size) 
{ 
  int bytesRead = bvsp.getReceivedStream(speakerBuffer, 
    SPEAKER_BUFFER_SIZE); 
    
  bvss.enqueue(speakerBuffer, bytesRead);
}

void playNextLEDNote()
{
  unsigned long elapsed = millis() - playStartTime;

  allLEDsOff();
  if (elapsed >= 11500)
  {
    analogWrite(RED_LED_PIN, 0);
    playLEDNotes = false;
  }
  // NOTES
  else if (elapsed >= 9900)
    analogWrite(RED_LED_PIN, 255); // C note
  else if (elapsed >= 9370)
    analogWrite(RED_LED_PIN, 255); // C note
  else if (elapsed >= 8900)
    analogWrite(YELLOW_LED_PIN, 255); // D note
  else if (elapsed >= 8610)
    analogWrite(RED_LED_PIN, 255); // C note
  else if (elapsed >= 8230)
    analogWrite(YELLOW_LED_PIN, 255); // D note
  else if (elapsed >= 7970)
    analogWrite(YELLOW_LED_PIN, 255); // D note
  else if (elapsed >= 7470)
    analogWrite(RED_LED_PIN, 255); // C note
  else if (elapsed >= 6760)
    analogWrite(GREEN_LED_PIN, 255); // E note
  else if (elapsed >= 6350)
    analogWrite(RED_LED_PIN, 255); // C note
  else if (elapsed >= 5880)
    analogWrite(YELLOW_LED_PIN, 255); // D note
  else if (elapsed >= 5560)
    analogWrite(RED_LED_PIN, 255); // C note
  else if (elapsed >= 5180)
    analogWrite(YELLOW_LED_PIN, 255); // D note
  else if (elapsed >= 4890)
    analogWrite(YELLOW_LED_PIN, 255); // D note
  else if (elapsed >= 4420)
    analogWrite(RED_LED_PIN, 255); // C note
  else if (elapsed >= 3810)
    analogWrite(GREEN_LED_PIN, 255); // E note
  else if (elapsed >= 3420)
    analogWrite(RED_LED_PIN, 255); // C note
  else if (elapsed >= 2930)
    analogWrite(YELLOW_LED_PIN, 255); // D note
  else if (elapsed >= 2560)
    analogWrite(RED_LED_PIN, 255); // C note
  else if (elapsed >= 2200)
    analogWrite(YELLOW_LED_PIN, 255); // D note
  else if (elapsed >= 1930)
    analogWrite(YELLOW_LED_PIN, 255); // D note
  else if (elapsed >= 1470)
    analogWrite(RED_LED_PIN, 255); // C note
  else if (elapsed >= 1000)
    analogWrite(GREEN_LED_PIN, 255); // E note
}
void allLEDsOff()
{
  analogWrite(RED_LED_PIN, 0);
  analogWrite(YELLOW_LED_PIN, 0);
  analogWrite(GREEN_LED_PIN, 0);
}
