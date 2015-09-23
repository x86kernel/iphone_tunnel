all:
	@make clean && make iphone_tunnel

debug:
	@gcc tunnel.c socket_tcp.c socket_device.c -limobiledevice -lpthread -g -o iphone_tunnel

iphone_tunnel:
	@gcc -std=c99 tunnel.c socket_tcp.c socket_device.c -limobiledevice -lpthread -o iphone_tunnel

clean:
	@rm -f iphone_tunnel
