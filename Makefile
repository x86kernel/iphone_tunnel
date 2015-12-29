all: clean libcheck iphone_tunnel

debug:
	@gcc tunnel.c socket_tcp.c socket_device.c -limobiledevice -lpthread -g -o iphone_tunnel

iphone_tunnel:
	@gcc -std=c99 tunnel.c socket_tcp.c socket_device.c -limobiledevice -lpthread -o iphone_tunnel

libcheck:
	@ldconfig -p | grep libimobiledevice || (echo "Can not find the imobiledevice library, please install the libimobiledevice or libmobiledevice-dev first"; exit 1)

clean:
	@rm -f iphone_tunnel
