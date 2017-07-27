#include "src/data/map/controller.h"
#include "network/tcp_socket.h"
#include "network/tcp_connection.h"

#include "stdint.h"

namespace data {
    namespace map {
        controller::controller() : m_mapOpen(false) {

        }

        controller::~controller() {

        }

        bool controller::open() {
            accept_thread::options_t acceptOptions;
            send_thread::m_contextarg_t sendOptions;
            
            acceptOptions.port = 9585; //TODO
            acceptOptions.backlog = 10;
            acceptOptions.updateIntervall = 10;
            
            acceptOptions.mutex = &connectionMutex;
            acceptOptions.connectionQueue = &connectionQueue;
            acceptOptions.sender = &sender;
            
            sendOptions.mutex = &connectionMutex;
            sendOptions.connectionQueue = &connectionQueue;
            
            return accepter.start(acceptOptions) && sender.executor.start(sendOptions);
        }

        void controller::close() {
            accepter.stop();
            sender.executor.stop();
        }

    }
}
