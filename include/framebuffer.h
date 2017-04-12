#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#define uint_8 char
#include <stdio.h>


#define MY_WIDTH		480	
#define MY_HEIGHT		544

#define RGB565_RED      0xf800  
#define RGB565_GREEN    0x07e0  
#define RGB565_BLUE     0x001f 


int Frame_fd;
long int screensize; 
unsigned char *fbmem;
unsigned int Framebpp;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;

int init_FrameBuffer(void);//初始化帧缓存

typedef struct {
        uint_8 x; 
        uint_8 y; 
} rgb16;

//rgb结构
typedef struct {
	uint_8 r; // 红色分量
	uint_8 g; // 绿色分量
	uint_8 b; // 蓝色分量
} rgb24;

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
