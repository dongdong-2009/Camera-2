#ifndef _RGB2BMP_H__
#define _RGB2BMP_H__

#include <stdlib.h>
#include <stdio.h>

#define NEW_BMP "camera_cap.bmp"

typedef struct
{
	long imageSize;
	long blank;
	long startPosition;
}BmpHead;

typedef struct
{
	long    Length;
	long    width;
	long    height;
	unsigned short    colorPlane;
	unsigned short    bitColor;
	long    zipFormat;
	long    realSize;
	long    xPels;
	long    yPels;
	long    colorUse;
	long    colorImportant;
}InfoHead;


/***************************************************
        RGB加上头部信息转换成BMP
        参数说明:
        rgb_buffer      :数据文件中的信息
        nWidth          :图像宽度的像素数
        nHeight         :图像高度的像素数
        fp1             :所存放的文件

****************************************************/
int MY_RGB2BMP(unsigned char *rgb_buffer,int nWidth,int nHeight);

#endif
