#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "stepper_bee_interface.h"

using namespace std;

int main() {
  
	int motor_one_status, motor_two_status, motor_one_steps, motor_two_steps;
	int reply;
	printf("TESTING STEPPER BEE INTERFACE \n");
	reply = init_controller();
	printf("Controller initialized %d \n", reply);	
	reply = set_modes(1,1,1,1);
	printf("motor mode set %d \n", reply);
	reply = get_motor_status(&motor_one_status, &motor_two_status, &motor_one_steps, &motor_two_steps) ;
	printf("INITIAL_STATE %d \n", reply);
	printf("motor 1 = %d \n", motor_one_status);
	printf("motor 2 = %d \n", motor_two_status);
	printf("steps 1 = %d \n", motor_one_steps);
	printf("steps 2 = %d \n", motor_two_steps);
	
	printf("Press any key to continue, forward direction 200 step test \n");
	getchar();
	
	reply = move_motor(0, 0, 200, 1, 0, 0, 0); 
	reply = move_motor(1, 0, 200, 1, 0, 0, 0);
	printf("motor moved %d \n", reply);
	reply = get_motor_status(&motor_one_status, &motor_two_status, &motor_one_steps, &motor_two_steps) ;
	
	while(motor_one_status == 1 || motor_two_status == 1){
	reply = get_motor_status(&motor_one_status, &motor_two_status, &motor_one_steps, &motor_two_steps) ;
	printf("status %d, MOTOR 1 - steps left = %d \n", motor_one_status, motor_one_steps);	
	printf("status %d, MOTOR 2 - steps left = %d \n", motor_two_status, motor_two_steps);	
	}
	
	printf("Press any key to continue, reverse direction 200 step test \n");
	getchar();
	
	reply = move_motor(0, 1, 200, 1, 0, 0, 0); 
	reply = move_motor(1, 1, 200, 1, 0, 0, 0);
	printf("motor moved %d \n", reply);
	reply = get_motor_status(&motor_one_status, &motor_two_status, &motor_one_steps, &motor_two_steps) ;
	
	while(motor_one_status == 1 || motor_two_status == 1){
	reply = get_motor_status(&motor_one_status, &motor_two_status, &motor_one_steps, &motor_two_steps) ;
	printf("status %d, MOTOR 1 - steps left = %d \n", motor_one_status, motor_one_steps);	
	printf("status %d, MOTOR 2 - steps left = %d \n", motor_two_status, motor_two_steps);	
	}
	
	close_controller();
	return 0;
	
}