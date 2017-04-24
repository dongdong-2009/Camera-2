#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdint.h>
#include <termios.h>
#include <sys/ioctl.h>

#define CTL_ADC		0xc000fa01
#define ADC_NAME	"/dev/adc"
rocker_send(int retx,int rety)
{
	int x = 0,y = 0;
	int limit_x,limit_y;
	int LeftWheelTurn = 0,RightWheelTurn = 0;		//0:后转 1:前转	
	int SpeedLeft = 0,SpeedRight = 0;
	int adjustment;	
	
	if(retx < 2032){ x = 2032 - retx; limit_x = 0;}
	else {x = retx - 2032;limit_x = 1;}
	if(rety < 2026){ y = 2026 - rety; limit_y = 0;}
	else if(rety > 2026) {y = rety - 2026;limit_y = 1;}
	
	if(x > 2000) x = 2000;
	if(y > 2000) y = 2000;
	printf("x:%d  y:%d\n",x,y);
	
   	if(limit_x == 1 && limit_y == 1)		//后右方
	{
		printf("第一像限\n");
		LeftWheelTurn = 0;
                SpeedLeft = (int )((sqrt(x*x+y*y)*10)/2000) ;
		if(SpeedLeft == 11) SpeedLeft -= 1;
		if(SpeedLeft == 12) SpeedLeft -= 2;
                if(x < 1000){
                        SpeedRight = SpeedLeft - x*SpeedLeft/1000;
                        RightWheelTurn = 0;
                }else{
                        SpeedRight= x*SpeedLeft/850 - SpeedLeft;
                        RightWheelTurn = 1;
                }	
		adjustment  = SpeedLeft - SpeedRight;
		if(adjustment <= 2)
			SpeedLeft  = SpeedRight;
	}
	if(limit_x == 0 && limit_y == 1)		//后左方
	{
		printf("第2像限\n");
		  LeftWheelTurn = 0;
                SpeedLeft = (int )((sqrt(x*x+y*y)*10)/2000) ;
		if(SpeedLeft == 11) SpeedLeft -= 1;
		if(SpeedLeft == 12) SpeedLeft -= 2;
	
                if(x < 1000){
                        SpeedRight = SpeedLeft - x*SpeedLeft/1000;
                        RightWheelTurn = 0;
                }else{
                        SpeedRight= x*SpeedLeft/1000 - SpeedLeft;
                        RightWheelTurn = 1;
                }

	}
	if(limit_x == 1 && limit_y == 0)		//右前方
	{
		printf("第3像限\n");
	  	LeftWheelTurn = 1;
                SpeedLeft = (int )((sqrt(x*x+y*y)*10)/2000) ;
		if(SpeedLeft == 11) SpeedLeft -= 1;
		if(SpeedLeft == 12) SpeedLeft -= 2;

                if(x < 1000){
                        SpeedRight = SpeedLeft - x*SpeedLeft/1000;
                        RightWheelTurn = 1;
                }else{
                        SpeedRight= x*SpeedLeft/1000 - SpeedLeft;
                        RightWheelTurn = 0;
                }


	}	
	if(limit_x == 0 && limit_y == 0)		//左前方
	{
	printf("第4像限\n");
		  LeftWheelTurn = 1;
                SpeedLeft = (int )((sqrt(x*x+y*y)*10)/2000) ;
                if(SpeedLeft > 10) SpeedLeft = 10;
		if(x < 1000){
                        SpeedRight = SpeedLeft - x*SpeedLeft/1000;
                        RightWheelTurn = 1;
                }else{
                        SpeedRight= x*SpeedLeft/1000 - SpeedLeft;
                        RightWheelTurn = 0;
                }
		adjustment  = SpeedLeft - SpeedRight;
		if(adjustment <= 2)
			SpeedLeft  = SpeedRight;

	}
	printf("左轮方向:%d,右轮方向:%d,左轮档位:%d,右轮档位:%d\n",LeftWheelTurn,RightWheelTurn,SpeedLeft,SpeedRight);
}

int main(void){
	int fd;
	char buffer[50] = "\0";
	int len,ret1,ret2;
	int falg = 1;
	if((fd = open(ADC_NAME, O_RDWR|O_NOCTTY|O_NDELAY))<0)
	{
		perror("open faild\n");
		exit(1);
	}	
	while(1)
	{
		ioctl(fd,CTL_ADC,1);
		len=read(fd,buffer,50);//parameter 2 is datas,parameter 3 is datas length,parameter 4 default	
		ret1 = atoi(buffer);
		//ret1 = ret1*9000/4095;	//Datas  transition to Res
		ioctl(fd,CTL_ADC,2);
		len=read(fd,buffer,50);
		ret2 = atoi(buffer);
		printf("ret1 = %d,ret2 = %d \n",ret1,ret2);	
		rocker_send(ret1,ret2);
		//ret2 = ret1*9000/4095;
		sleep(1);
	}
}
















