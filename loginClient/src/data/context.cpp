#include <cstring>
#include "include/authentication/types.h"
#include "include/encryption/randomize.h"
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

    void context::getServerID(authentication::serverID_t& id) {
        util::wait_lock::read<authentication::serverID_t> lock(destinationServerID_);
        id = lock.data();
    }

    void context::setServerID(const authentication::serverID_t& id) {
        util::wait_lock::write<authentication::serverID_t> lock(destinationServerID_);
        lock.data() = id;
    }

    bool context::waitForServerID(authentication::serverID_t& id, double timeOut) {
        util::wait_lock::wait<authentication::serverID_t> lock(destinationServerID_, timeOut);

        if (!lock.isValid()) {
            return false;
        }

        id = lock.data();
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
        util::wait_lock::write<std::queue<authentication::ticket_t>> lock(ticketInputQueue_);
        lock.data().push(ticket);
    }

    bool context::popTicket(authentication::ticket_t& ticket) {
        util::wait_lock::write<std::queue<authentication::ticket_t>> lock(ticketInputQueue_);

        if (lock.data().size() > 0) {
            ticket = lock.data().front();
            lock.data().pop();
            return true;
        }

        return false;
    }

    void context::setValidatedTicket(const authentication::ticket_t& ticket) {
        util::wait_lock::write<std::queue<authentication::session_t>> lock(sessionOutputQueue_);
        if (ticket.serverID == localServerID_) {
            authentication::session_t session;
            session.accountID = ticket.accountID;
            encryption::randomize<authentication::sessionID_t>(&session.sessionID);
            lock->push(session);    
        }
        
    }

    bool context::waitForValidatedSession(authentication::session_t& session, double timeOut) {
        util::wait_lock::wait<std::queue<authentication::session_t>> lock(sessionOutputQueue_, timeOut);

        if (!lock.isValid() || lock.data().size() == 0) {
            return false;
        }

        session = lock.data().front();
        lock.data().pop();
        return session.accountID != 0;
    }
}
