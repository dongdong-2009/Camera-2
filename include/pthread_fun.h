#ifndef _PTHREAD_FUN__H__
#define _PTHREAD_FUN__H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <pthread.h>
#include "framebuffer.h"
#include "camera_rgb.h"
#include "show_font.h"
#include "serial.h"
#include "rgb2bmp.h"


//------------------key.h-------------//
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NUM_FRAM                50
unsigned char key_vals[2]; 	//key个数
unsigned char usart_count[4]; 	//usart发个数

void *camera_show(void *); //摄像头采集线程函数

void *my_key_pth(void *); //按键线程函数

void *my_usart_pth(void *);	//串口收发date

#endif
