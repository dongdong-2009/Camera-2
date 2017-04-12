#include "pthread_fun.h"



static int key_falg = 0;

void *camera_show(void * c) //摄像头线程函数
{
        char buffer[MY_WIDTH*MY_HEIGHT*2];
        char image[MY_WIDTH*MY_HEIGHT*3];
        clock_t starttime, endtime;
        double totaltime;

        open_cameral(VIDEO_PATH);
        init_FrameBuffer();
        get_camInfo();
        set_Camera_format(MY_WIDTH,MY_HEIGHT);

        map_buf();

        printf("-------%s,line = %d\n",__FUNCTION__,__LINE__);
        startcon();     
        while(1){       
                get_picture(buffer);
                write_data_to_fb(fbmem, Frame_fd,buffer,MY_WIDTH,MY_HEIGHT,Framebpp);
                }
        stopcon();
        bufunmap();
        exit_Framebuffer();
        close_cameral();
        return 0;
}
static unsigned char key_scan(void)	//获取按键
{
         static unsigned char key_up = 1;
         if(key_up&&(key_vals[0]==0||key_vals[1]==0))
         {
                usleep(10000);
                key_up = 0;
                if(key_vals[0]==0)     return 1;
                else if(key_vals[1]==0) return 2;
         }else if(key_vals[0]==1&&key_vals[1]==1) key_up=1;
         return 0;
}

void *my_key_pth(void *c)
{
	int fd;
	unsigned char key;
	fd = open("/dev/key", O_RDWR);
	if (fd < 0)
	{
		printf("can't open!\n");
	}
	lcd_put_chinese(30,30,"关");
	while (1)
	{
		read(fd, key_vals, sizeof(key_vals));
		key=key_scan();
		if(key)
		{
			switch(key)
			{
				case 1:
					if(key_falg)
					{
						printf("-----%d----\n",key_falg);	
						lcd_del(30,30,MY_HANZI);
						lcd_put_chinese(30,30,"关");
						key_falg = 0;
						usart_count[2] = 0x00;
					}
					else 
					{
						printf("-----%d----\n",key_falg);	
						lcd_del(30,30,MY_HANZI);
						lcd_put_chinese(30,30,"开");
						key_falg = 1;
						usart_count[2] = 0x01;
					}
					break;
				case 2:
					printf("key2 !!\n");
					break;
				default:
					break;
			}
		}
	}
}

void *my_usart_pth(void *c){
	unsigned char buff[BUFFSIZE];
	open_uart();    //初始化串口
	while(1){
		serial_send(usart_count);
		printf("-----------1----------\n");
		//usleep(10000);
		sleep(1);
	//	read_buff(buff);
		
	//	memset(buff,0,BUFFSIZE);
	}
	close_uart();
} 

