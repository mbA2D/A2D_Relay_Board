/* A2D Electronics 64 Channel Data Acquisition Module Library
 * Written By: Micah Black
 * Date: Jan 22, 2021
 *
 *
 *
 *
 */


#include <A2D_Relay_Board.h>

//constructor and initialization list
A2D_Relay_Board::A2D_Relay_Board()
{	
	
}

void A2D_Relay_Board::init()
{
	reset();
}

void A2D_Relay_Board::reset()
{
	//LED
	pinMode(LED_PIN, OUTPUT);
	digitalWrite(LED_PIN, LOW);
	
	pinMode(CH0_PIN, OUTPUT);
	pinMode(CH1_PIN, OUTPUT);
	digitalWrite(CH0_PIN, !CH_EN);
	digitalWrite(CH1_PIN, !CH_EN);
}

void A2D_Relay_Board::set_led(bool state)
{
	digitalWrite(LED_PIN, state);
}

void A2D_Relay_Board::set_dig_out(uint8_t channel, bool output_val)
{
	//check valid channel
	if (!_valid_channel(channel)) return;
	
	if (channel == 0) digitalWrite(CH0_PIN, output_val);
	if (channel == 1) digitalWrite(CH1_PIN, output_val);
}

bool A2D_Relay_Board::_valid_channel(uint8_t channel)
{
	return (channel < NUM_CHANNELS);
}
