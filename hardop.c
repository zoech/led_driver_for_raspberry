//#include<stdio.h>
//#include<stdlib.h>
#include<unistd.h>

//0x20200000 is the base gpio register address,and plus 4 is the second register 
#define reg_gpio_ctrl *(volatile int *)(0x20200000 + 4) 
#define reg_gpio_set *(volatile int *)(0x2020001c)
#define reg_gpio_clr *(volatile int *)(0x20200028)

void light_init(void){
	//set the 17th gpio output. the 3 bits of the 17th gpio fun select is 001
	reg_gpio_ctrl |= (1 << 21);
}

void light_on(void){
	reg_gpio_set |= (1 << 17);
}

void light_off(void){
	reg_gpio_clr |= (1 << 17);
}

int main(void){
	
	light_init();
	light_on();

	return 0;
}
