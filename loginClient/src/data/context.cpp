#include <cstring>
#include "include/authentication/types.h"
#include "include/util/timestamp.h"
#include "include/util/mem.h"
#include "include/data/context.h"

namespace data {

    encryption::public_key& context::getKey() {
        return key_;
    }

    void context::getServerInfo(authentication::server_info_t& info) {
        util::wait_lock::read<authentication::server_info_t> lock(serverInfo_);
        info = lock.data();
    }

    void context::setServerInfo(const authentication::server_info_t& info) {
        util::wait_lock::write<authentication::server_info_t> lock(serverInfo_);
        lock.data() = info;
    }

    bool context::waitForServerInfo(authentication::server_info_t& info, double timeOut) {
        util::wait_lock::wait<authentication::server_info_t> lock(serverInfo_, timeOut);

        if (!lock.isValid()) {
            return false;
        }

        info = lock.data();
        return true;
    }

    void context::getCredentials(authentication::credentials_t& credentials) {
        util::wait_lock::read<authentication::identification_t> lock(identification_);
        credentials = lock.data().credentials;
    }

    void context::setCredentials(const authentication::credentials_t& credentials) {
        util::wait_lock::write<authentication::identification_t> lock(identification_);
        lock.data().credentials = credentials;
    }

    void context::getIdentification(authentication::identification_t& identification) {
        util::wait_lock::read<authentication::identification_t> lock(identification_);
        identification = lock.data();
    }

    void context::setIdentification(const authentication::identification_t& identification) {
        util::wait_lock::write<authentication::identification_t> lock(identification_);
        lock.data() = identification;
    }

    void context::setTicket(const authentication::ticket_t& ticket) {
        util::wait_lock::write<authentication::ticket_t> lockTicket(ticket_);
        util::wait_lock::write<authentication::identification_t> lockIdentification(identification_);
        lockTicket.data() = ticket;
        lockIdentification->accountID = ticket.accountID;
    }

    bool context::waitForTicket(authentication::ticket_t& ticket, double timeOut) {
        util::wait_lock::wait<authentication::ticket_t> lock(ticket_, timeOut);

        if (!lock.isValid()) {
            return false;
        }

        ticket = lock.data();
        return ticket.accountID != 0;
    }

    void context::clearTicket() {
        util::wait_lock::write<authentication::ticket_t> lockTicket(ticket_);
        util::wait_lock::write<authentication::identification_t> lockIdentification(identification_);
        util::mem::set<authentication::ticket_t>(&lockTicket.data(), 0);
        util::mem::set<authentication::identification_t>(&lockIdentification.data(), 0);
    }

    bool context::waitForClearedTicket(double timeOut) {
        util::wait_lock::wait<authentication::ticket_t> lock(ticket_, timeOut);

        if (!lock.isValid()) {
            return false;
        }

        return lock.data().accountID == 0;
    }

    void context::pushTicket(const authentication::ticket_t& ticket) {
        util::wait_lock::write<std::queue<authentication::ticket_t>> lock(ticketQueue_);
        lock.data().push(ticket);
    }

    bool context::popTicket(authentication::ticket_t& ticket) {
        util::wait_lock::write<std::queue<authentication::ticket_t>> lock(ticketQueue_);

        if (lock.data().size() > 0) {
            ticket = lock.data().front();
            lock.data().pop();
            return true;
        }

        return false;
    }

    void context::setValidatedID(const authentication::accountID_t accountID) {
        util::wait_lock::write<std::queue<authentication::accountID_t>> lock(validAccountIDsQueue_);
        lock->push(accountID);
    }

    bool context::waitForValidatedID(authentication::accountID_t& accountID, double timeOut) {
        util::wait_lock::wait<std::queue<authentication::accountID_t>> lock(validAccountIDsQueue_, timeOut);

        if (!lock.isValid() || lock.data().size() == 0) {
            return false;
        }

        accountID = lock.data().front();
        lock.data().pop();
        return accountID != 0;
    }
}
