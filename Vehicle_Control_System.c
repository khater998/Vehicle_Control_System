/*
 ============================================================================
 Name        : Project_1.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Vehicle Control System
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

/* errors that may happen in functions */
typedef enum{
	NO_ERROR,
	INVALID_INPUT,
}EN_error_type;

/* First Menu options */
typedef enum{
	TURN_ON = 'a',
	TURN_OFF,
	QUIT
}EN_operation;

/* Second Menu options */
typedef enum{
	TURN_OFF_ENGINE = 'a',
	SET_TRAFFIC_LIGHT_COLOR,
	SET_ROOM_TEMP,
	SET_ENGINE_TEMP
}EN_sensor_menu;

#define WITH_ENGINE_TEMP_CONTROLLER 1

#define DEFAULT_SPEED 30
#define DEFAULT_ROOM_TEMP 20
#define DEFAULT_ENGINE_TEMP 150

#define ON 1
#define OFF 0

int vehicle_speed = DEFAULT_SPEED;

float room_temp = DEFAULT_ROOM_TEMP;
char AC_state = OFF;

typedef struct{
	char state;
	float temp;
	char temp_controller_state;
	float temp_controller_degree;
}engine_t;

engine_t engine = {OFF, DEFAULT_ENGINE_TEMP, OFF, 0.0};

EN_error_type getOperation(EN_operation *op); // capture user input from first menu options
EN_error_type sensorsConfig(char *engine_off_flag); // capture user input from second menu options
EN_error_type getTrafficLightColor(void); // get traffic light color to set vehicle speed
void configAC(void);  // to adjust AC configurations
void configEngineTemp(void);  // to adjust engine temperature controller configurations
void checkVehicleSpeed(void); // check if vehicle speed is 30 to take specific actions
void displayVehicleState(void);



int main(void) {
	EN_error_type err_state; // to check the error state of every function
	EN_operation operation;  // to pass as a function argument or receive function return
	char engine_off_flag = 0;// flag that is set if engine is turned on

	while (1)
	{
		/* in the main menu asking user for input */
		do{
			/* ask user for input */
			err_state = getOperation(&operation);
			if (err_state == INVALID_INPUT)
			{
				/* user input invalid character */
				printf("INVALID INPUT !!!\n");
			}
			else{
				/* user wants to end the program */
				if(QUIT == operation)
				{
					return EXIT_SUCCESS; /* return from main function */
				}else{}
			}
		/* if user enters invalid character or wants to turn off the engine,
		 	  stay in the main menu, and ask for input */
		}while (err_state != NO_ERROR || operation == TURN_OFF);

		/* Second menu (after the user turns on the engine) */
		do{
			/* ask user for input */
			err_state = sensorsConfig(&engine_off_flag);
			if (err_state == INVALID_INPUT)
			{
				/* user input invalid character */
				printf("INVALID INPUT !!!\n");
			}else
			{
				displayVehicleState();
			}

		/* stay in the menu till user turns off the engine */
		}while (1 != engine_off_flag);
	}

	return EXIT_SUCCESS;
}

/****************************************************************************
 **************************   getOperation()   ******************************
 ****************************************************************************/
EN_error_type getOperation(EN_operation *op)
{
	EN_error_type error_state = NO_ERROR;
	/* user input invalid character */
	char input; // to store the user input
	printf("\nMain Menu:\n");
	printf("a. Turn on the vehicle engine\n");
	printf("b. Turn off the vehicle engine\n");
	printf("c. Quit the system\n\n");
	fflush(stdout);

	scanf(" %c", &input);
	fflush(stdin);


	if (input == QUIT)
	{
		*op = QUIT;
		printf("PROGRAM ENDED\n");	}
	else if (input == TURN_ON)
	{
		*op = TURN_ON;
		engine.state = ON;
		printf("Vehicle Engine is now TURNED ON\n");
	}
	else if (input == TURN_OFF)
	{
		*op = TURN_OFF;
		engine.state = OFF;
		printf("Vehicle Engine is now TURNED OFF\n");
	}
	else
	{
		error_state = INVALID_INPUT;
	}

	return error_state;
}
/****************************************************************************
 ****************************************************************************
 ****************************************************************************/

/****************************************************************************
 **************************   sensorsConfig()   *****************************
 ****************************************************************************/
EN_error_type sensorsConfig(char *engine_off_flag)
{
	/* create a variable to track errors */
	EN_error_type err_state = NO_ERROR;

	/* ask user for input */
	char input;
	printf("\nSensor Set Menu:\n");
	printf("a. Turn off the engine\n");
	printf("b. Set the traffic light color\n");
	printf("c. Set the room temperature\n");

#if(WITH_ENGINE_TEMP_CONTROLLER == 1)
	printf("d. Set the engine temperature\n");
#endif
	fflush(stdout);

	scanf(" %c", &input);

	/* check user input */
	if (input == TURN_OFF_ENGINE) 					// user input 'a'
	{
		printf("Engine is now TURNED OFF\n");
		//engine_state = OFF;
		engine.state = OFF;	// turn off engine
		*engine_off_flag = 1; 				// set the engine off flag
	}
	else if (input == SET_TRAFFIC_LIGHT_COLOR) 		// user input 'b'
	{
		/* keep asking user for a valid input (G, O, or R) */
		while (getTrafficLightColor() != NO_ERROR){};
	}
	else if (input == SET_ROOM_TEMP) 					// user input 'c'
	{
		/* call a function to set room temperature */
		configAC();
	}
#if(WITH_ENGINE_TEMP_CONTROLLER == 1)
	else if (input == SET_ENGINE_TEMP)					// user input 'd'
	{
		/* call a function to set engine temperature */
		configEngineTemp();
	}
#endif
	else{
		err_state = INVALID_INPUT; 					// user entered invalid character
	}

	return err_state;
}
/****************************************************************************
 ****************************************************************************
 ****************************************************************************/

/****************************************************************************
 **************************   getTrafficLightColor()   ***************************
 ****************************************************************************/
EN_error_type getTrafficLightColor(void)
{
	EN_error_type err_state = NO_ERROR;

	/* ask user for input */
	char traffic_light;
	printf("Enter Traffic Light Color: ");
	fflush(stdout);
	scanf(" %c", &traffic_light);

	switch(traffic_light)
	{
	case 'G':
	case 'g':
		vehicle_speed = 100;
		break;
	case 'O':
	case 'o':
		vehicle_speed = 30;
		break;
	case 'R':
	case 'r':
		vehicle_speed = 0;
		break;
	default:
		err_state = INVALID_INPUT;
	}
	checkVehicleSpeed();
	return err_state;
}
/****************************************************************************
 ****************************************************************************
 ****************************************************************************/

/****************************************************************************
 **************************   configEngineTemp()   **************************
 ****************************************************************************/
void configAC(void)
{
	int AC_temp; // to adjust AC temperature
	/* ask user for input */
	printf("Enter Room Temperature: \n");
	fflush(stdout);
	scanf("%f", &room_temp);

	if (room_temp < 10 || room_temp > 30)
	{
		/* turn on AC and set its temperature to 20 */
		printf("AC is TURNED ON\n");
		AC_state = ON;
		AC_temp = 20;
		printf("AC TEMPERATURE: %d\n", AC_temp);
		fflush(stdout);
	}
	else
	{
		/* turn off AC */
		printf("AC is TURNED OFF\n");
		AC_state = OFF;
		fflush(stdout);
	}
}
/****************************************************************************
 ****************************************************************************
 ****************************************************************************/

/****************************************************************************
 **************************   configEngineTemp()   **************************
 ****************************************************************************/
void configEngineTemp(void)
{
	/* ask user for input */
	printf("Enter engine Temperature: \n");
	fflush(stdout);
	scanf("%f", &engine.temp);


	if (engine.temp < 100 || engine.temp > 150)
	{
		printf("Engine Temperature Controller is now TURNED ON\n");
		engine.temp_controller_state = ON;
		engine.temp_controller_degree = 125;
		printf("Controller TEMPERATURE: %f\n", engine.temp_controller_degree);
		fflush(stdout);
	}
	else
	{
		printf("Engine Temperature Controller is TURNED OFF\n");
		engine.temp_controller_state = OFF;
		fflush(stdout);
	}
}
/****************************************************************************
 ****************************************************************************
 ****************************************************************************/

void checkVehicleSpeed(void)
{
	if (vehicle_speed == 30)
	{
		AC_state = ON;
		room_temp = room_temp * (5/4.0) + 1;

#if(WITH_ENGINE_TEMP_CONTROLLER == 1)
		engine.temp_controller_state = ON;
		engine.temp = engine.temp * (5/4.0) + 1;
#endif

	}else{}
}
/****************************************************************************
 **************************   displayVehicleState()   **************************
 ****************************************************************************/
void displayVehicleState(void)
{
	printf("\n#############################################################\n");
	ON == engine.state ? printf("Engine state: ON\n") : printf("Engine state: OFF\n");

	ON == AC_state ? printf("AC state: ON\n") : printf("AC state: OFF\n");

	printf("Vehicle Speed: %d\n", vehicle_speed);

	printf("Room Temperature: %.2f\n", room_temp);

#if(WITH_ENGINE_TEMP_CONTROLLER == 1)
	ON == engine.temp_controller_state ? printf("Engine Temperature Controller State: ON\n")
			: printf("Engine Temperature Controller State: OFF\n");

	printf("Engine Temperature: %.2f\n", engine.temp);
#endif
	printf("##############################################################\n\n");


}
