#include "src/connector/controller.h"
#include "src/connector/msg_transfer/auth.h"
#include "src/connector/msg_transfer/create_account.h"
#include "src/connector/msg_transfer/region_action.h"
#include "src/connector/msg_transfer/region_chat.h"

connector::controller::controller(region::controller& rCtrl, session::controller& sCtrl) :
    m_context(rCtrl, sCtrl),
    m_msgCtrl(1500),
    inputCtrl(m_context, m_msgCtrl),
    outputCtrl(m_context, m_msgCtrl) {
    m_msgCtrl.init(1851);
    m_msgCtrl.register_handler<msg_transfer::auth>(&m_context);
    m_msgCtrl.register_handler<msg_transfer::create_account>(&m_context);
    m_msgCtrl.register_handler<msg_transfer::region_action, std::pair<uint32_t, std::vector<region::dynamic_obj>&>*>(&m_context);
    m_msgCtrl.register_handler<msg_transfer::region_chat>(&m_context);
}

connector::controller::~controller() {

}

connector::controller::input::input(connector::context& context, message::msg_controller& msgCtrl): m_context(context), m_msgCtrl(msgCtrl) {

}

connector::controller::input::~input() {
    close();
}

void connector::controller::input::update() {
    m_msgCtrl.recv();
}

void connector::controller::input::close() {
    m_msgCtrl.close();
}

connector::controller::output::output(connector::context& context, message::msg_controller& msgCtrl) : m_context(context), m_msgCtrl(msgCtrl), m_buffer(1024) {
    m_running = true;
}

connector::controller::output::~output() {
    close();
}

void connector::controller::output::update() {
    while (m_running) {
        auto region = m_context.regionCtrl.get_region_with_update();

        if (region) {
            int numObjs = (*region)->changed_dynamic_objects().size();
            uint32_t regionID = (*region)->get_id();
            std::vector<std::vector<region::dynamic_obj>> outputObjs(1);
            size_t currentVector = 0;

            for (int i = 0; i < numObjs; i++) {
                outputObjs[currentVector].push_back(
                    (*region)->all_dynamic_objects().at(
                        (*region)->changed_dynamic_objects()[i]
                    )
                );
                
                if (outputObjs[currentVector].size() == 10) {
                    outputObjs.resize(outputObjs.size() + 1);
                    currentVector++;
                }
            }
            
            for (auto& vObjs : outputObjs) {
                std::pair<uint32_t, std::vector<region::dynamic_obj>&> execArg(regionID, vObjs);
                for (uint32_t userID : (*region)->affected_users()) {
                    auto user = m_context.sessionCtrl.get_user(userID);

                    if (user) {
                        m_buffer.clear();
                        m_msgCtrl.exec_request <
                            msg_transfer::region_action,
                            std::pair<uint32_t, std::vector<region::dynamic_obj>&>*
                        > (
                            (*user)->get_addr(),
                            m_buffer,
                            &execArg
                        );
                    }
                }
            }
            
            (*region)->commit();
        }
    }
}

void connector::controller::output::close() {
    m_running = false;
}


