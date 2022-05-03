#include "RS485.h"

void RS485_init(){
	MODE = RECV;
}

void RS485_send_data(char *string){
	unsigned int i = 0;
    MODE = EMIT;
	while(string[i] != '\0'){
		SBUF = string[i++];
		while(!TI);
		TI = 0;
	}
	MODE = RECV;
}

