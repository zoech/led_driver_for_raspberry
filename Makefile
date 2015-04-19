# a simple demo character type device driver kernel module
obj-m := driver_led.o
KERNELDIR := ~/others/bin/rasp/linux/
PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
clean:
	rm -rf *.o *.ko *.mod.* *.order *.symvers .tmp_versions .*.cmd *~
install:
	scp driver_led.ko pi@192.168.1.111:~/
