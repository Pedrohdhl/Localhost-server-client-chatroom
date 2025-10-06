
#include "client.h"
#include <thread>

std::atomic<bool> running(true);

int main(){
    int sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock == -1){
        std::cerr << "[CLIENT] Erro ao criar socket!" << std::endl;
        return -1;
    }
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0){
        std::cerr << "[CLIENT] Endereço inválido!" << std::endl;
        return -1;
    }
    if(connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)) == -1){
        std::cerr << "[CLIENT] Não foi possível conectar ao servidor!" << std::endl;
        return -1;
    }
    std::cout << "[CLIENT] Conectado ao servidor na porta " << PORT << "." << std::endl;
    std::thread receiver(receive_messages, sock);
    std::string message;
    bool user_quit = false; //Flag para distinguir quando o server fecha e quando o cliente pede para sair 
    while (running){
        std::getline(std::cin, message);
        //Comando para sair do chat
        if (message == "/quit") {
            running = false;
            user_quit = true;
            shutdown(sock,SHUT_RDWR); //SHUT_RDWR desativa leitura e escrita
            break;
        }
        message += "\n";
        if (send(sock,message.c_str(),message.size(),MSG_NOSIGNAL) <= 0){
            std::cerr << "[CLIENT] Falha ao enviar a mensagem!" << std::endl;
            running = false;
            break;
        }
    }
    close(sock);
    if (receiver.joinable())
        receiver.join();
    if (user_quit)
        std::cout << "[CLIENT] Cliente saiu!" << std::endl;
    return 0;
}
