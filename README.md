# iphone_tunnel

itunnel_mux for linux

A simple command line tool which provides SSH tunnelling over the USB cable to a jailbroken iDevice

this require libimobiledevice and usbmux

## Usage

```
	make
	./iphone_tunnel [iphone_port] [local_port]

	ex) ./iphone_tunnel 22 9990
```

This will open 9990 port which can communicate with jailbroken iDevice's 22 port over the USB cable
