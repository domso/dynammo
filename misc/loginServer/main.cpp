#include <iostream>
#include "signal.h"
#include "src/message/msg_controller.h"
#include "src/message/msg_auth.h"
#include "src/encryption/rsa.h"
#include "src/database/connection.h"
#include "src/util/config_file.h"
#include "src/controller/login_server_context.h"

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

message::msg_controller<controller::login_server_context>* GlobalControllerLink = nullptr;

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
    message::msg_controller<controller::login_server_context> networkManager(config.getNumeric<int>("bufferSize"));
    networkManager.registerHandler<message::msg_auth>();
    
    controller::login_server_context context;

    if (!context.init(connection)) {
        std::cout << "[Error] Could not init context!" << std::endl;
        std::cout << "[Error] " << connection.getErrorMsg() << std::endl;
        return 0;
    }

    networkManager.init(config.getNumeric<uint16_t>("port"), &context);
    //______________________________________________________________________________________________________
    signal(SIGINT, &stopServer);
    GlobalControllerLink = &networkManager;
    networkManager.recv(nullptr);
    std::cout << "[Info] Closing server..." << std::endl;
    return 0;
}

