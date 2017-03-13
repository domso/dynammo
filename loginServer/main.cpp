#include <iostream>
#include "signal.h"
#include "include/message/msg_controller.h"
#include "include/message/msg_info.h"
#include "include/message/msg_login.h"
#include "include/message/msg_logout.h"
#include "include/message/msg_lookup.h"
#include "include/encryption/rsa.h"
#include "include/database/connection.h"
#include "include/util/config_file.h"


bool checkConfigFile(util::config_file& config) {
    config.requireString("dbUsername");
    config.requireString("dbPassword");
    config.requireString("dbHost");
    config.requireString("dbName");

    config.requireNumeric<int32_t>("serverID");
    config.requireNumeric<uint16_t>("port");

    config.recommendNumeric<int>("bufferSize", 1000);

    config.recommendString("privateKey", "");

    if (config.isError()) {
        std::cout << config.getErrorMsgs();
        std::cout << config.getWarningMsgs();
        return false;
    }

    if (config.isWarning()) {
        std::cout << config.getWarningMsgs();
    }

    return true;
}

message::msg_controller<data::context>* GlobalControllerLink = nullptr;

void stopServer(int signal) {
    std::cout << signal << std::endl;
    if (GlobalControllerLink != nullptr) {
        GlobalControllerLink->close();
    }
}

int main(int argc, char* argv[]) {
    
    std::cout << "[Info] Starting server..." << std::endl;
    
    if (argc != 2) {
        std::cout << "[Error] No config-file" << std::endl;
        return 0;
    }

    util::config_file config;
    std::string configFileName(argv[1]);

    config.load(configFileName);

    if (!checkConfigFile(config)) {
        return 0;
    }

    //______________________________________________________________________________________________________
    database::mysql::connection::user_data_t user;
    database::mysql::connection::server_adress_t server;

    user.username = config.getString("dbUsername");
    user.password = config.getString("dbPassword");

    server.host = config.getString("dbHost");
    server.dbName = config.getString("dbName");

    database::mysql::connection connection;

    if (!connection.open(user, server)) {
        std::cout << "[Error] " << connection.getErrorMsg() << std::endl;
        return 0;
    }

    //______________________________________________________________________________________________________
    encryption::private_key privateKey;
    encryption::private_key* privateKeyLink = nullptr;

    std::string privateKeyFile = config.getString("privateKey");

    if (privateKeyFile != "") {
        if (!privateKey.load(privateKeyFile)) {
            std::cout << "[Error] Could not load '" + privateKeyFile + "' as private key!" << std::endl;
            return 0;
        }

        privateKeyLink = &privateKey;
    }

    //______________________________________________________________________________________________________
    message::msg_controller<data::context> controller(config.getNumeric<int>("bufferSize"));
    data::context context;

    if (!context.init(connection, config.getNumeric<int32_t>("serverID"))) {
        std::cout << "[Error] Could not init context!" << std::endl;
        std::cout << "[Error] " << connection.getErrorMsg() << std::endl;
        return 0;
    }

    //______________________________________________________________________________________________________
    controller.registerHandler<message::msg_info>();
    controller.registerHandler<message::msg_login>();
    controller.registerHandler<message::msg_logout>();
    controller.registerHandler<message::msg_lookup>();

    controller.init(config.getNumeric<uint16_t>("port"), &context);
    //______________________________________________________________________________________________________
    signal(SIGINT, &stopServer);
    GlobalControllerLink = &controller;
    controller.recv(privateKeyLink);
    std::cout << "[Info] Closing server..." << std::endl;
    return 0;
}
