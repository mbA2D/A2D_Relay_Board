/*
AUTHOR: Micah Black, A2D Electronics
DATE: Dec 9, 2022
PURPOSE: This example implements some SCPI commands
        (that don't completely follow the susbystem style standard)
        to communicate with the relay board.
CHANGELOG:

*/

#include <A2D_Relay_Board.h>

#define MANUFACTURER  ("A2D Electronics")
#define DESCRIPTION ("Relay Board")
#define VERSION     ("V1.0.0")

//SERIAL DEFINES
#define BAUDRATE    57600
#define SER_BUF_LEN   32
#define END_CHAR    '\n'
#define NO_CMD      ""

//Macro for finding commands - F to store string literal
//in flash instead of memory
#define CMDIS(i,c) (!strcmp_P(i, PSTR(c)))

//Function Prototypes:
void parse_serial(char ser_buf[], char command[], uint8_t* channel_num, bool* value_bool, int16_t* value_int);

A2D_Relay_Board relay_board;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUDRATE);
  relay_board.init();
}

void loop() {
  // put your main code here, to run repeatedly:
  //Allocate memory for the serial buffer
  char ser_buf[SER_BUF_LEN];
  char command[SER_BUF_LEN];
  uint8_t chars_input = 0;
  uint8_t channel_num = 0;
  bool value_bool = 0;
  int16_t value_int = 0;
  
  //if serial data is available
  if(Serial.available()){
    //Read until a full command is received
    chars_input = Serial.readBytesUntil(END_CHAR, ser_buf, SER_BUF_LEN);
    //terminate the input string with NULL
    ser_buf[chars_input] = '\0'; 
    //if(chars_input == 0);//TODO - set a default command to read
    //}
	  parse_serial(ser_buf, command, &channel_num, &value_bool, &value_int);
  }
  else{
    strcpy(command, "NOCMD");
  }


  //NOCMD?
  if(CMDIS(command, "NOCMD")){
    ;
  }
  
  //*IDN?
  else if(CMDIS(command, "*IDN?")){
    Serial.print(MANUFACTURER);
    Serial.print(" ");
    Serial.print(DESCRIPTION);
    Serial.print(" ");
    Serial.println(VERSION);
    Serial.flush();
  }
  
  //*RST
  else if (CMDIS(command, "*RST")){
    relay_board.reset();
  }
  
  //*CLS
  else if (CMDIS(command, "*CLS")){
    ; //nothing since we don't have errors yet
  }
  
  //INSTR:DAQ:SET:OUTP (@ch),VAL //VAL is boolean 0 or 1
  else if (CMDIS(command, "INSTR:DAQ:SET:OUTP")){
    relay_board.set_dig_out(channel_num, value_bool);
  }
  
  //INSTR:DAQ:GET:NCHS to get the number of channels that the board has.
  else if (CMDIS(command, "INSTR:DAQ:GET:NCHS")){
    serial.println(relay_board.get_num_channels())
  }
  
  //INSTR:DAQ:SET:LED X VAL  //X is placeholder for channel
  //can be anything, but we need it for the basic parsing currently in use
  //VAL is boolean 0 or 1
  else if (CMDIS(command, "INSTR:DAQ:SET:LED")){
    relay_board.set_led(value_bool);
  }
}

void parse_serial(char ser_buf[], char command[], uint8_t *channel_num, bool *value_bool, int16_t *value_int){
  //All SCPI commands are terminated with newline '/n'
  //but the Serial.readBytesUntil discards the terminator
  //so do we need to add one to use strcmp?
  
  //we will assume only 1 command is sent at a time
  //so we don't have to deal with SCPI's ';' to send
  //multiple commands on the same line
  
  //split input string on space to extract the command
  //and the parameters
  //strtok replaces the delimeter with NULL to terminate
  //the string
  char delimeters[] = " ,";
  char* token;
  char channel_str[8];
  char value_str[8];
  
  token = strtok(ser_buf, delimeters);
  strcpy(command, token);
  token = strtok(NULL, delimeters);
  strcpy(channel_str, token);
  token = strtok(NULL, delimeters);
  strcpy(value_str, token);

  token = strtok(value_str, delimeters);
  *value_int = atoi(token);
  *value_bool = 0;
  if(*value_int >= 1)
    *value_bool = 1;
  
  strcpy(delimeters, "@)");
  token = strtok(channel_str, delimeters); //get rid of "(@"
  token = strtok(NULL, delimeters);
  *channel_num = atoi(token);
  if(*channel_num > 63)
    *channel_num = 63;
  else if(*channel_num < 0)
    *channel_num = 0;
}
