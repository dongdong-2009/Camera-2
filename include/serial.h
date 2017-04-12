#ifndef __SERIAL_H_
#define __SERIAL_H_

#define BUFFSIZE  	1024

int fd_uart;

void open_uart(void);		//初始化串口

void serial_send(const char *);	//通过串口发送一传字符串

void read_buff(char *);		//接受数据并回填

void close_uart(void );
#endif
