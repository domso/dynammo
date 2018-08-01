#pragma once

#include <vector>
#include <type_traits>

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

            template <typename T, typename V>
            using enable_if_same = typename std::enable_if<std::is_same<T, V>::value, bool>::type;                       
            
            template <typename T>
            static enable_if_same<T, uint32_t> complete_T(std::vector<T>& objs, connector::context* context) {
                context->config.global().set("sessionID", objs[0]);
                return true;
            }
            
            template <typename T>
            static enable_if_same<T, region::layer<uint32_t>> complete_T(std::vector<T>& objs, connector::context* context) {
                context->gameCtrl.add_game_object(0, objs);
                return true;
            }
            
            template <typename T>
            static enable_if_same<T, region::dynamic_obj> complete_T(std::vector<T>& objs, connector::context* context) {
                context->gameCtrl.add_game_object(0, objs);
                return true;
            }
            
            template <typename T>
            static enable_if_same<T, region::static_obj> complete_T(std::vector<T>& objs, connector::context* context) {
                context->gameCtrl.add_game_object(0, objs);
                return true;
            }           
            
            
            template <typename T>
            void register_link(obj_link<T>& link) {
                link.init(&complete_T<typename T::content>, &m_context);
                m_tcpRecv.register_callbacks(&link);
            }
            
            connector::context& m_context;
            static constexpr const auto buffersize = 1500;
            connector::data_transfer::tcp_receiver m_tcpRecv;
        };
    }
}
