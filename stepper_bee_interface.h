#ifndef stepper_bee_interface_h__
#define stepper_bee_interface_h__

extern int init_controller(void);

extern int close_controller(void);

extern int get_digital_input_status(int *input_1, 
				    int *input_2,
				    int *input_3, 
				    int *input_4,
				    int *input_5) ;
extern int move_motor(int motor_id, 
	       int direction, 
	       int steps, 
	       int step_time, 
	       int output_1, 
	       int output_2, 
	       int output_3 );

extern int get_motor_status(int *motor_one_status, 
		     int *motor_two_status, 
		     int *motor_one_steps, 
		     int *motor_two_steps) ;
		     
extern int set_modes (int motor_one_state,
		     int motor_two_state,
		     int motor_one_mode,
		     int motor_two_mode);
		 
		     		 
#endif // stepper_bee_interface_h__