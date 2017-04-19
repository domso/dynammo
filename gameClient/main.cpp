#include "include/data/login_client.h"
#include "include/util/msg_queue.h"
#include "include/util/binary.h"
#include <chrono>

#include <queue>
#include <mutex>


#include "include/util/sem.h"
#include "include/util/execution_pipeline.h"

#include "network/tcp_socket.h"
#include "network/tcp_connection.h"



int main() {

    data::login_client client(1891, 1024);
    encryption::public_key key;

    if (!key.load("../keys/public.pem")) {
        std::cout << "could not load key" << std::endl;
    }

    
    network::ipv4_addr addr;
    addr.init("127.0.0.1", 1890);
    
    client.setServer(addr, &key);


    client.init();

    client.requestInfo();
    authentication::server_info_t info;
    client.waitForInfo(info, 1);

    std::cout << "-------------" << std::endl;
    std::cout << "id: " << info.id << std::endl;
    std::cout << "uptime: " << info.uptime << std::endl;


    authentication::credentials_t credentials;
    util::mem::set<char[8]>(&credentials.username, 0);
    util::mem::set<char[128]>(&credentials.key, 0);

    credentials.username[0] = 'D';
    credentials.username[1] = 'o';
    credentials.username[2] = 'm';
    credentials.username[3] = 's';
    credentials.username[4] = 'o';

    credentials.key[0] = 't';
    credentials.key[1] = 'e';
    credentials.key[2] = 's';
    credentials.key[3] = 't';

//     std::cout << client.requestLogin(credentials) << std::endl;


    authentication::ticket_t ticket;
//     client.waitForLogin(ticket, 10);
    std::cout << "-------------" << std::endl;
    std::cout << "id: " << ticket.accountID << std::endl;
    std::cout << "issuer: " << ticket.issuer << std::endl;
    std::cout << "time: " << ticket.loginTime << std::endl;
    std::cout << "permission: " << ticket.permissions << std::endl;
    std::cout << "ip: " << ticket.userAddr.getIP() << std::endl;
    std::cout << "port: " << ticket.userAddr.getPort() << std::endl;


//     client.requestLogout();
//     client.waitForLogout(10);




    client.close();
    return 0;

    util::semaphore sem;
    util::msg_queue box(400);


    auto t1 = std::chrono::high_resolution_clock::now();

    auto w = box.write<int>();
    std::cout << sizeof(w) << std::endl;
    auto t2 = std::chrono::high_resolution_clock::now();


    auto duration =  std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
    std::cout << duration << std::endl;
    return 0;
}

