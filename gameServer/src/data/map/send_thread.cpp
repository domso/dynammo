#include "src/data/map/send_thread.h"

namespace data {
    namespace map {
        send_thread::send_thread() {
            executor.registerTask<addConnectionTask>();
            executor.registerTask<sendUpdateTask>();
        }

        send_thread::context_t::context_t(const context_arg_t& arg) : mutex(*arg.mutex), connectionQueue(*arg.connectionQueue) {

        }

        void send_thread::addConnectionTask::execute(context_t& context) {
            std::unique_lock<std::mutex> ul(context.mutex);
            
            if (!context.connectionQueue.empty()) {
                auto connection = context.connectionQueue.front();
                
                ul.unlock();
                //TODO send all data to connection
                context.connections.insert(connection);
                
                //connection->closeSocket();
            }
        }

        void send_thread::sendUpdateTask::execute(context_t& context) {

        }
    }
}
