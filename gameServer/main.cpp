
#include <iostream>
#include "src/data/session_login_server.h"

int main() {
    // registration demo
    data::session_login_server loginConnector(1891, 1024);

    network::ipv4_addr addr;
    addr.init("127.0.0.1", 1890);
    encryption::public_key key;

    if (!key.load("../keys/public.pem")) {
        std::cout << "could not load key" << std::endl;
    }


    loginConnector.setServer(addr, &key);


    if (!loginConnector.init()) {
        std::cout << "could not init" << std::endl;
    }


    authentication::credentials_t userdata;

    util::mem::set<char[8]>(&userdata.username, 0);
    util::mem::set<char[128]>(&userdata.key, 0);

    userdata.username[0] = 'D';
    userdata.username[1] = 'o';
    userdata.username[2] = 'm';
    userdata.username[3] = 's';
    userdata.username[4] = 'o';

    userdata.key[0] = 't';
    userdata.key[1] = 'e';
    userdata.key[2] = 's';
    userdata.key[3] = 't';



    loginConnector.registerService(27, userdata);


    bool flag = false;

    if (!loginConnector.waitForRegisterState(flag, 10)) {
        std::cout << "could not connect to server" << std::endl;
        return 0;
    }

    if (!flag) {
        std::cout << "Auth-error!" << std::endl;
    } else {
        std::cout << "success!" << std::endl;
    }


    while (true) {

    }


    loginConnector.close();

    return 0;
}
