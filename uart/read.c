#include <stdio.h>

#include "serial.h"


int main()
{
	char buff[1024];
	open_uart();

	while(1){
	read_buff(buff);
	printf("read-date:%s\n",buff);
	}
	close_uart();
	return 0;
}
