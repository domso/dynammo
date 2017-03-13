#include "include/database/connection.h"

namespace database {
    namespace mysql {
        
        connection::connection() : connection_(nullptr) {

        }
        
        connection::~connection() {
            if (connection_ != nullptr) {
                mysql_close(connection_);
            }
        }
        
        bool connection::open(const user_data_t& user, const server_adress_t& server) {
            // there might be an open connection
            if (connection_ != nullptr) {
                return false;
            }

            mysql_init(&databaseLink_);

            // mysql_real_connect exspects nullptr for an disabled parameter
            const char* socket = nullptr;

            if (server.socketName != "") {
                socket = server.socketName.c_str();
            }

            connection_ = mysql_real_connect(&databaseLink_, server.host.c_str(), user.username.c_str(), user.password.c_str(), server.dbName.c_str(), server.port, socket , server.flags);

            return connection_ != nullptr;

        }
        
        void connection::close() {
            mysql_close(connection_);
            connection_ = nullptr;
        }
        
        MYSQL* connection::getNativeHandler() {
            return connection_;
        }
        
        std::string connection::getErrorMsg()  {
            std::string result(mysql_error(&databaseLink_));
            return result;
        }
        
    }
}


