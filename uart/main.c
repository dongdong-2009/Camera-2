#include <stdio.h>

#include "serial.h"

int main()
{
	open_uart();
	while(1){
	serial_send("hello world !!!");
	sleep(1);
	}
	close_uart();
	return 0;
}
