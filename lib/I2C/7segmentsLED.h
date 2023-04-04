#include <Adafruit_LEDBackpack.h>

class SevenSegmentDisplay
{
public:
  SevenSegmentDisplay(uint8_t address)
  {
    _address = address;
    _matrix = Adafruit_7segment();
  };

  void initI2C()
  {
    _matrix.begin(_address);
  }

  void display(int n)
  {
    _matrix.println(n);
    _matrix.writeDisplay();
  }

private:
  Adafruit_7segment _matrix;
  uint8_t _address;
};