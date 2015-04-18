#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

#define CHR_DEV_NAME "/dev/led_dev"

#define CMD_LED_OFF 0
#define CMD_LED_ON 1
#define CMD_SET_INTERVAL 3

int main(){

	int i;
	unsigned long count;
	unsigned long interval;

	int fd = open(CHR_DEV_NAME,O_WRONLY | O_NONBLOCK);
	if(fd < 0){
		fprintf(stderr,"open dev %s failed!\n",CHR_DEV_NAME);
		return -1;
	}
	printf("open %s success!\n",CHR_DEV_NAME);


//-------------------------------------------------------

	printf("turn on led.\n");
	ioctl(fd,CMD_LED_ON);
	sleep(1);

	printf("turn off led.\n");
	ioctl(fd,CMD_LED_OFF);
	sleep(2);

//--------------------------------------------------------

	printf("blink led 3 time...\n");
	count = 3;
	write(fd,&count,sizeof(int));
	printf("end blink.\n");
	sleep(2);

//-------------------------------------------------------

	interval = 1000;
	printf("set blink interval as %1dms\n",interval);
	ioctl(fd,CMD_SET_INTERVAL,&interval);
	
	printf("blink led 3 time again...\n");
	count = 3;
	write(fd,&count,sizeof(int));
	printf("end of second blink.\n");

//------------------------------------------------------

	close(fd);
	printf("bye\n");

	return 0;

}
