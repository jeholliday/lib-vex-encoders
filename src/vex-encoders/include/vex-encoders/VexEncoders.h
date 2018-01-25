#ifndef VEX_ENCODERS_h
#define VEX_ENCODERS_h

#define torque_rotations 0.02551020408
#define time_delta 0.00000213333
#define ticks 8
#define ENCODER_START 0x22

class VexEncoders
{
private:
  int count;
public:
  void init(int encoderCount);
  void getEncoderInfo(int id, double* data);
};
#endif
