#include <iostream>
#include <math.h>
#include <stdio.h>
#include <libusb-1.0/libusb.h>

using namespace std;

#define NO_ERROR			0
#define ERROR_INIT_USB 			1
#define ERROR_GETTING_HANDLE_USB 	2
#define ERROR_CLAIMING_INTERFACE	3

#define ERROR_INVALID_MOTOR_ID		1
#define ERROR_INVALID_DIRECTION		2
#define ERROR_USB_WRITING_ERROR		3
#define ERROR_USB_READING_ERROR		4

#define MOTOR_ONE_IDENTIFIER		2
#define MOTOR_TWO_IDENTIFIER		4


static libusb_device_handle *dev_handle; //a device handle
static libusb_context *ctx = NULL; //a libusb session

int close_controller(void){
	libusb_close(dev_handle); //close the device we opened
	libusb_exit(ctx); //needs to be called to end the
	return NO_ERROR;
}

int init_controller(void) {
	
	int r; //for return values

	r = libusb_init(&ctx); 
	if(r < 0) {
		return ERROR_INIT_USB;
	}
	libusb_set_debug(ctx, 3); 

	// initialivze stepper bee
	dev_handle = libusb_open_device_with_vid_pid(ctx, 0x4d8, 0x5d); 
	if(dev_handle == NULL){
		return ERROR_GETTING_HANDLE_USB;
	}	

	if(libusb_kernel_driver_active(dev_handle, 0) == 1) { 
	   libusb_detach_kernel_driver(dev_handle, 0); 
	}

	r = libusb_claim_interface(dev_handle, 0); 
	if(r < 0) {
		return ERROR_CLAIMING_INTERFACE;
	}
	
	return NO_ERROR;
}

int set_modes       (int motor_one_state,
		     int motor_two_state,
		     int motor_one_mode,
		     int motor_two_mode) {
  
unsigned char *data = new unsigned char[2]; //data to write
data[0] = 16;
data[1] = 0;

int motor_one_inverse ;
int motor_two_inverse ;
int r, actual;

if (motor_one_state == 0) motor_one_inverse = 1;
if (motor_one_state == 1) motor_one_inverse = 0;
if (motor_two_state == 0) motor_two_inverse = 1;
if (motor_two_state == 1) motor_two_inverse = 0;
	
data[1] |= (motor_one_mode << 0);
data[1] |= (motor_two_mode << 1);
data[1] |= (motor_one_inverse << 2);
data[1] |= (motor_two_inverse << 3);

r = libusb_bulk_transfer(dev_handle, 0x02, data, 2, &actual, 0); 

if(r != 0 || actual != 2){
  delete[] data; 
  return ERROR_USB_WRITING_ERROR;
}

delete[] data; //delete the allocated memory for data

return NO_ERROR;
}

	
int get_motor_status(int *motor_one_status, 
		     int *motor_two_status, 
		     int *motor_one_steps, 
		     int *motor_two_steps){
    
   
    int r, actual, bit_value;
    unsigned char *data = new unsigned char[8]; //data to write
    unsigned char *data_in = new unsigned char[1]; //data to write
    data_in[0] = 8;
    
    r = libusb_bulk_transfer(dev_handle, 0x02, data_in, 1, &actual, 0); 
	if(r != 0 || actual != 1) {
		delete[] data_in;
		return ERROR_USB_WRITING_ERROR;
	}
    delete[] data_in;
    
    r = libusb_bulk_transfer(dev_handle, 0x81 , data, 8, &actual, 0); 
	if(r != 0 || actual != 8) {
	 delete[] data; 
	 return ERROR_USB_READING_ERROR; 
	}
	
    int i;
    
    *motor_one_status = 0;
    *motor_two_status = 0;
    
    for(i=7; 0<= i; i--){
      bit_value = data[0] >> i & 0x01;
 
      if (i==0 && bit_value == 1){
	*motor_one_status = 1;
      } 
      
      if (i==2 && bit_value == 1){
	*motor_two_status = 1;
      } 
      
    }
    
    *motor_one_steps = (data[1]+(data[2]-1)*128);
    *motor_two_steps = (data[3]+(data[4]-1)*128);
	   
    delete[] data; 
    return NO_ERROR;
    
}

int get_digital_input_status(int *input_1, 
			     int *input_2,
			     int *input_3, 
			     int *input_4,
			     int *input_5){
    int r, actual, bit_value;
    unsigned char *data = new unsigned char[8]; //data to write
    unsigned char *data_in = new unsigned char[1]; //data to write
    data_in[0] = 8;
    
    r = libusb_bulk_transfer(dev_handle, 0x02, data_in, 1, &actual, 0); 
	if(r != 0 || actual != 1) {
		delete[] data_in;
		return ERROR_USB_WRITING_ERROR;
	}
    delete[] data_in;
    
    r = libusb_bulk_transfer(dev_handle, 0x81 , data, 8, &actual, 0); 
	if(r != 0 || actual != 8) {
	 delete[] data; 
	 return ERROR_USB_READING_ERROR;; 
	}
    int i;
    
    *input_1 = 0;
    *input_2 = 0;
    *input_3 = 0;
    *input_4 = 0;
    *input_5 = 0;
    
    for(i=7; 0<= i; i--){
      bit_value = data[5] >> i & 0x01;
//       
      if (i==7 && bit_value == 1){
	*input_1 = 1;
      } 
      
      if (i==6 && bit_value == 1){
	*input_2 = 1;
      } 
      
      if (i==5 && bit_value == 1){
	*input_3 = 1;
      } 
      
      if (i==2 && bit_value == 1){
	*input_4 = 1;
      } 
      
      if (i==1 && bit_value == 1){
	*input_5 = 1;
      } 
      
    }
  
    delete[] data; 
    return NO_ERROR;
    
}

int move_motor(int motor_id, int direction, int steps, int step_time, int output_1, int output_2, int output_3 ) {
	
	unsigned char *data = new unsigned char[7]; //data to write
	int used_direction = 0;
	
	if (motor_id == 0){
	  data[0]= MOTOR_ONE_IDENTIFIER;
	
	  if (direction == 0){
	    used_direction = 1;  
	  }
	
	  if (direction == 1){
	    used_direction = 3;  
	  }
	
	}
	
	if (motor_id == 1){
	  data[0]= MOTOR_TWO_IDENTIFIER;
	
	  if (direction == 0){
	    used_direction = 4;  
	  }
	
	  if (direction == 1){
	    used_direction = 6;  
	  }
	
	}
	
	if (motor_id != 1 && motor_id != 0){
	  delete[] data;
	  return ERROR_INVALID_MOTOR_ID;
	}
	
	if (direction != 1 && direction != 0){
	  delete[] data;
	  return ERROR_INVALID_DIRECTION;
	}
	
	data[1]= used_direction;
	
	if (step_time > 128){
	data[2]= (step_time % 128) +1;
	} else {
	data[2]= step_time+1;  
	}
	
	data[3]= floor(step_time/128)+1;
	
	if (steps > 128){
	data[4]= (steps % 128) +1;
	} else {
	data[4]= steps+1;  
	}
	
	data[5]= floor(steps/128)+1;
	data[6]= 0;
	
	data[6] |= (output_1 << 0);
	data[6] |= (output_2 << 1);
	data[6] |= (output_3 << 2);

	int r, actual; //used to find out how many bytes were written
	
	r = libusb_bulk_transfer(dev_handle, 0x02, data, 7, &actual, 0); 
	if(r != 0 || actual != 7){
	        delete[] data; 
		return ERROR_USB_WRITING_ERROR;
	}

	delete[] data; //delete the allocated memory for data
	return NO_ERROR;	
}