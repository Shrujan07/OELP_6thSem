#include "uart.h" 
#include "string.h"
#include "pinmux.h"
#include "gpio.h"

#define HC05_UART uart_instance[1]
#define STR_SIZE 15


int write_to_hc05(char *data) {
	while (*data != '\0') {
		write_uart_character(HC05_UART, *data);
		data++;
	}
	write_uart_character(HC05_UART,'\r');
	write_uart_character(HC05_UART,'\n');

}

int write_to_hc05_obd2(char *data) {
	while (*data != '\0') {
		write_uart_character(HC05_UART, *data);
		data++;
	}
	write_uart_character(HC05_UART,'\r');

}


int write_enter_to_hc05(){
	write_uart_character(HC05_UART,'\r');
	write_uart_character(HC05_UART,'\n');
}

int read_from_hc05_setup(char *data) {
	int ch;
	char *str = data;
	char *test = data;
	for(int i=0;i<198;i++) {
		read_uart_character(HC05_UART,&ch);
		*str = ch;
		str++; 
		*str = '\0';
		if(strstr(test,"OK") != NULL)  {
			printf("read from hc05 %s\n",test);
			return 0;
		}
		if(strstr(test,"ERROR") != NULL) {
			printf("read from hc05 %s\n",test);
			return 0;
		}
		if(strstr(test,">") != NULL) {
			printf("read from hc05 %s\n",test);
			return 0;
		}
		if(strstr(test,"RECV") != NULL) {
			printf("read from hc05 %s\n",test);
			return 0;
		}
		if(strstr(test,"IP") != NULL) {
			printf("read from hc05 %s\n",test);
			return 0;
		}
	}
	*str = '\0';
	return ;
}

int read_from_hc05(char *data) {
	int ch;
	char *str = data;
	char *test = data;
	for(int i=0;i<100;i++) {
		read_uart_character(HC05_UART,&ch);
		*str = ch;
		str++; 
		*str = '\0';
		
		if((char)ch == '\n' || *(str-1) == '>') {
			//printf("received str = %s\n", test);
			//printf("Read uart char \ n = %d\n", (char)ch);
			return 0;
		}
	}
	*str = '\0';

	return ;
}


void setup_hc05(){
	char data[200];
	write_word(GPIO_DIRECTION_CNTRL_REG, 0x4);
	write_word(GPIO_DATA_REG, 0x4);
	
	printf(" Verifying AT status:\n");
	write_to_hc05("AT");
	delay(3);
	read_from_hc05_setup(data);
	printf(" Delete all autheticted devices AT RMAAD:\n");
	write_to_hc05("AT+RMAAD");
	delay(3);
	read_from_hc05_setup(data);
	printf(" Setting up AT ROLE\n");
	write_to_hc05("AT+ROLE=1");
	delay(3);
	read_from_hc05_setup(data);
	printf(" Checking AT ROLE\n");
	write_to_hc05("AT+ROLE?");
	delay(3);
	read_from_hc05_setup(data);
	printf(" Setting up AT CMODE\n");
	write_to_hc05("AT+CMODE=0");
	delay(3);
	read_from_hc05_setup(data);

	// printf(" setting AT PAIR\n");
	// write_to_hc05("AT+PAIR=1111,22,344405, 9");
	// delay(50);
	// read_from_hc05(data);

	printf(" sending AT BIND\n");
	write_to_hc05("AT+BIND=DOE6,5D,CF1D71");
	delay(5);
	read_from_hc05_setup(data);

	printf(" setting AT LINK\n");
	write_to_hc05("AT+LINK=DOE6,5D,CF1D71");
	delay(30);
	read_from_hc05_setup(data);

	// printf(" write AT+LINK\n");
	// write_to_hc05("AT+LINK=BT_ADRS");
	// delay(1);
	// read_from_hc05(data);

	write_word(GPIO_DATA_REG, 0x0);
	read_from_hc05(data);
}


void get_OBD_data(char* receiveData){
	char data[10];
	for (int i = 0; i < 100; i++){
		// Getting engine RPM
		write_to_hc05("010C");
		delay(2);
		read_from_hc05(receiveData + i*22 );

		// Getting vehicle speed
		write_to_hc05("010D");
		delay(2);
		read_from_hc05(receiveData + i*22 + 1*STR_SIZE);
		
		// Getting throttle position
		write_to_hc05("0111");
		delay(2);
		read_from_hc05(receiveData + i*22 + 2*STR_SIZE);
		
		// Getting engine load
		write_to_hc05("0104");
		delay(2);
		read_from_hc05(receiveData + i*22 + 3*STR_SIZE);

		// Getting coolant temp
		write_to_hc05("0105");
		delay(2);
		read_from_hc05(receiveData + i*22 + 4*STR_SIZE);
		
		// Getting long term fuel trim bank 1
		write_to_hc05("0107");
		delay(2);
		read_from_hc05(receiveData + i*22 + 5*STR_SIZE);
		
		// Getting short term fuel trim bank 1
		write_to_hc05("0106");
		delay(2);
		read_from_hc05(receiveData + i*22 + 6*STR_SIZE);
		
		// Getting intake manifold pressure
		write_to_hc05("010B");
		delay(2);
		read_from_hc05(receiveData + i*22 + 7*STR_SIZE);

		// Getting fuel tank level
		write_to_hc05("012F");
		delay(2);
		read_from_hc05(receiveData + i*22 + 8*STR_SIZE);
		
		// Getting absolute throttle position
		write_to_hc05("0147");
		delay(2);
		read_from_hc05(receiveData + i*22 + 9*STR_SIZE);
		
		// Getting pedal position D
		write_to_hc05("0149");
		delay(2);
		read_from_hc05(receiveData + i*22 + 10*STR_SIZE);
		
		// Getting pedal position E
		write_to_hc05("014A");
		delay(2);
		read_from_hc05(receiveData + i*22 + 11*STR_SIZE);
		
		// Getting commanded throttle actuator's value
		write_to_hc05("014C");
		delay(2);
		read_from_hc05(receiveData + i*22 + 12*STR_SIZE);
		
		// Getting commanded fuel-air equi. ratio
		write_to_hc05("0144");
		delay(2);
		read_from_hc05(receiveData + i*22 + 13*STR_SIZE);
		
		// Getting absolute barometric position
		write_to_hc05("0133");
		delay(2);
		read_from_hc05(receiveData + i*22 + 14*STR_SIZE);
		
		// Getting relative throttle position
		write_to_hc05("0145");
		delay(2);
		read_from_hc05(receiveData + i*22 + 15*STR_SIZE);
		
		// Getting intake air temperature
		write_to_hc05("010F");
		delay(2);
		read_from_hc05(receiveData + i*22 + 16*STR_SIZE);
		
		// Getting timing advance
		write_to_hc05("010E");
		delay(10);
		read_from_hc05(receiveData + i*22 + 17*STR_SIZE);
		
		// Getting Catalyst temperature from bank 1, sensor 1
		write_to_hc05("013C");
		delay(2);
		read_from_hc05(receiveData + i*22 + 18*STR_SIZE);
		
		// Getting Catalyst temperature from bank 1, sensor 2
		write_to_hc05("013D");
		delay(2);
		read_from_hc05(receiveData + i*22 + 19*STR_SIZE);
		
		// Getting Control module voltage
		write_to_hc05("0142");
		delay(2);
		read_from_hc05(receiveData + i*22 + 20*STR_SIZE);
		
		// Getting Commanded evaporative purge
		write_to_hc05("012E");
		delay(2);
		read_from_hc05(receiveData + i*22 + 21*STR_SIZE);
		
		// // Getting run-time with MIL on
		// write_to_hc05("014D");
		// delay(2);
		// read_from_hc05(receiveData + i*22 + 22);
		
		// // Getting run-time after clearing trouble codes
		// write_to_hc05("014E");
		// delay(2);
		// read_from_hc05(receiveData + i*22 + 23);

		// // Getting distance traveled with MIL on
		// write_to_hc05("0121");
		// delay(2);
		// read_from_hc05(receiveData + i*22 + 24);
		
		// // Getting warm ups since trouble codes cleared
		// write_to_hc05("0130");
		// delay(2);
		// read_from_hc05(receiveData + i*22 + 25);
		
	}
	// *(receiveData + (i+1)*22)='\0';
	delay(10);
}


void main()
{	write_word(GPIO_DIRECTION_CNTRL_REG, 0x4);
	write_word(GPIO_DATA_REG, 0x4);
	int i=0;
	int baudrate = 9600;
	//int baudrate = 115200;
	int transmit_count = 0;
   	printf("\n setting PIN MUX config to 2 .... \n");
    	*pinmux_config_reg = 0x5;
   	printf("\n set PIN MUX config to 2 .... \n");
	set_baud_rate(HC05_UART,baudrate);	
	printf("\n Baud rate set to %d \n",baudrate);
	printf("\n Waiting to HC05 to initialize \n");
	delay(3);
	setup_hc05();
	delay(3);
	char receiveData[100][22][STR_SIZE];
	get_OBD_data(*receiveData);
	for(int j=0; j<100; j++){
		for(int k=0; k<22; k++){
			printf("%s  ", receiveData[j][k]);
		}
		printf("\n");
	}
	int ch;
	printf("Entering while loop\n");
	while(1){
	char data_read[200];
	// read_from_hc05(data_read);
	// printf("%s", data_read);
	write_to_hc05("Hello World !");
	delay(10);
	}
}
