#include "Server.h"


//Essa funcao envia a mensagem para todos os clientes
//Importante: Nao queremos que o autor da mensagem receba a propria mensagem.
void broadcast_message(int sender_socket, const std::string& message) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    for (const auto& pair : clients) {
        if (pair.first != sender_socket) {
            send(pair.first, message.c_str(), message.length(),0);
        }
    }
}

// Conexao do cliente
void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    std::string username;

    
    while(true) {
        // O cliente digita o username
        std::string prompt = "Digite seu username: ";
        send(client_socket, prompt.c_str(), prompt.length(), 0);

        //Recebe o username do cliente
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        username = std::string(buffer, bytes_received);

        //Limpa o newline e o carriage return
        username.erase(std::remove(username.begin(), username.end(), '\n'), username.end());
        username.erase(std::remove(username.begin(), username.end(), '\r'), username.end());

        // Nao permitimos usuarios repetidos.
        bool is_duplicate = false;
        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            for (const auto& pair : clients) {
                if (pair.second == username) {
                    is_duplicate = true;
                    break;
                }
            }
        }

        if (is_duplicate) {
            std::string dup_warning = "[SERVER] Username ja utilizado, escolha outro!\n";
            send(client_socket, dup_warning.c_str(), dup_warning.length(), 0);
        } else {
            break;
        }
    }


    // Se deu tudo certo, o cliente eh adicionado a lista de cliente validos
    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients[client_socket] = username;
    }

    std::string join_message = "[SERVER] " + username + " entrou!\n";
    std::cout << join_message;
    broadcast_message(0, join_message);


    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            break; 
        }

        std::string message(buffer, bytes_received);

        //Corta mensagens de zero bytes
        while (!message.empty() && isspace(message.back())) {
            message.pop_back();
        }
        
        if (!message.empty()) {
            std::string broadcast_msg = "[" + username + "] " + message + "\n";
            
            std::cout << broadcast_msg;
            broadcast_message(client_socket, broadcast_msg);
        }
    }

    std::string leave_message = "[SERVER] " + username + " foi desconectado.\n";
    std::cout << leave_message; //Para o log do servidor
    broadcast_message(0, leave_message); //Para todos os outros clientes


    //IMPORTANTE: Depois que o usuario eh desconectado, apagamos o registro dele da lista
    //Caso contrario, ninguem mais poderia pegar o mesmo username
    //E esse usuario jamais poderia logar novamente
    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.erase(client_socket);
    }
    close(client_socket);
}
