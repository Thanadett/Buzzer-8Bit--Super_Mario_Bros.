#include <Arduino.h>

const int Button = 32;
const int Buzzer = 21;

hw_timer_t *timer = NULL;
volatile int noteIndex = 0;
volatile int speedLevel = 2;
const int speedLevels[5] = {250, 500, 1000, 1500, 2000};

#define NOTE_E7 2637
#define NOTE_C7 2093
#define NOTE_G7 3136
#define NOTE_G6 1568
#define NOTE_E6 1319
#define NOTE_A6 1760
#define NOTE_B6 1976
#define NOTE_AS6 1865
#define NOTE_D7 2349
#define NOTE_F7 2794
#define NOTE_C6 1047

const int melody[] = {
    NOTE_E7, NOTE_E7, 0, NOTE_E7,
    0, NOTE_C7, NOTE_E7, 0,
    NOTE_G7, 0, 0, 0,
    NOTE_G6, 0, 0, 0,

    NOTE_C7, 0, 0, NOTE_G6,
    0, 0, NOTE_E6, 0,
    0, NOTE_A6, 0, NOTE_B6,
    0, NOTE_AS6, NOTE_A6, 0,};

const int noteDurations[] = {
    150, 150, 150, 150,
    150, 150, 150, 150,
    150, 150, 150, 150,
    150, 150, 150, 150,

    150, 150, 150, 150,
    150, 150, 150, 150,
    150, 150, 150, 150,
    150, 150, 150, 150,};

const int melodyLength = sizeof(melody) / sizeof(melody[0]);

void IRAM_ATTR onTimer()
{
  if (noteIndex < melodyLength)
  {
    ledcWriteTone(0, melody[noteIndex]);
    timerAlarmWrite(timer, noteDurations[noteIndex] * 1000, true);
    noteIndex++;
  }
  else
  {
    ledcWriteTone(0, 0);
    timerAlarmDisable(timer);
    noteIndex = 0;
  }
}

void IRAM_ATTR handleButton()
{
  speedLevel = (speedLevel + 1) % 5;
  timerAlarmWrite(timer, speedLevels[speedLevel] * 1000, true);
}

void setup()
{
  Serial.begin(115200);

  ledcAttachPin(Buzzer, 0);
  ledcSetup(0, 2000, 8);

  pinMode(Button, INPUT_PULLUP);
  attachInterrupt(Button, handleButton, FALLING);

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, speedLevels[speedLevel] * 1000, true);

  timerAlarmEnable(timer);
}

void loop()
{
}
