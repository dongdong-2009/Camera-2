#include "pthread_fun.h";

int main()
{
	pthread_t camera_cap,my_key,my_usart;
	usart_count[0] =  0x55;
	usart_count[1] =  0x01;
	usart_count[2] =  0x00;
	usart_count[3] =  0xAA;
	init_HZK16();	//初始化字庫
	
	pthread_create(&camera_cap,NULL,camera_show,NULL);
	pthread_create(&my_key,NULL,my_key_pth,NULL);
	pthread_create(&my_usart,NULL,my_usart_pth,NULL);
	

	
	pthread_join(camera_cap,NULL);
	pthread_join(my_usart,NULL);
	pthread_join(my_key,NULL);
	return 0;
}

