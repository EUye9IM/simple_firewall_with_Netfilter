NAME = simple_firewall

KERNEL_PATH :=/usr/src/kernels/$(shell uname -r)
CURRENT_PATH := $(shell pwd)
obj-m := $(NAME).o
$(NAME)-objs := main.o rules_list.o

.PHONY: all clean load unload
all:
	make -C $(KERNEL_PATH) M=$(CURRENT_PATH) modules
install:
	insmod $(NAME).ko
remove:
	rmmod $(NAME)
clean:
	make -C $(KERNEL_PATH) M=$(CURRENT_PATH) clean