
#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H

#include <iostream>
#include <string>
#include <atomic>    //Para a flag
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

const int PORT = 7992;
const int BUFFER_SIZE = 4096;


extern std::atomic<bool> running; //Flag para controlar a execução

void receive_messages(int sock);

#endif 
