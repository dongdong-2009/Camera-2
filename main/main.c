#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include "framebuffer.h"
#include "camera_rgb.h"
#include "show_font.h"

#define NUM_FRAM                50

int main()
{
	char buffer[MY_WIDTH*MY_HEIGHT*2];
	char image[MY_WIDTH*MY_HEIGHT*3];
	clock_t starttime, endtime;
	double totaltime;

	//打开摄像头设备
	open_cameral(VIDEO_PATH);
	init_FrameBuffer();//初始化帧缓存
	get_camInfo();
	set_Camera_format(MY_WIDTH,MY_HEIGHT);

	map_buf();

	printf("-------%s,line = %d\n",__FUNCTION__,__LINE__);
	startcon();
	starttime = clock();
	init_HZK16();	//初始化字庫
	//memset(temp_show,0,MY_WIDTH*MY_HEIGHT*32);
//	lcd_put_chinese(30,30,"吕");
//	lcd_put_ascii(40,40,'A');
//	lcd_del(30,30,MY_HANZI);
	//lcd_del(35,35,MY_ASCII);
	while(1){	
		get_picture(buffer);
		write_data_to_fb(fbmem, Frame_fd,buffer,MY_WIDTH,MY_HEIGHT,Framebpp);	
		}
	//	endtime = clock();
	//	totaltime = (dogble)( (endtime - starttime)/(double)CLOCKS_PER_SEC );
	//	printf("time :%f, rate :%f\n",totaltime,NUM_FRAM/totaltime);
	//停止采集
	stopcon();  
	//解除映射
	bufunmap();
	//关闭帧缓冲
	exit_Framebuffer();  
	close_cameral();      
	return 0;
}
