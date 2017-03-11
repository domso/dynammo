#ifndef loginClient_data_context_h
#define loginClient_data_context_h

#include <unordered_map>
#include <queue>
#include "include/authentication/types.h"
#include "include/encryption/rsa.h"
#include "include/util/wait_lock.h"

namespace data {
    class context {
    public:

        bool init();
        encryption::public_key& getKey();

        void getServerInfo(authentication::server_info_t& info);
        void setServerInfo(const authentication::server_info_t& info);
        bool waitForServerInfo(authentication::server_info_t& info, double timeOut);


        void getCredentials(authentication::credentials_t& credentials);
        void setCredentials(authentication::credentials_t& credentials);

        void getIdentification(authentication::identification_t& identification);
        void setIdentification(const authentication::identification_t& identification);

        bool waitForTicket(authentication::ticket_t& ticket, double timeOut);

        void setTicket(const authentication::ticket_t& ticket);
        void clearTicket();

        void pushTicket(const authentication::ticket_t& ticket);
        bool popTicket(authentication::ticket_t& ticket);

        void setValidatedID(const authentication::accountID_t accountID);
        bool waitForValidatedID(authentication::accountID_t& accountID, double timeOut);


    private:

        encryption::public_key key_;

        util::wait_lock::context_t<authentication::server_info_t> serverInfo_;

        util::wait_lock::context_t<authentication::identification_t> identification_;

        util::wait_lock::context_t<authentication::ticket_t> ticket_;

        util::wait_lock::context_t<std::queue<authentication::ticket_t>> ticketQueue_;

        util::wait_lock::context_t<std::queue<authentication::accountID_t>> validAccountIDsQueue_;

    };
}

#endif



