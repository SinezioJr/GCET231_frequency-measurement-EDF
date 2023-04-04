#define FFT_OUTPUT_SIZE 513
#define MOVING_AVERAGE_SIZE 20

#define SAMPLE_RATE 16000
#define N_SAMPLES 1024

double get_principal_frequency(double *fft_output)
{
  // Initialize moving average buffer
  double moving_average_buffer[MOVING_AVERAGE_SIZE] = {0};

  // Calculate moving average of FFT output
  double moving_average = 0;
  for (int i = 0; i < MOVING_AVERAGE_SIZE; i++)
  {
    moving_average_buffer[i] = fft_output[i];
    moving_average += moving_average_buffer[i];
  }
  moving_average /= MOVING_AVERAGE_SIZE;

  // Find peak frequency bin
  double max_magnitude = 0;
  int max_bin = 0;
  for (int i = 0; i < FFT_OUTPUT_SIZE / 2; i++)
  {
    if (fft_output[i] > max_magnitude)
    {
      max_magnitude = fft_output[i];
      max_bin = i;
    }
  }

  // Check if peak frequency is within moving average range
  double frequency;
  if (max_bin >= MOVING_AVERAGE_SIZE / 2 && max_bin < FFT_OUTPUT_SIZE / 2 - MOVING_AVERAGE_SIZE / 2)
  {
    // Calculate frequency based on bin number and sample rate
    frequency = max_bin * (double)SAMPLE_RATE / N_SAMPLES;

    // Calculate moving average of peak frequency magnitude
    double peak_magnitude_ma = 0;
    for (int i = max_bin - MOVING_AVERAGE_SIZE / 2; i < max_bin + MOVING_AVERAGE_SIZE / 2; i++)
    {
      peak_magnitude_ma += fft_output[i];
    }
    peak_magnitude_ma /= MOVING_AVERAGE_SIZE;

    // Calculate threshold as 75% of moving average
    double threshold = 0.75 * moving_average;

    // Return principal frequency if peak magnitude is above threshold
    if (peak_magnitude_ma > threshold)
    {
      return frequency;
    }
  }

  // Return 0 if no principal frequency found
  return 0;
}
