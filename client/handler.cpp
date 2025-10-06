#include "client.h"

//Função que recebe mensagens do servidor
void receive_messages(int sock){
    char buffer[BUFFER_SIZE];
    while(running){
        memset(buffer,0,BUFFER_SIZE);
        int bytes_received = recv(sock,buffer,BUFFER_SIZE,0);
        if (bytes_received <= 0){
            //Só exibe a mensagem se a flag é true, distinguindo do caso quando o cliente pede para sair
            if (running){
                std::cout << "[CLIENT] Conexão fechada pelo servidor!" << std::endl;
            }
            running = false;
            break;
        }
        std::cout << std::string(buffer,bytes_received) << std::flush;
    }
}
