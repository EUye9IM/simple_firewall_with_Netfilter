.PHONY : all clean install remove
subdir=$(shell ls -l | grep "^d" | awk '{print $$9}')

all:
	@for u in $(subdir);\
	do \
		if [ -e $$u/Makefile ]; then $(MAKE) -C $$u $@; fi \
	done

clean:
	@for u in $(subdir);\
	do \
		if [ -e $$u/Makefile ]; then $(MAKE) -C $$u $@; fi \
	done

install:
	@for u in $(subdir);\
	do \
		if [ -e $$u/Makefile ]; then $(MAKE) -C $$u $@; fi \
	done

remove:
	@for u in $(subdir);\
	do \
		if [ -e $$u/Makefile ]; then $(MAKE) -C $$u $@; fi \
	done