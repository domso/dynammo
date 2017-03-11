
#include <atomic>
#include <unordered_map>
#include <thread>

#include "network/tcp_socket.h"
#include "network/tcp_connection.h"
#include "util.h"

#include "world.h"


void user_container_accept_thread_exec(user_container* container, world_group_t* map) {
    container->accept(*map);
}

template<typename T>
class user_container {
public:

    user_container(const int port) {
        isRunning_.store(true);
        loginSocket_.acceptOn(port, 10);
        loginSocket_.setTimeout(0.1);
    }

    ~user_container() {
        isRunning_.store(false);
    }

    void start(const world_group_t* map) {
        acceptThread_ = std::thread(user_container_accept_thread_exec, this, map);
    }

    void stop() {
        if (isRunning_.load()) {
            isRunning_.store(false);
            acceptThread_.join();
        }
    }

    void accept(const world_group_t& map) const {

        user_data_t* user = new user_data_t();

        while (isRunning_.load()) {
            if (loginSocket_.acceptConnection(user->connection)) {
                user->key = IDs_.next();
                user->data
                // TODO
                map[0].enqueueNewUser(user);
                user = new user_data_t();
            }
        }

        delete user;
    }

private:

    char cachePad[64];
    std::atomic<bool> isRunning_;
    std::thread acceptThread_;
    char cachePad[64];

    unique_key_generator<int> IDs_;
    network::tcp_socket<network::ipv4_addr> loginSocket_;
};
