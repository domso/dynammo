
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
        m_isRunning.store(true);
        m_loginSocket.acceptOn(port, 10);
        m_loginSocket.setTimeout(0.1);
    }

    ~user_container() {
        m_isRunning.store(false);
    }

    void start(const world_group_t* map) {
        m_acceptThread = std::thread(user_container_accept_thread_exec, this, map);
    }

    void stop() {
        if (m_isRunning.load()) {
            m_isRunning.store(false);
            m_acceptThread.join();
        }
    }

    void accept(const world_group_t& map) const {

        user_data_t* user = new user_data_t();

        while (m_isRunning.load()) {
            if (m_loginSocket.acceptConnection(user->connection)) {
                user->key = m_IDs.next();
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
    std::atomic<bool> m_isRunning;
    std::thread m_acceptThread;
    char cachePad[64];

    unique_m_keygenerator<int> m_IDs;
    network::tcp_socket<network::ipv4_addr> m_loginSocket;
};
