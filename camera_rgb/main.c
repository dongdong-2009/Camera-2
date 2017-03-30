#include <stdio.h>
#include "camera_rgb.h"

int  main()
{
 //打开摄像头设备
 open_cameral(VIDEO_PATH);
// get_camInfo();
 set_Camera_format(720,576);
 get_CameraBuf();
 map_buf();
 startcon();
#if 0
 //初始化帧缓冲
 init_FrameBuffer();  
 //获取当前摄像头的格式信息    
 get_camInfo();
 //设置用户需要的摄像头格式信息(分辨率和图形格式)
 set_format();
 //获取摄像头采集图片buf
 get_buf();
 //映射buf到用户空间
 map_buf();
 //开始采集
 startcon();
 while(1) //这里可以优化成select，就不会阻塞了
 {
     //获取采集到的数据
     get_picture(bmp);
     //把采集数据写入帧缓冲
     write_data_to_fb(FrameBuffer, Frame_fd, bmp, cam_width, cam_hight, Framebpp);    
 }
 
 //停止采集
 stopcon();  
 //解除映射
 bufunmap();
 //关闭帧缓冲
 exit_Framebuffer();  
#endif
 //关闭摄像头设备
// stopcon();
 //bufunmap();
 close_cameral();      
}
