#ifndef general_msg_controller_h
#define general_msg_controller_h

#include <functional>
#include <type_traits>

#include "assert.h"
#include "network/udp_socket.h"
#include "src/message/msg_header.h"
#include "src/message/msg_types.h"

namespace message {
    //______________________________________________________________________________________________________
    //
    // sender and receiver for udp-packages
    //______________________________________________________________________________________________________

    class msg_controller {
    public:
        //______________________________________________________________________________________________________
        //
        // Description:
        // - does not fully initialize the controller (see init())
        // - initializes 4 internal buffer with bufferSize
        // Parameter:
        // - bufferSize: size of the internal buffers
        //______________________________________________________________________________________________________
        msg_controller(const int bufferSize);
        //______________________________________________________________________________________________________
        //
        // Description:
        // - tries to initialize the socket
        // Parameters:
        // - port: the udp-socket-port (0 for unspecific)
        // Return:
        // - true  | on success
        // - false | on any error
        //______________________________________________________________________________________________________
        bool init(const uint16_t port);
        //______________________________________________________________________________________________________
        //
        // Description:
        // - closes the internal sockets and shutdown all recv()-calls
        //______________________________________________________________________________________________________
        void close();
        //______________________________________________________________________________________________________
        //
        // Description:
        // - callable-operator for msg_controller::recv()
        //______________________________________________________________________________________________________
        void operator()();
        //______________________________________________________________________________________________________
        //
        // Description:
        // - starts to receive on the internal socket for incoming udp packages
        // - the message is required in the format: <signature(optional)|header|data>
        // - the header specifies the requestHandler with the message type (see msg_header_t and registerHandler())
        // - to stop receiving either an internal error needs to occure, or close() was called
        //______________________________________________________________________________________________________
        void recv();
        //______________________________________________________________________________________________________
        // Description:
        // - tries to execute a request (static function) specified by the template type T
        // - T needs to implement a static function bool T::request(message::msg_header_t&, network::ipv4_addr&, network::pkt_buffer&, network::udp_socket<network::ipv4_addr>&,instant_argT, additional_datatype_t*)
        // - T::request() should push the request-payload into the pkt_buffer
        // - if T::request() returns true, the message will be send to destination address specified by destAddr
        // Parameters:
        // - destAddr: destination address of the request
        // - outputBuffer: buffer for the message
        // Return:
        // - true  | on success
        // - false | on any error
        //______________________________________________________________________________________________________
        template <typename T, typename instant_argT = void*>
        bool exec_request(network::ipv4_addr& destAddr, network::pkt_buffer& outputBuffer, instant_argT arg = instant_argT()) {
            static_assert((sizeof(T::id) == 1));
            message::msg_header_t* outputHeader = outputBuffer.push_next<message::msg_header_t>();


            if (outputHeader != nullptr && !m_networkSocket.is_closed()) {
                outputHeader->msgType = T::id;
                outputHeader->status = message::status::ok;
                outputHeader->attr = 0;

                // T::request exspects an actual type as parameter, internal we store only void*
                // Type-Safety is guaranteed by the register-call
                auto castedRequest = (bool (*)(message::msg_header_t&, network::ipv4_addr&, network::pkt_buffer&, network::udp_socket<network::ipv4_addr>&, instant_argT, void*)) T::request;

                if (castedRequest(*outputHeader, destAddr, outputBuffer, m_networkSocket, arg, m_additional_data[T::id])) {
                    return internal_send(m_networkSocket, destAddr, outputBuffer);
                }
            }

            return false;
        }
        //______________________________________________________________________________________________________
        //
        // Description:
        // - tries to register a new handler for a message-type specified by T::id
        // - only the last 7 Bits of T::id are used as an identifier
        // - the first bit used as a internal flag bit to seperate requests and response (the bit will be ignored if falsely set)
        // - T needs to implement a static function message::msg_status_t requestHandler(message::msg_header_t&, network::ipv4_addr&, network::pkt_buffer&, network::pkt_buffer&, network::udp_socket<network::ipv4_addr>&, message::msg_option_t&, instant_argT, additional_datatype_t*);
        // - T needs to implement a static function message::msg_status_t responseHandler(message::msg_header_t&, network::ipv4_addr&, network::pkt_buffer&, network::pkt_buffer&, network::udp_socket<network::ipv4_addr>&, message::msg_option_t&, instant_argT, additional_datatype_t*);
        // - both T::requestHandler() and T::responseHandler() should read the message-payload from the first pkt_buffer, push any new payload into the second pkt_buffer and return with a message-status (see msg_header.h)
        // - any access to the pkt_buffers outside of T::requestHandler() and T::responseHandler() leads to undefined behaviour!
        // Return:
        // - true  | on success
        // - false | T::id is already used
        //______________________________________________________________________________________________________
        template <typename T, typename instant_argT = void*, typename additional_datatype_t = typename T::content>
        bool register_handler(additional_datatype_t* context)  {
            static_assert(sizeof(T::id) == 1, "");
            static_assert(check<bool (*)(message::msg_header_t&, network::ipv4_addr&, network::pkt_buffer&, network::udp_socket<network::ipv4_addr>&, instant_argT, additional_datatype_t*)>(T::request));
            static_assert(check<message::msg_status_t (*)(message::msg_header_t&, network::ipv4_addr&, network::pkt_buffer&, network::pkt_buffer&, network::udp_socket<network::ipv4_addr>&, message::msg_option_t&, additional_datatype_t*)>(T::requestHandler));
            static_assert(check<message::msg_status_t (*)(message::msg_header_t&, network::ipv4_addr&, network::pkt_buffer&, network::pkt_buffer&, network::udp_socket<network::ipv4_addr>&, message::msg_option_t&, additional_datatype_t*)>(T::responseHandler));

            if (m_callbacks[T::id & 127] != nullptr || m_callbacks[(int)(T::id & 127) + 128] != nullptr) {
                return false;
            }

            m_callbacks[T::id & 127] = (message::msg_status_t (*)(message::msg_header_t&, network::ipv4_addr&, network::pkt_buffer&, network::pkt_buffer&, network::udp_socket<network::ipv4_addr>&, message::msg_option_t&, void*)) &T::requestHandler;
            m_callbacks[(int)(T::id & 127) + 128] = (message::msg_status_t (*)(message::msg_header_t&, network::ipv4_addr&, network::pkt_buffer&, network::pkt_buffer&, network::udp_socket<network::ipv4_addr>&, message::msg_option_t&, void*)) &T::responseHandler;


            m_additional_data[T::id & 127] = (void*) context;
            m_additional_data[(int)(T::id & 127) + 128] = (void*) context;

            return true;
        }
        //______________________________________________________________________________________________________
        //
        // Return:
        // - Registered additional_datatype for T
        //______________________________________________________________________________________________________
        template <typename T, typename additional_datatype_t = typename T::content>
        additional_datatype_t* additional_datatype() {
            return (additional_datatype_t*) m_additional_data[T::id & 127];
        }
        //______________________________________________________________________________________________________
        //
        // Return:
        // - Port of underlying UDP-socket
        //______________________________________________________________________________________________________
        uint16_t port() const;
    private:

        
        template <typename T, typename P>
        static constexpr bool check(const P) {
            return std::is_same<T, P>::value;
        }
        //______________________________________________________________________________________________________
        //
        // dont use it outside of recv()
        //______________________________________________________________________________________________________
        bool internal_recv();
        //______________________________________________________________________________________________________
        //
        // dont use it outside of recv() or execRequest()
        //______________________________________________________________________________________________________
        static bool internal_send(network::udp_socket<network::ipv4_addr>& networkSocket, network::ipv4_addr& dest, network::pkt_buffer& outputBuffer, const int flags = 0);

        std::function<message::msg_status_t(message::msg_header_t& header, network::ipv4_addr&, network::pkt_buffer&, network::pkt_buffer&, network::udp_socket<network::ipv4_addr>&, message::msg_option_t&, void*)> m_callbacks[256];
        void* m_additional_data[256];

        network::udp_socket<network::ipv4_addr> m_networkSocket;
        network::pkt_buffer m_inputBuffer;
        network::pkt_buffer m_outputBuffer;

        network::ipv4_addr m_srcAddr;
    };
}

#endif


