#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

/* seconddrvtest 
  */
  	unsigned char key_vals[2];

unsigned char key_scan(void)
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
int main(int argc, char **argv)
{
	int fd;
	unsigned char key;
	fd = open("/dev/key", O_RDWR);
	if (fd < 0)
	{
		printf("can't open!\n");
	}
	while (1)
	{
		read(fd, key_vals, sizeof(key_vals));
		key=key_scan();
		if(key)
		{
			switch(key)
			{
			 case 1:
			 printf("key1 !!\n");
			 break;
			 case 2:
			 printf("key2 !!\n");

			 break;
			 default:break;	
				
			}		
		}	
	}
	return 0;
}

