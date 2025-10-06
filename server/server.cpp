#include "Server.h"
#include <thread>


int main() {


    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening== -1) {
        std::cerr << "Erro na criacao do socket!" << std::endl;
        return -1;
    }

    int opt = 1;
    setsockopt(listening, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(PORT);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    if (bind(listening, (sockaddr*)&hint, sizeof(hint)) == -1) {
        std::cerr << "Erro no bind da porta  " << PORT << "(Verifique se a porta nao foi usada recentemente) !" << std::endl;
        close(listening);
        return -1;
    }

    listen(listening, SOMAXCONN);
    std::cout << "Escutando na porta " << PORT << ". Esperando clientes..." << std::endl;

    while (true) {
        int client_socket = accept(listening, nullptr, nullptr);
        if (client_socket == -1) {
            std::cerr << "Erro no aceite do cliente" << std::endl;
            continue;
        }

        std::thread client_thread(handle_client, client_socket);
        client_thread.detach();
    }

    close(listening);
    return 0;
}

