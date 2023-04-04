#include <EDF.h>

#include <7segmentsLED.h>
#include <microphoneController.h>

#include <fft.h>
#include <movingAvegange.h>

#define I2S_WS 15
#define I2S_SD 4
#define I2S_SCK 2
#define I2S_PORT I2S_NUM_0

// Define the length and sample rate of the audio buffer
#define BUFFER_LENGTH 1024
#define SAMPLE_RATE 16000

arduinoFFT FFT = arduinoFFT();

SevenSegmentDisplay display(0x70);
microphoneController microphone(I2S_WS, I2S_SD, I2S_SCK, I2S_PORT, BUFFER_LENGTH, SAMPLE_RATE);

// Allocate memory for the audio buffer
int16_t audio_buffer[BUFFER_LENGTH];
double fft_output[BUFFER_LENGTH / 2 + 1];
size_t bytes_read;
int freq;

void readMicrophone()
{
  esp_err_t result = microphone.microphone_read(audio_buffer, &bytes_read);
}

void calculeFFT()
{
  fft(audio_buffer, fft_output, &FFT);
}
void principal_frequency()
{
  freq = get_principal_frequency(fft_output);
}

void displayResult()
{
  display.display(freq);
}

void setup()
{
  microphone.setupI2S();
  display.initI2C();
  addTask(64000, 1000, 64000, 4, &readMicrophone);
  // FFT
  addTask(64000, 33000, 65000, 3, &calculeFFT);
  // MDM
  addTask(64000, 2000, 98000, 2, &principal_frequency);
  // DISPLAY
  addTask(64000, 2000, 100000, 1, &displayResult);

  start_EDF();
}

void loop()
{
  if (exec_index != -1)
  {
    execFunctionAndUpdateDeadlines();
    exec_index = -1;
  };
  // put your main code here, to run repeatedly:
}
