#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include "framebuffer.h"
#include "camera_rgb.h"

#define MY_WIDTH		480	
#define MY_HEIGHT		272
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
	int i;
	while(1){	
		get_picture(buffer);
		rgb16torgb24(buffer,image);
		write_data_to_fb(FrameBuffer, Frame_fd,image,MY_WIDTH,MY_HEIGHT,Framebpp);	
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
