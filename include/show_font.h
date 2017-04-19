#ifndef _SHOW_FONT_H__
#define _SHOW_FONT_H__

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "framebuffer.h"
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "pthread_fun.h"

#define FONTDATAMAX 4096
#define MY_HANZI    1
#define MY_ASCII    0



rgb32_frame temp_show[MY_WIDTH*MY_HEIGHT]; //显示汉字的模板

unsigned int line_width;
unsigned int pixel_width;
//int fd_hzk16;		
unsigned char *hzkmem;

struct stat hzk_stat;

int init_HZK16(void ); 						//初始化字库

void lcd_put_chinese(int x, int y, unsigned char *str);		//显示汉字
	
void lcd_put_ascii(int x, int y, unsigned char c);		//显示字符

//void lcd_put_pixel(int x, int y, unsigned int color);

/*mode :	模式选择	删除字符:MY_ASCII  删除汉字: MY_HANZI	*/
void lcd_del(int x, int y, int mode);				//删除字符或汉字


#endif
