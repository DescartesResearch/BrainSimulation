.PHONY: all install uninstall
name = brainsimulation
all: $(name)

$(name): main.c $(name).c nodefunc.c
	cc -Wall main.c $(name).c nodefunc.c -o $(name)

install: $(name)
	echo "Must be run as root/sudo"
	cp -f $(name) /usr/local/bin
	chmod a+x /usr/local/bin/$(name)

uninstall:
	echo "Must be run as root/sudo"
	rm -f /usr/local/bin/$(name)

clean:
	rm -f $(name)
