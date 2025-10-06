//referencia : https://www.youtube.com/watch?v=cNdlrbZSkyQ (Construcao em geral)
//referencia : https://github.com/nnnyt/chat (Para o map)
//Por enquanto, use nc para testar o server
//Exemplo: nc localhost 8001
//Em outra aba deixe o server rodando



#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H


#include <iostream>
#include <string>
#include <vector>
#include <unordered_map> //Tomando o git acima de inpiracao, criamos pares de threads e usernames.
#include <mutex> //Mutex impede race conditions, principalmente em decorrencia do map que usamos para organizar cliente.
#include <thread>
#include <algorithm>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

// --- Organizacao ---

extern std::unordered_map<int, std::string> clients; //Criamos pares de threads e usernames.
//Cada cliente tem sua thread e todos possuem usernames diferentes 
extern std::mutex clients_mutex; 

const int PORT = 7992; //IMPORTANTE: Se estiver tendo problemas com o bind, mude a porta!!!
const int BUFFER_SIZE = 4096; //Tamanho maximo do buffer

void broadcast_message(int sender_socket, const std::string& message);
void handle_client(int client_socket);

#endif
