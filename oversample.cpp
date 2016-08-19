#include "oversample.h"

Oversample::Oversample(byte pin, byte resolution)
{
  _pin = pin;

  pinMode(_pin, INPUT);
  setPrescaler(_prescaler);
  setResolution(resolution);
}

double Oversample::read()
{
  unsigned long scaled = readScaled();
  double averaged = (scaled * 1.0) / (B00000001 << _additionalBits);
  return averaged;
}

unsigned long Oversample::readScaled()
{
  /* Sum all measurements. */
  unsigned long total = 0UL;
  for(int i = 0; i < _sampleCount; i++) {
    total += analogRead(_pin);
  }

  /* Scale by right shifting. */
  return total >> _additionalBits;
}

void Oversample::setResolution(byte resolution)
{
  _resolution = sanitizeResolution(resolution);
  _additionalBits = _resolution - _baseResolution;
  _sampleCount = B00000001 << (_additionalBits * 2);
}

byte Oversample::getResolution()
{
  return _resolution;
}

void Oversample::setPrescaler(byte prescaler)
{
  _prescaler = prescaler;
  byte mask = B11111000;
  ADCSRA &= mask;
  ADCSRA |= _prescaler;
}

byte Oversample::getPrescaler()
{
  return _prescaler;
}

byte Oversample::sanitizeResolution(byte resolution)
{
  if(resolution > 16) {
    resolution = 16;
  }

  else if(resolution < 10) {
    resolution = 10;
  }

  return resolution;
}