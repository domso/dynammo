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
            //accept_thread::options_t acceptOptions;
            send_thread::context_arg_t sendOptions;

            //   acceptOptions.port = 9585; //TODO
            //   acceptOptions.backlog = 10;
            //   acceptOptions.updateIntervall = 10;

            //acceptOptions.mutex = &m_connectionMutex;
            // acceptOptions.connectionQueue = &m_connectionQueue;
            //  acceptOptions.sender = &m_sender;

            sendOptions.mutex = &m_connectionMutex;
            sendOptions.connectionQueue = &m_connectionQueue;

            //  return accepter.start(acceptOptions) && m_sender.executor.start(sendOptions);
        }

        void controller::close() {
            //    m_accepter.stop();
            m_sender.executor.stop();
        }

    }
}
