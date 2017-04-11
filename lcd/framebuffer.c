#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "framebuffer.h"
#include <linux/fb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

int Framex = 0;

int init_FrameBuffer(void)  
{  
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    Frame_fd = open("/dev/fb0" , O_RDWR);  
    if(-1 == Frame_fd)  
    {  
        perror("open frame buffer fail");  
        return -1 ;   
    }  
    
    // Get fixed screen information
    if (ioctl(Frame_fd, FBIOGET_FSCREENINFO, &finfo))
    {
        printf("Error reading fixed information.\n");
        exit(0);
    }

    // Get variable screen information
    if (ioctl(Frame_fd, FBIOGET_VSCREENINFO, &vinfo)) 
    {            
        printf("Error reading variable information.\n");
        exit(0);
    }
    //这里把整个显存一起初始化（xres_virtual 表示显存的x，比实际的xres大,bits_per_pixel位深）
    screensize = vinfo.xres_virtual * vinfo.yres_virtual * vinfo.bits_per_pixel / 8;
    //获取实际的位色，这里很关键，后面转换和填写的时候需要
    Framebpp = vinfo.bits_per_pixel;
    printf("%dx%d, %dbpp  screensize is %ld\n", vinfo.xres_virtual, vinfo.yres_virtual, vinfo.bits_per_pixel,screensize);
    
    //映射出来，用户直接操作
    FrameBuffer = mmap(0, screensize, PROT_READ | PROT_WRITE , MAP_SHARED , Frame_fd ,0 );  
    if(FrameBuffer == (void *)-1)  
    {  
        perror("memory map fail");  
        return -2 ;  
    }  
    return 0 ;   
}  

void rgb16torgb24(void*buffer,void*image){
	rgb16 *rgb_565 = (rgb16 *)buffer;
	rgb24 *rgb_888 = (rgb24 *)image;
	int row, column;
	int num = 0;
	for(row = 0; row < 272; row++)
	{
		for(column = 0; column < 480; column++)
		{
			rgb_888[num].r  = ((rgb_565[num].x + (rgb_565[num].y << 8) & RGB565_RED))    >> 8;  
			rgb_888[num].g  = ((rgb_565[num].x + (rgb_565[num].y << 8) & RGB565_GREEN))  >> 3;  
			rgb_888[num].b  = ((rgb_565[num].x + (rgb_565[num].y << 8) & RGB565_BLUE))   << 3;  
			
			/*
			//sleep(1);
			//printf("x:%d y:%d,num:%d",rgb_565[num].x,rgb_565[num].y,(rgb_565[num].x + (rgb_565[num].y << 8)));	
	//		fflush(NULL);	
			rgb_888[num].b =((rgb_565[num].x + (rgb_565[num].y << 8))&0xF800) << 3;
			rgb_888[num].g =((rgb_565[num].x + (rgb_565[num].y << 8))&0x7E0) << 2;
			rgb_888[num].r =((rgb_565[num].x + (rgb_565[num].y << 8))&0x1F)<< 3;
		 	*/num++;
		}
	}
}

//写入framebuffer   fbp：帧缓冲首地址   fbfd：帧缓冲fd   img_buf:采集到的图片首地址  width：用户的宽 height：用户的高  bits：帧缓冲的位深 
int write_data_to_fb(void *fbp, int fbfd, void *img_buf, unsigned int img_width, unsigned int img_height, unsigned int img_bits)
{   
	int row, column;
	int num = 0;        //img_buf 中的某个像素点元素的下标
	rgb32_frame *rgb32_fbp = (rgb32_frame *)fbp;
	rgb24 *rgb32_img_buf = (rgb24 *)img_buf;    
	//   int *fd = (int *)fbp;    
	//     char *buf = (char *)img_buf;

	//防止摄像头采集宽高比显存大
	if(screensize < img_width * img_height * img_bits / 8)
	{
		printf("the imgsize is too large\n");
		return -1;
	}
	int i;
	/*不同的位深度图片使用不同的显示方案*/
	switch (img_bits)
	{
		case 32:
			for(row = 0; row < img_height; row++)
			{
				for(column = 0; column < img_width; column++)
				{
					//由于摄像头分辨率没有帧缓冲大，完成显示后，需要强制换行，帧缓冲是线性的，使用row * vinfo.xres_virtual换行
				
				//printf(" -------------%d-\n",sizeof(rgb32_img_buf[num]));r
					rgb32_fbp[row*960+column].r = rgb32_img_buf[num].r;
					rgb32_fbp[row*960+column].g = rgb32_img_buf[num].g;
					rgb32_fbp[row*960+column].b = rgb32_img_buf[num].b;

					//printf("---------r:%d,g:%d,b:%d------\n",rgb32_img_buf[num].r,rgb32_img_buf[num].g,rgb32_img_buf[num].b);
					num++;
				}
			}  
			break;
		default:
			break;
	}
	return 0;
}


//退出framebuffer  
int exit_Framebuffer(void)  
{  
    munmap(FrameBuffer , screensize);  
    close(Frame_fd);  
    return 0 ;   
}
