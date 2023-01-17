/* A2D Electronics Relay Board Library
 * Written By: Micah Black
 * Date: Dec 9, 2022
 *
 * This class will deal with interfacing with the board
 * 
 */


#include "Arduino.h"
#include <A2D_Relay_Board_V1.0.h> //header file with pins, etc

#ifndef A2D_Relay_Board_h
#define A2D_Relay_Board_h

class A2D_Relay_Board
{
	public:
		A2D_Relay_Board(); //constructor
		
		//Configuration
		void init();
		void reset();
		
		//Interface
		void set_dig_out(uint8_t channel, bool output_val);
		uint8_t get_num_channels();
		void set_led(bool state);
		
	private:
		//************METHODS****************
		bool _valid_channel(uint8_t channel);

};

#endif