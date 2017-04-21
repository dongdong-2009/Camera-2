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
int  select_sim(int ret)
{
	int num_su = 0;
	if(ret < 2200 && ret > 1900)            {num_su = 10;}
        else if(ret > 2200 && ret < 2370)       {num_su = 11;}
        else if(ret > 2370 && ret < 2540)       {num_su = 12;}
        else if(ret > 2540 && ret < 2710)       {num_su = 13;}
        else if(ret > 2710 && ret < 2880)       {num_su = 14;}
        else if(ret > 2880 && ret < 3050)       {num_su = 15;}
        else if(ret > 3050 && ret < 3220)       {num_su = 16;}
        else if(ret > 3220 && ret < 3390)       {num_su = 17;}
        else if(ret > 3390 && ret < 3560)       {num_su = 18;}
        else if(ret > 3560 && ret < 3730)       {num_su = 19;}
        else if(ret > 3730 && ret < 3900)       {num_su = 20;}
	else if(ret < 1900 && ret > 1730)	{num_su = 9;}
	else if(ret < 1730 && ret > 1560)	{num_su = 8;}
	else if(ret < 1560 && ret > 1390)	{num_su = 7;}
	else if(ret < 1390 && ret > 1220)	{num_su = 6;}
	else if(ret < 1220 && ret > 1050)	{num_su = 5;}
	else if(ret < 1050 && ret >  880)	{num_su = 4;}
	else if(ret < 880  && ret >  710)	{num_su = 3;}
	else if(ret < 710  && ret >  540)	{num_su = 2;}
	else if(ret < 540  && ret >  370)	{num_su = 1;}
	else if(ret < 370 )	{num_su = 0;}
	return num_su;
}                             
rocker_send(int retx,int rety)
{
	int get_x,get_y;
	int x,y;
	int limit_x,limit_y;
	int LeftWheelTurn = 0,RightWheelTurn = 0;               //0:后转 1:前转 
        int SpeedLeft = 0,SpeedRight = 0;
	int adjustment;	

	get_x = select_sim(retx);
	get_y = select_sim(rety);

	printf("get_x: %d  get_y:%d\n",get_x,get_y);	
	if(get_x < 10){ x = 10 - get_x; limit_x = 0;}
	else{ x = get_x - 10;limit_x = 1; }
	if(get_y < 10){ y = 10 - get_y; limit_y = 0;}
	else{ y = get_y - 10;limit_y = 1; }
		
	printf("x: %d  y:%d\n",x,y);	

	if(limit_x == 1 && limit_y == 1)                //后右方
	{
		printf("第1像限\n");
		LeftWheelTurn = 0;
		SpeedLeft = (int )sqrt(x*x+y*y) ;
		if(SpeedLeft > 10)SpeedLeft = 10;
		if(x < 5){
			SpeedRight = SpeedLeft - x*SpeedLeft/5;
			RightWheelTurn = 0;
		}else{
			SpeedRight= x*SpeedLeft/5 - SpeedLeft;
			RightWheelTurn = 1;
		}
	}
	if(limit_x == 0 && limit_y == 1)                //后左方
	{
		printf("第2像限\n");
		LeftWheelTurn = 0;
		SpeedLeft = (int )sqrt(x*x+y*y);
		if(SpeedLeft > 10)SpeedLeft = 10;

		if(x < 5){
			SpeedRight = SpeedLeft - x*SpeedLeft/5;
			RightWheelTurn = 0;
		}else{
			SpeedRight= x*SpeedLeft/5 - SpeedLeft;
			RightWheelTurn = 1;
		}

	}
	if(limit_x == 1 && limit_y == 0)                //右前方
	{
		printf("第3像限\n");
		LeftWheelTurn = 1;
		SpeedLeft = (int )sqrt(x*x+y*y);
		if(SpeedLeft > 10)SpeedLeft = 10;
		if(x < 5){
			SpeedRight = SpeedLeft - x*SpeedLeft/5;
			RightWheelTurn = 1;
		}else{
			SpeedRight= x*SpeedLeft/5 - SpeedLeft;
			RightWheelTurn = 0;
		}
	}
	if(limit_x == 0 && limit_y == 0)                //左前方
	{
		printf("第4像限\n");
		LeftWheelTurn = 1;
		SpeedLeft = (int )sqrt(x*x+y*y) ;
		if(SpeedLeft > 10)SpeedLeft = 10;
		if(x < 5){
			SpeedRight = SpeedLeft - x*SpeedLeft/5;
			RightWheelTurn = 1;
		}else{
			SpeedRight= x*SpeedLeft/5 - SpeedLeft;
			RightWheelTurn = 0;
		}

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
















