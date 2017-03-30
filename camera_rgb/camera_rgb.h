#ifndef _CAMERA_RGB_H_
#define _CAMERA_RGB_H_

#include <sys/types.h>
#define VIDEO_PATH "/dev/video0"
#define CLEAR(x) memset (&(x), 0, sizeof (x))


int fd;
int cam_width;
int cam_hight;
  
struct buffer{
    void *start;
    size_t length;//buffer's length is different from cap_image_size
};

struct buffer *buffers;

int xioctl(int fd, int request, void * arg);//重做ioctl防止被信号打断

int open_cameral(char *);	  //打开摄像头

int get_camInfo(void );		  //获取摄像头的支持格式信息

int set_Camera_format( int,int );//设置摄像头具体格式

int get_CameraBuf(void); 	 //获取摄像头图片采集的缓存buf

void map_buf(void); 		 //映射buf到用户空间

void startcon(void ); 		 //开始采集




int bufunmap(void)		//停止映射

int stopcon(void);		//结束采集

void close_cameral();		//关闭设备

#endif
