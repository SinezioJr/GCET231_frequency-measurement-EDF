#include <driver/i2s.h>
// Define the length of the audio buffer
class microphoneController
{
private:
  /* data */
  int I2S_WS;
  int I2S_SD;
  int I2S_SCK;
  i2s_port_t I2S_PORT;
  int BUFFER_LENGTH;
  uint32_t SAMPLE_RATE;

public:
  microphoneController(int _I2S_WS,
                       int _I2S_SD,
                       int _I2S_SCK,
                       i2s_port_t _I2S_PORT,
                       int _BUFFER_LENGTH,
                       uint32_t _SAMPLE_RATE)
  {
    I2S_WS = _I2S_WS;
    I2S_SD = _I2S_SD;
    I2S_SCK = _I2S_SCK;
    I2S_PORT = _I2S_PORT;

    BUFFER_LENGTH = _BUFFER_LENGTH;
    SAMPLE_RATE = _SAMPLE_RATE;
  };

  // Set up I2S interface
  void setupI2S()
  {
    i2s_config_t i2s_config = {
        .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
        .intr_alloc_flags = 0,
        .dma_buf_count = 8,
        .dma_buf_len = BUFFER_LENGTH,
        .use_apll = false};
    i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_SCK,
        .ws_io_num = I2S_WS,
        .data_out_num = -1,
        .data_in_num = I2S_SD};
    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_PORT, &pin_config);
  };

  esp_err_t microphone_read(int16_t *audio_buffer, size_t *bytes_read)
  {
    // Read audio data from the I2S interface
    size_t bytes_read2;
    esp_err_t result = i2s_read(I2S_PORT, audio_buffer, 2 * BUFFER_LENGTH, &bytes_read2, portMAX_DELAY);
    *bytes_read = bytes_read2;

    return result;
  };
};