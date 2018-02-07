extern "C" {
  #include <roboticscape.h>
  #include <rc_usefulincludes.h>
}

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <iostream>

#include "vex-encoders/VexEncoders.h"

void VexEncoders::init(int encoderCount) {
    count = encoderCount;
  	rc_i2c_init(1, 0x30);
    std::cout << "initialize bus1 to addressOx30" << std::endl;

  	for (int i = 0; i < count; ++i) {
    	int newAddress = ENCODER_START + i;  // TODO: We'll change this later to soft-coded addresses for each motor
    	rc_i2c_write_byte(1, 0x4D, newAddress<<1);
      std::cout << "encoder "<< i<< " got new address" << std::endl;
    	rc_i2c_set_device_address(1, newAddress);
      std::cout <<  "encoder "<< i<< " set new address" << std::endl;
    	if(i+1 < count) {
      		rc_i2c_send_byte(1,0x4B);
          std::cout << "encoder "<< i<< " disabled terminator" << std::endl;
    	}else{
        rc_i2c_send_byte(1,0x4C);
        std::cout << "encoder "<< i<< " enabled terminator" << std::endl;
      }
    	rc_i2c_set_device_address(1, 0x30);
      std::cout << "reset bus1 to addressOx30" << std::endl;
  	}

}



// Get rid of comment tag when ready to compile Encoder portion:


// -------------------------------------
// Vex Encoder Methods
////////////////////////////////////

// TODO: Change these functions' name to something that's not as confusing...
// count specifies which motor in I2C daisy-chain
// data[0] stores position, data[1] stores velocity
void VexEncoders::getEncoderInfo(int id, double* data) {
  	rc_i2c_set_device_address(1, ENCODER_START+id);
    uint8_t bytes[8];


    	//0x40 Rotation tics bits 15-08 (Msb)
		//0x41 Rotation tics bits 07-00 (Lsb)
        //0x42 Rotation tics bits 31-24 (Msb)
        //0x43 Rotation tics bits 23-16 (Lsb)
        //0x44 Velocity bits 15-08 (Msb)
        //0x45 Velocity bits 07-00 (Lsb)
        //0x46 Rotation tics bits 47-40 (Msb)
        //0x47 Rotation tics bits 39-32 (Lsb)



  	for (int i = 0; i < 8; i++) {
    	rc_i2c_read_byte(1, 0x40+i, bytes+i);
  	}
  	uint16_t speed = 0;
 	  int64_t position = 0;
  	position |= (bytes[0]) << 8;
  	position |= (bytes[1]);
  	position |= (bytes[2]) << 24;
  	position |= (bytes[3]) << 16;
  	position |= (bytes[6]) << 40;
  	position |= (bytes[7]) << 32;
  	speed |= (bytes[4]) << 8;
  	speed |= (bytes[5]);
  	data[0] = torque_rotations / ((double) ticks) * ((double) position);
  	data[1] = torque_rotations / (((double) speed) * time_delta);
    std::cout << "encoder " << i << " position " << data[0] << " speed " << data[1] << std::endl;
}
