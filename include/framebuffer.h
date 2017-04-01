#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#define uint_8 unsigned int
#include <stdio.h>

int Frame_fd;
long int screensize; 
char *FrameBuffer;
unsigned int Framebpp;

int init_FrameBuffer(void);//初始化帧缓存
//rgb结构
typedef struct {
	uint_8 r; // 红色分量
	uint_8 g; // 绿色分量
	uint_8 b; // 蓝色分量
	uint_8 rgbReserved; // 保留字节（用作Alpha通道或忽略）
} rgb32;

//帧缓冲中的rgb结构
typedef struct {
	uint_8 b; // 蓝色分量
	uint_8 g; // 绿色分量
	uint_8 r; // 红色分量
	uint_8 rgbReserved; // 保留字节（用作Alpha通道或忽略）
} rgb32_frame;



/************write_data_to_fb()************************
*写入framebuffer   
*fbp：帧缓冲首地址  
*fbfd：帧缓冲fd   
*img_buf:采集到的图片首地址  
*width：用户的宽 
*height：用>户的高  
*bits：帧缓冲的位深
*******************************************************/
int write_data_to_fb(void *fbp, int fbfd, void *img_buf, unsigned int img_width, \
			unsigned int img_height, unsigned int img_bits);

int exit_Framebuffer(void); //退出framebuffer 





#endif
