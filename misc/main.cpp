#include <iostream>

#include <thread>

#include "include/message/msg_controller.h"
#include "include/message/msg_info.h"
#include "include/message/msg_login.h"
#include "include/message/msg_logout.h"
#include "include/message/msg_lookup.h"

// #include "include/message/client/msg_info.h"
// #include "include/message/client/msg_login.h"
// #include "include/message/client/msg_logout.h"
// #include "include/message/client/msg_lookup.h"

#include "include/encryption/rsa.h"
#include "include/database/connection.h"


int main() {
    database::mysql::connection::user_data_t user;
    database::mysql::connection::server_adress_t server;

    user.username = "test";
    user.password = "testpw";

    server.host = "localhost";
    server.dbName = "test";

    database::mysql::connection connection;

    if (!connection.open(user, server)) {
        std::cout << connection.getErrorMsg() << std::endl;
    }

    message::msg_controller<data::context> controller(1000);
    data::context context;

    encryption::private_key privateKey;

    if (!privateKey.load("../keys/private.pem")) {
        std::cout << "Could not load private key!" << std::endl;
    }

    if (!context.init(connection, 37)) {
        std::cout << "Could not init context!" << std::endl;
        std::cout << connection.getErrorMsg() << std::endl;
    }

    controller.registerHandler<message::msg_info>();
    controller.registerHandler<message::msg_login>();
    controller.registerHandler<message::msg_logout>();
    controller.registerHandler<message::msg_lookup>();

    controller.init(5000, &context);
    controller.recv(&privateKey);

    return 0;
}


/*
void client() {
    data::client::context context;

    if (!context.getKey().load("../keys/public.pem")) {
        std::cout << "Could not load public key!" << std::endl;
    }

    message::msg_controller<data::client::context> controller(1000);

    controller.registerHandler<message::client::msg_info>();
    controller.registerHandler<message::client::msg_login>();
    controller.registerHandler<message::client::msg_logout>();
    controller.registerHandler<message::client::msg_lookup>();

    controller.init(5000, &context);

    encryption::public_key publicKey;

    if (!publicKey.load("../keys/public.pem")) {
        std::cout << "Could not load public key!" << std::endl;
    }


    network::ipv4_addr addr;

    addr.init("127.0.0.1", 5000);

    network::pkt_buffer buffer1(1500);
    network::pkt_buffer buffer2(1500);

    controller.execRequest<message::client::msg_info, data::client::context>(addr, buffer1, buffer2, context, nullptr, &publicKey);


    controller.recv(nullptr, &publicKey);

}



#include <chrono>
#include "include/util/mem.h"


int main() {
    client();
    return 0;


    encryption::private_key key;
    key.load("../keys/private.pem");
    encryption::public_key publicKey;
    publicKey.load("../keys/public.pem");


    unsigned char text[753];
    unsigned char rsaBuffer[2560];
    unsigned char rsaBuffer2[2560];



    int len = encryption::encryptChar(publicKey, 753, text, 2560, rsaBuffer);
    int len2 = encryption::decryptChar(key, len, rsaBuffer, 2560, rsaBuffer2);

//     text[1] = 5;
//     bool len2 = encryption::verifyChar(publicKey, len, rsaBuffer, 500, text);

    std::cout << "---------" << std::endl;
    std::cout << len << std::endl;
    std::cout << len2 << std::endl;


//     client();
    return 0;
}
*/
//
// #include <chrono>
//
// #include "include/message/msg_receiver.h"
// #include "include/message/server/msg_info.h"
// #include "include/message/server/msg_login.h"
//
//
//
//
//
//
//
// struct test {
//     char a;
//     char b;
//     char c;
// };
//
// struct large_test {
//     char pad[800000];
// };
//
// void print_test(test& t) {
//     std::cout << (int)t.a << " ";
//     std::cout << (int)t.b << " ";
//     std::cout << (int)t.c << " ";
//     std::cout << std::endl;
// }
//
// // template<typename T>
// void maskXOR(large_test* dest, large_test* src, large_test* mask) {
// //     char* t1 = (char*)&dest;
// //     char* t2 = (char*)&src;
// //     char* t3 = (char*)&mask;
// // std::cout << (int*)&dest << std::endl;
//
// //     for (int i = 0; i < sizeof(large_test); i++) {
// // //         ((char*)&dest)[i] = ((char*)&src)[i] ^ ((char*)&mask)[i];
// //         if (i < 1) {
// //
// // //             std::cout << i << std::endl;
// //
// // //             std::cout << (int*)t1 + i << std::endl;
// //         }
// //
// //     }
//
// }
//
// int foo0(int x) {
//     return x * x;
// }
//
// int foo1(int x) {
//     return x * x;
// }
//
// int foo2(int x) {
//     return x * x;
// }
//
// int foo3(int x) {
//     return x * x;
// }
//
//
// struct padtest {
//     char c1;
//     char c2;
//     char c3;
//     int i1;
//     char c4;
// };
//
// #include "include/encryption/rsa.h"
//
// struct rsa1 {
//     char data[200];
// };
//
// struct rsa2 {
//     char buff[256];
// };
//
// #include <chrono>
//
// int main(int argc, char** argv) {
//
//
//     encryption::private_key key;
//     key.load("../keys/private.pem");
//
//     int tmp;
//     auto t1 = std::chrono::high_resolution_clock::now();
//
//     for (int i = 0; i < 1000000000; i++) {
//          tmp = key.getRequiredSize();
//
//     }
//
//
//
//     auto t2 = std::chrono::high_resolution_clock::now();
//
//     auto duration =  std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
//     std::cout << duration << std::endl;
//
//
//
//
//     encryption::encrypter en;
//     encryption::decrypter de;
//
//     if (!en.load("../keys/public.pem")) {
//         std::cout << "Could not load public key!" << std::endl;
//         return 0;
//     }
//
//     if (!de.load("../keys/private.pem")) {
//         std::cout << "Could not load private key!" << std::endl;
//         return 0;
//     }
//
//     rsa1 src;
//     rsa2 src2;
//
//     rsa2 dest;
//
//     for (int i = 0; i < 10; i++) {
//         src.data[i] = i;
//     }
//
//     if (!en.encrypt<rsa1, rsa2>(&src, &dest)) {
//         std::cout << "Could not encrypt" << std::endl;
//         return 0;
//     }
//
//     if (!de.decrypt<rsa2, rsa2>(&dest, &src2)) {
//         std::cout << "Could not decrypt" << std::endl;
//         return 0;
//     }
//
//     for (int i = 0; i < 10; i++) {
//         std::cout << (int) src2.buff[i] << " ";
//     }
//
//     std::cout <<  std::endl;
//
//     return 0;
//
// //     message::receiver<data::login_context> receiver(1000);
// //     data::login_context d;
// //
// //     receiver.registerMsgHandler<message::msg_info>(1);
// //
// //     receiver.start(5000, &d);
//
//     /*
//
//         padtest pt;
//
//         std::cout << sizeof(pt) << std::endl;
//         std::cout << &pt << std::endl;
//         std::cout << (int*)&pt.c1 << std::endl;
//         std::cout << (int*)&pt.c2 << std::endl;
//         std::cout << (int*)&pt.c3 << std::endl;
//         std::cout << (int*)&pt.c4 << std::endl;
//         std::cout << &pt.i1 << std::endl;
//
//
//         return 0;
//
//         int (* func[8])(int);
//
//         for (int i = 0; i < 8; i++) {
//             func[i] = nullptr;
//         }
//
//         func[0] = &foo0;
//         func[1] = &foo1;
//         func[2] = &foo2;
//         func[3] = &foo3;
//
//
//     //     std::cout << sizeof(large_test) << std::endl;
//         auto tp1 = std::chrono::high_resolution_clock::now();
//
//
//         for (int i = 0; i < 1000000; i++) {
//     //         if (func[i % 4] != nullptr) {
//             func[i % 4](i);
//     //         }
//         }
//
//         auto tp2 = std::chrono::high_resolution_clock::now();
//
//         auto duration =  std::chrono::duration_cast<std::chrono::nanoseconds>(tp2 - tp1).count();
//         std::cout << duration << std::endl;
//
//         tp1 = std::chrono::high_resolution_clock::now();
//
//         for (int i = 0; i < 1000000; i++) {
//             switch (i % 4) {
//                 case 0: {
//                     foo0(i);
//                     break;
//                 }
//
//                 case 1: {
//                     foo1(i);
//                     break;
//
//                 }
//
//                 case 2: {
//                     foo2(i);
//                     break;
//
//                 }
//
//                 case 3: {
//                     foo3(i);
//                     break;
//
//                 }
//             }
//         }
//
//         tp2 = std::chrono::high_resolution_clock::now();
//
//         duration =  std::chrono::duration_cast<std::chrono::nanoseconds>(tp2 - tp1).count();
//         std::cout << duration << std::endl;
//
//
//     //     message::receiver receiver(1000);
//     //     receiver.start(5000);*/
//
// }
//

