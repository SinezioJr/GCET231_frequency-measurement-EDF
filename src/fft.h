#include <arduinoFFT.h>

#define SAMPLE_RATE 16000
#define BUFFER_LENGTH 1024

double vReal[BUFFER_LENGTH];
double vImag[BUFFER_LENGTH];

void fft(int16_t *audio_buffer, double *fft_output, arduinoFFT *FFT)
{
  // Prepare samples
  for (int i = 0; i < BUFFER_LENGTH; i++)
  {
    vReal[i] = (double)audio_buffer[i];
    vImag[i] = 0.0;
  }

  // Perform FFT
  (*FFT).Windowing(vReal, BUFFER_LENGTH, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  (*FFT).Compute(vReal, vImag, BUFFER_LENGTH, FFT_FORWARD);
  (*FFT).ComplexToMagnitude(vReal, vImag, BUFFER_LENGTH);

  memcpy(fft_output, vReal, BUFFER_LENGTH / 2 + 1);
}
