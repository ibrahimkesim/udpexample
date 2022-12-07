

*In ibrahim_kesim_case_1_epc file, there are udp_client_1.c and udp_server_1.c files
	
This system works on 127.0.0.1 IP adressv and 2123.

The server and client receive 2123 Port as arguments while running 

Firstly Server is running and then client. The server sends the PLMN data and the client is decodind data then print out. 
Secondly client encodes MCC and MNC values to PLMN value and prints out them.

Compiling command when terminal in udp_client_1.c and udp_server_1.c files.

	gcc udp_server_1.c -o server

running server

	./server 2123

compiling client

	gcc udp_client_1.c -o client -lpthread

running client

	./client 2123


Following text prints out:

[+]Data send: Hello, World!
[+]Data recv: [PLMN:05f539]
MNC has two digits
mcc 505
mnc 93
[PLMN:05f539]




- 