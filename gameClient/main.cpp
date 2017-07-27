#include "src/data/login_client.h"
#include "src/util/msg_queue.h"
#include "src/util/binary.h"
#include <chrono>

int main() {
        data::login_client client(1892, 1024);
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

        std::cout << client.requestSession(credentials) << std::endl;


        authentication::session_t session;

        if (client.waitForSession(session, 10)) {
            std::cout << session.accountID << " / " << session.sessionID << std::endl;
        } else {
            std::cout << "could not get session!" << std::endl;
        }

        client.close();
        return 0;


        util::msg_queue box(400);


        auto t1 = std::chrono::high_resolution_clock::now();

        auto w = box.write<int>();
        std::cout << sizeof(w) << std::endl;
        auto t2 = std::chrono::high_resolution_clock::now();


        auto duration =  std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
        std::cout << duration << std::endl;
        return 0;
}

