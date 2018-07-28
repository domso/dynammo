#pragma once

#include <vector>
#include "src/connector/data_transfer/obj_link.h"
#include "src/connector/data_transfer/tcp_receiver.h"
#include "src/connector/context.h"

namespace connector {
    namespace data_transfer {
        class controller {
        public:            
            controller(connector::context& context);
            controller(const controller& copy) = delete;
            controller(controller&& move) = delete;
            
            void update();
            void close();
        private:
            obj_link<types::data_transfer::content::session_id> sessionIDLink;
            obj_link<types::data_transfer::content::region_select> regionSelectLink;
            obj_link<types::data_transfer::content::region_layer> layerLink;
            obj_link<types::data_transfer::content::static_object> staticObjLink;
            obj_link<types::data_transfer::content::dynamic_object> dynObjLink;

            template <typename T>
            static void complete_T(std::vector<T>& objs, connector::context* context) {
                
            }
            
            template <typename T>
            void register_link(obj_link<T>& link) {
                link.init(&complete_T<typename T::content>, &m_context);
                m_tcpRecv.register_callbacks(link);
            }
            
            connector::context& m_context;
            static constexpr const auto buffersize = 1500;
            connector::data_transfer::tcp_receiver m_tcpRecv;
        };
    }
}
