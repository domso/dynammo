#pragma once

#include <stdint.h>
#include <vector>
#include <functional>
#include "src/util/state_machine.h"
#include "src/types/data_transfer/content.h"

namespace connector {
    namespace data_transfer {
        template <typename T, typename callbackParamT = void>
        class obj_link {
        public:
            constexpr static const auto id = T::id;

            obj_link(const std::function<void(obj_link<T, callbackParamT>&, callbackParamT*)> callback, callbackParamT* param) : m_callback(callback), m_additionArg(param) {

            }

            void reset() {
                m_currentState = states::recvCount;
            }
            
            static connector::tcp_receiver::target configure(obj_link<T, callbackParamT>* obj) {
                connector::tcp_receiver::target result;

                switch (obj->m_currentState.get()) {
                case states::recvCount:
                    result = configure_count(obj);
                    break;

                case states::recvData:
                    result = configure_data(obj);
                    break;
                }
                
                return result;
            }

            static bool complete(obj_link<T, callbackParamT>* obj) {
                bool result = true;

                switch (obj->m_currentState.get()) {
                case states::recvCount:
                    result =  complete_count(obj);
                    break;

                case states::recvData:
                    result =  complete_data(obj);
                    break;
                }

                return result;
            }

//             bool wait_for_completion(const double timeout = 10) {
//                 return m_currentState.wait_for(states::recvComplete, timeout);
//             }

            std::vector<typename T::content> data;
        private:
            static connector::tcp_receiver::target configure_count(obj_link<T, callbackParamT>* obj) {
                connector::tcp_receiver::target result;
                result.set<uint16_t>(&obj->m_count);
                return result;
            }

            static bool complete_count(obj_link<T, callbackParamT>* obj) {
                obj->m_currentState.set(states::recvData);
                obj->data.resize(obj->m_count);
                return false;
            }

            static connector::tcp_receiver::target configure_data(obj_link<T, callbackParamT>* obj) {
                connector::tcp_receiver::target result;
                result.set<typename T::content>(obj->data.data(), obj->data.size());
                return result;
            }

            static bool complete_data(obj_link<T, callbackParamT>* obj) {
                if (obj->m_callback != nullptr) {
                    obj->m_callback(*obj, obj->m_additionArg);
                }

                obj->m_currentState.set(states::recvCount);
                return true;
            }

            enum states {
                recvCount,
                recvData,
                recvComplete
            };

            util::state_machine<states> m_currentState = states::recvCount;
            uint16_t m_count;
            std::function<void(obj_link<T, callbackParamT>&, callbackParamT*)> m_callback;
            callbackParamT* m_additionArg;
        };
    }
}
