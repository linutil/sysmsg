# sysmsg makefile

CC:=gcc

all:
	$(CC) main.c -o sysmsg -luuid

install: all
	mkdir -p /usr/local/share/sysmsg/
	chmod -R 777 /usr/local/share/sysmsg/
	mv sysmsg /usr/bin/sysmsg

remove:
	rm /usr/bin/sysmsg
	rm -rf /usr/local/share/sysmsg/