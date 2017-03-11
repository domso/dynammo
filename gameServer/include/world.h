

#include <vector>
#include <queue>
#include <atomic>


#include "network/tcp_socket.h"
#include "network/tcp_connection.h"

struct user_data_t {
    int key;
    int data;
    network::tcp_connection<network::ipv4_addr> connection;
};

class world_tile_t {
public:
    char data;
};

class world_chunk_t {
public:
    world_tile_t tiles[64];
};

class world_map_t {
public:
    world_chunk_t chunks[64];

    void update() {

    }
};

class world_map_handle_t {
public:
    int id;
    std::atomic<char> seperatorState;
    world_map_t* map;

    std::vector<user_data_t*> user;
    std::queue<user_data_t*> newUserQueue;


    void update() {

    }

    void enqueueNewUser(user_data_t* newUser) {
        newUserQueue.push(newUser);
    }


    bool registerNewUser() {
        if (!newUserQueue.empty()) {
            user.push_back(newUserQueue.front());
            newUserQueue.pop();
            return true;
        }

        return false;
    }
};

class world_group_t {
public:
    std::vector<world_map_handle_t> maps;


    void update(int startIndex, int endIndex) {


        for (int i = startIndex; i <= endIndex; i++) {
            maps[i].update();
        }

    }
};
