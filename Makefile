obj-m += test.o

all:
	make -C /lib/moduels/`uname -r`/build M=$(PWD) modules
clean:
	make -C /lib/modules/`uname -r`/build M=$(PWD) clean
