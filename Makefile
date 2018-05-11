.PHONY: all install uninstall
name = brainsimulation
cfiles = main.c $(name).c nodefunc.c brainsetup.c utils.c kernels.c
all: $(name)

$(name):$(cfiles)
	cc -Wall $(cfiles) -o $(name) -lpthread

install: $(name)
	echo "Must be run as root/sudo"
	cp -f $(name) /usr/local/bin
	chmod a+x /usr/local/bin/$(name)

uninstall:
	echo "Must be run as root/sudo"
	rm -f /usr/local/bin/$(name)

clean:
	rm -f $(name)
