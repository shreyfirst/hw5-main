
# Makefile for f2022, ecs36b
# https://macappstore.org/libjson-rpc-cpp/
# https://github.com/cinemast/libjson-rpc-cpp

CC = g++ -std=c++17
CFLAGS = -g

LDFLAGS = 	-ljsoncpp -lmicrohttpd -ljsonrpccpp-common -ljsonrpccpp-server -ljsonrpccpp-client -lcurl

INC	=	ecs36b_Common.h
INC_CL	=	JvTime.h Person.o Thing.o #GPS.h Network.h JvTime.h Person.h Thing.h IOT_Thing.h Shadow_Thing.h Core.h
OBJ	=	JvTime.o Person.o Thing.o #GPS.o Network.o JvTime.o Person.o Thing.o IOT_Thing.o Shadow_Thing.o Core.o

# rules.
all: hw5Home hw5Forest hw5GrandmaHome # hw5client

#
#
# <target> : [... tab...] <dependency>
# [... tab ....] <action>


JvTime.o:	JvTime.cpp JvTime.h $(INC)
	$(CC) -c $(CFLAGS) JvTime.cpp

hw5client.h:		ecs36b_hw5.json
	jsonrpcstub ecs36b_hw5.json --cpp-server=hw5Server --cpp-client=hw5Client

hw5server.h:		ecs36b_hw5.json
	jsonrpcstub ecs36b_hw5.json --cpp-server=hw5Server

hw5client.o:		hw5client.cpp hw5client.h $(INC_CL) $(INC)
	$(CC) -c $(CFLAGS) hw5client.cpp

hw5server.o:		hw5server.cpp hw5server.h $(INC_CL) $(INC)
	$(CC) -c $(CFLAGS) hw5server.cpp

hw5Home.o:		hw5Home.cpp hw5server.h hw5client.h $(INC_CL) $(INC)
	$(CC) -c $(CFLAGS) hw5Home.cpp
	
hw5Forest.o:		hw5Forest.cpp hw5server.h hw5client.h $(INC_CL) $(INC)
	$(CC) -c $(CFLAGS) hw5Forest.cpp

hw5GrandmaHome.o:		hw5GrandmaHome.cpp hw5server.h hw5client.h $(INC_CL) $(INC)
	$(CC) -c $(CFLAGS) hw5GrandmaHome.cpp

ecs36b_JSON.o:		ecs36b_JSON.cpp $(INC)
	$(CC) -c $(CFLAGS) ecs36b_JSON.cpp

hw5client:	hw5client.o ecs36b_JSON.o $(OBJ)
	$(CC) -o hw5client hw5client.o ecs36b_JSON.o $(OBJ) $(LDFLAGS)

hw5server: hw5server.o ecs36b_JSON.o $(OBJ)
	$(CC) -o hw5server hw5server.o ecs36b_JSON.o $(OBJ) $(LDFLAGS)

hw5Home: hw5Home.o ecs36b_JSON.o hw5client.h $(OBJ)
	$(CC) -o hw5Home hw5Home.o ecs36b_JSON.o $(OBJ) $(LDFLAGS)

hw5Forest: hw5Forest.o ecs36b_JSON.o hw5client.h $(OBJ)
	$(CC) -o hw5Forest hw5Forest.o ecs36b_JSON.o $(OBJ) $(LDFLAGS)

hw5GrandmaHome: hw5GrandmaHome.o ecs36b_JSON.o hw5client.h $(OBJ)
	$(CC) -o hw5GrandmaHome hw5GrandmaHome.o ecs36b_JSON.o $(OBJ) $(LDFLAGS)

Person.o: Person.cpp Person.h
	$(CC) -c $(CFLAGS) Person.cpp $(LDFLAGS)
Thing.o: Thing.cpp Thing.h
	$(CC) -c $(CFLAGS) Thing.cpp $(LDFLAGS)

clean:
	rm -f *.o *~ core hw5client.h hw5server.h hw5client hw5server hw5Home hw5Forest hw5GrandmaHome Person.o Thing.o


