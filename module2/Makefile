obj-m := linux-user-bde.o

linux-user-bde-y := gmodule.o
linux-user-bde-y += testmodule.o

all:
	make -C /lib/modules/`uname -r`/build M=$(PWD) modules
clean:
	make -C /lib/modules/`uname -r`/build M=$(PWD) clean