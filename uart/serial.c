#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <termio.h>
#include <syslog.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <semaphore.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <bits/signum.h>
#include <sys/resource.h>

#include "serial.h"

static unsigned char buf[BUFFSIZE];

static int uart_init(int arg, int baud)
{
    char port[20];
    int fd;
    struct termios Opt;
    int uartbiit[50]= {B115200,B9600,B19200,B4800,B2400,B1200};
    sprintf(port,"/dev/ttySAC%d",arg);
    printf("Use port: %s \n", port);
    fd = open(port, O_RDWR);
    if (fd<0)
    {
        return -1;
    }
    tcgetattr(fd,&Opt);
    tcflush(fd,TCIFLUSH);
    cfsetispeed(&Opt,uartbiit[baud]);
    cfsetospeed(&Opt,uartbiit[baud]);
    Opt.c_cflag |= CS8;
    Opt.c_cflag &= ~PARENB;
    Opt.c_oflag &= ~(OPOST);
    Opt.c_cflag &= ~CSTOPB;
    Opt.c_lflag &= ~(ICANON|ISIG|ECHO|IEXTEN);
    Opt.c_iflag &= ~(INPCK|BRKINT|ICRNL|ISTRIP|IXON);
    Opt.c_cc[VMIN] = 64;
    Opt.c_cc[VTIME] = 1;
    if (tcsetattr(fd,TCSANOW,&Opt) != 0)
    {
        perror("SetupSerial!\n");
        close(fd);
        return -1;
    }
    return fd;
}

void open_uart(void )                     // ¿¿¿¿2
{
        if((fd = uart_init(3, 0)) <0){
                perror("Open uart err \n");
                return ;
        }
}

void read_buff(char *ptr){
        int len = 0, i  = 0;
       
	memset(buf, 0 ,sizeof(buf));
        
	if((len = read(fd,buf,BUFFSIZE)) < 0)
	{
		perror("read error");
		exit(1);
	}
	else
	strncpy(ptr,buf,len); 
}

void serial_send(const char *buf)        //通过串口发送一传字符串
{
	size_t len,ret;
	len = sizeof(buf);
	if((ret = write(fd, buf,len)) < 0){
	perror("write device error");
	exit(1);	
	}
}

void close_uart()
{
	close(fd);
}


