#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include "camera.h"

#define DEV_NAME_LENGTH         50
#define NUM_FRAM                200

int main() {
    //char *dev_name = "/dev/video0";   //ITU
    //char *dev_name = "/dev/video4";   //UVC
    char *dev_name = NULL;
    FILE * outf = 0;
    unsigned int image_size;
    int i;

    int camera_type = 0;//0:ITU, 1:UVC  
    int width=720;
    int height=576;

        dev_name = (char *)malloc(sizeof(char) * DEV_NAME_LENGTH);
	 if(!dev_name)
        {
                printf("malloc mem error\n");
                return -1;
        }

        memset(dev_name, 0, sizeof(char) * DEV_NAME_LENGTH);
        strcpy(dev_name,"/dev/video0");


    outf = fopen("out.yuv", "wb");
    unsigned char image[width*height*2];

    clock_t starttime, endtime;
    double totaltime;
    
    Camera(dev_name,480,640, camera_type);
    if(!OpenDevice()){
        printf("fun:%s, line = %d\n", __FUNCTION__, __LINE__);
        return -1;
    }
        printf("fun:%s, line = %d\n", __FUNCTION__, __LINE__);
    image_size= getImageSize();
        printf("fun:%s, line = %d\n", __FUNCTION__, __LINE__);
    unsigned int writesize=0;
        printf("fun:%s, line = %d\n", __FUNCTION__, __LINE__);
   for( i=0;i< 3;i++){
        if(!GetBuffer(image)){
            break;
        }
    }
	CloseDevice();

    Camera(dev_name,width,height, camera_type);
    if(!OpenDevice()){
        printf("fun:%s, line = %d\n", __FUNCTION__, __LINE__);
        return -1;
    }
        printf("fun:%s, line = %d\n", __FUNCTION__, __LINE__);
    image_size= getImageSize();
        printf("fun:%s, line = %d\n", __FUNCTION__, __LINE__);
    starttime = clock();
    //int frames=50;
     writesize=0;
        printf("fun:%s, line = %d\n", __FUNCTION__, __LINE__);
    
   for( i=0;i< NUM_FRAM;i++){
        if(!GetBuffer(image)){
                printf("fun:%s, line = %d\n", __FUNCTION__, __LINE__);
            break;
        }
                printf("fun:%s, line = %d\n", __FUNCTION__, __LINE__);
        writesize=fwrite(image,1,image_size,outf);
        //fflush(outf);
        printf("frame:%d,writesize:%d\n",i,writesize);
    }

        printf("fun:%s, line = %d\n", __FUNCTION__, __LINE__);
    endtime = clock();
    totaltime = (double)( (endtime - starttime)/(double)CLOCKS_PER_SEC );
    printf("time :%f, rate :%f\n",totaltime,NUM_FRAM/totaltime);
    CloseDevice();
    fclose(outf);
    return 0;
}

