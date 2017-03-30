#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <asm/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <linux/videodev2.h>
#include <sys/stat.h>
#include <malloc.h>
#include <errno.h>
#include <getopt.h>
#include <sys/mman.h>


#include "camera_rgb.h"

int xioctl(int fd, int request, void * arg)
{
   	int r;
	do
        r = ioctl(fd, request, arg);
	while (-1 == r && EINTR == errno);
	return r;
}

int open_cameral(char *dev_name)
{
	struct stat st;
	if (-1 == stat(dev_name, &st)) {
		fprintf(stderr, "Cannot identify '%s': %d, %s\n", dev_name, errno,
				strerror(errno));
		exit(1);
	}
	if (!S_ISCHR(st.st_mode)) {
		fprintf(stderr, "%s is no device\n", dev_name);
		exit(1);
	}
	fd = open(dev_name, O_RDWR /* required */| O_NONBLOCK, 0);
	if (-1 == fd) {
		fprintf(stderr, "Cannot open '%s': %d, %s\n", dev_name, errno,
				strerror(errno));
		exit(1);
	}
	return 0;
}

int get_camInfo(){
	struct v4l2_format fmt; 
	fmt.type=V4L2_BUF_TYPE_VIDEO_CAPTURE; 
	//获取当前摄像头的宽高
	xioctl(fd, VIDIOC_G_FMT, &fmt);
	printf("Current data format information:\n\twidth:%d\n\theight:%d\n",\
			fmt.fmt.pix.width,fmt.fmt.pix.height);

	struct v4l2_fmtdesc fmtdesc; 
	fmtdesc.index=0; 
	fmtdesc.type=V4L2_BUF_TYPE_VIDEO_CAPTURE; 
	//获取当前摄像头支持的格式
	while(xioctl(fd,VIDIOC_ENUM_FMT,&fmtdesc)!=-1)
	{
	//	if(fmtdesc.pixelformat & fmt.fmt.pix.pixelformat)
	//	{
			printf("\tformat:%s\n",fmtdesc.description);
	//		break;
	//	}
		fmtdesc.index++;
	}	
}

int set_Camera_format(int cam_width,int cam_hight){

	struct v4l2_input input;	
	struct v4l2_format fmt;
	CLEAR(input);
	int rtn = xioctl(fd, VIDIOC_S_INPUT, &input);
	if (rtn < 0) {
		printf("VIDIOC_S_INPUT:rtn(%d)\n", rtn);
		exit(1);
	}
	CLEAR (fmt); 
	fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE; //这里必须填这个
	fmt.fmt.pix.width       = cam_width;   //用户希望设置的宽
	fmt.fmt.pix.height      = cam_hight;   //用户希望设置的高
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB565;//选择格式：V4L2_PIX_FMT_YUYV或V4L2_PIX_FMT_MJPEG
	//fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
	int ret = 0;
	ret  = xioctl(fd,VIDIOC_S_FMT,&fmt);
	if (ret < 0) {
		printf("VIDIOC_S_FMT failed (%d)\n",ret);
		exit(1);
	}
	//如果用户传入超过了实际摄像头支持大小，摄像头会自动缩小成最大支持。这里把摄像头当前支持的宽高情况反馈给用户。
	cam_width = fmt.fmt.pix.width; 
	cam_hight = fmt.fmt.pix.height;

	printf("------------VIDIOC_S_FMT---------------\n");
	printf("Stream Format Informations:\n");
	printf(" type: %d\n", fmt.type);
	printf(" width: %d\n", fmt.fmt.pix.width);
	printf(" height: %d\n", fmt.fmt.pix.height);
	return 0;
}

int get_CameraBuf(){
	struct v4l2_requestbuffers req;    
	CLEAR(req);
	req.count = 4;  //摄像头图片缓存buf个数，这里一般设置4个
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;
	if (ioctl(fd,VIDIOC_REQBUFS,&req) <0)
	{
		perror("VIDIOC_REQBUFS error \n");
		return -1;
	}
	printf("get_CameraBuf seccue!\n");
	return 0;
}

void map_buf(void){
	int numBufs = 0;
	struct v4l2_buffer tmp_buf ;   //摄像头缓冲buf临时保存buf
	buffers = calloc(4, sizeof(*buffers));
	if (!buffers) {
        fprintf(stderr, "Out of memory\n");
	exit(1);    
	}
	for (numBufs = 0; numBufs < 4; numBufs++)
	{
		CLEAR(tmp_buf);
		tmp_buf.type =  V4L2_BUF_TYPE_VIDEO_CAPTURE ;   
		tmp_buf.memory = V4L2_MEMORY_MMAP ;   
		tmp_buf.index = numBufs;
		//获取内部buf信息到tmp_buf
		if (ioctl(fd, VIDIOC_QUERYBUF, &tmp_buf) < 0)
		{
			printf("VIDIOC_QUERYBUF (%d) error\n",numBufs);
			return;
		}
		buffers[numBufs].length = tmp_buf.length;
	//	buffers[numBufs].offset = (size_t) tmp_buf.m.offset;
		//开始映射
		buffers[numBufs].start = mmap (NULL, tmp_buf.length,PROT_READ | PROT_WRITE, MAP_SHARED, fd, tmp_buf.m.offset);
		if (buffers[numBufs].start == MAP_FAILED)
		{
			perror("pic_buffers error\n");
			exit(1);
		}
		//把设置好的buf入队列
		if (ioctl (fd, VIDIOC_QBUF, &tmp_buf) < 0)
		{
			printf("VIDIOC_QBUF error\n");
		}
	}
}

//开始采集
void startcon()
{
    enum v4l2_buf_type type;
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl (fd, VIDIOC_STREAMON, &type) < 0)
    {
        printf("VIDIOC_STREAMON error\n");
        // return -1;
    }
}

//获取采集到的数据
int get_picture(char *buffer)  
{  
/*    int ret ; 	  
    //把采集到图片的缓冲出队  
    ret = ioctl(fd,VIDIOC_DQBUF,&dequeue);  
    if(ret != 0)  
    {  
        perror("dequeue fail");  
        return -1 ;   
    }  
  
    //把图片数据放到buffer中
    memcpy(buffer,buffers[dequeue.index].start,buffers[dequeue.index].length);  

    //由于当前出队的缓冲数据已经拷贝到用户buffer中，这里可以重新入队用于后面的数据保存，构造起循环队列。
    enqueue.index = dequeue.index ;   
    ret = ioctl(fd , VIDIOC_QBUF , &enqueue);  
    if(ret != 0)  
    {  
        perror("enqueue fail");  
        return -2 ;   
    }  
    return 0 ;*/   
}  






//停止采集
int stopcon(void)  
{  
     //停止摄像头  
     int ret ;   
     int off= 1 ;   
     ret = ioctl(fd , VIDIOC_STREAMOFF, &off);  
     if(ret != 0)  
     {  
         perror("stop Cameral fail");  
         return -1 ;   
     }  
     return 0 ;  
}  

//停止映射
int bufunmap(void)  
{  
    int i ;   
    for(i = 0 ; i < CBUF_NUM ; i++)
    {
        munmap(pic_buffers[i].start , pic_buffers[i].length);
    }
    return 0 ;   
} 
 
//关闭设备
void close_cameral()
{
	close(fd);
}
