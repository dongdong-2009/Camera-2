#ifndef CAMERA_H
#define CAMERA_H

#include <sys/types.h>

#define CLEAR(x) memset (&(x), 0, sizeof (x))
#define bool char
#define true 1
#define false 0

typedef enum {
    IO_METHOD_READ, IO_METHOD_MMAP, IO_METHOD_USERPTR,
} io_method;

struct buffer{
    void *start;
    size_t length;//buffer's length is different from cap_image_size
};

char dev_name[50];
io_method io;
int fd;
int width;
int height;
struct buffer *buffers ;
unsigned int n_buffers ;
unsigned int cap_image_size ;//to keep the real image size!!

void Camera(char *DEV_NAME,int w,int h, int camer_type);

unsigned int getImageSize();
    
void CloseDevice();

void errno_exit(const char * s);
   
int xioctl(int fd, int request, void * arg);
    
int read_frame(unsigned char *image);
  
void stop_capturing(void);
    
bool start_capturing(void);
  
void uninit_device(void);
    
bool init_mmap(void);

bool init_device(void);
    
void close_device(void);

bool open_device(void);
  
bool OpenDevice();//´ò¿ªÉè±¸
    
bool GetBuffer(unsigned char *image);//»ñÈ¡Êý¾ÝÁ÷


#endif // CAMERA_H
