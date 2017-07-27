#ifndef general_msg_controller_h
#define general_msg_controller_h

#include "assert.h"
#include "network/udp_socket.h"
#include "src/authentication/types.h"
#include "src/message/msg_header.h"
#include "src/message/msg_types.h"
#include "src/util/timestamp.h"
#include "src/util/mem.h"
#include "src/encryption/rsa.h"

#include <iostream>
namespace message {
    //______________________________________________________________________________________________________
    //
    // sender and receiver for udp-packages
    //______________________________________________________________________________________________________
    template<typename additional_datatype_t>
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
        msg_controller(const int bufferSize, const int reorderWindow = 2) : m_currentBuffer(0),
            m_encryptedInputBuffer(bufferSize), m_decryptedInputBuffers(reorderWindow, bufferSize), m_encryptedOutputBuffer(bufferSize), m_uncryptedOutputBuffer(bufferSize)  {
                               
            for (int i = 0; i < 256; i++) {
                callbacks[i] = nullptr;
            }
        }
        //______________________________________________________________________________________________________
        //
        // Description:
        // - tries to initialize the socket and the additional_data argument
        // Parameters:
        // - port: the udp-socket-port
        // - additional_data: valid pointer to the argument (see registerHandler())
        // Return:
        // - true  | on success
        // - false | on any error
        //______________________________________________________________________________________________________
        bool init(const uint16_t port, additional_datatype_t* additional_data) {
            this->m_additional_data = additional_data;
            return (additional_data != nullptr && m_networkSocket.init(port));
        }
        //______________________________________________________________________________________________________
        //
        // Description:
        // - closes the internal sockets and shutdown all recv()-calls
        //______________________________________________________________________________________________________
        void close() {
            m_networkSocket.shutRDWR();
        }
        //______________________________________________________________________________________________________
        //
        // Description:
        // - callable-operator for msg_controller::recv()
        //______________________________________________________________________________________________________
        void operator()(const encryption::private_key* privateKey = nullptr, encryption::public_key* publicKey = nullptr) {
            recv(privateKey, publicKey);
        }
        //______________________________________________________________________________________________________
        //
        // Description:
        // - starts to receive on the internal socket for incoming udp packages
        // - every package will be decrypted with the optional private-key
        // - the message is required in the format: <signature(optional)|header|data>
        // - the header specifies the requestHandler with the message type (see msg_header_t and registerHandler())
        // - the output may be encrypted with the optional public-key and signed with optional private-key
        // - to stop receiving either an internal error needs to occure, or close() was called
        // Parameters:
        // - privateKey: valid pointer to a loaded instance of a private-key (optional)
        // - publicKey: valid pointer to a loaded instance of a public-key (optional)
        //______________________________________________________________________________________________________
        void recv(const encryption::private_key* privateKey = nullptr, encryption::public_key* publicKey = nullptr) {
            static_assert(sizeof(msg_header_t::msgType) == 1, "bad header-type");
            assert(m_additional_data != nullptr);

            msg_header_t* header;

            while (internalRecv(privateKey, publicKey)) {
                network::pkt_buffer& decryptedInputBuffer = m_decryptedInputBuffers[m_currentBuffer];
                header = decryptedInputBuffer.getNext<msg_header_t>();

                if (header != nullptr) {
                    if (callbacks[header->msgType] != nullptr) {

                        if (privateKey != nullptr) {
                            m_uncryptedOutputBuffer.reserveLen(privateKey->getRequiredSize());
                        } else {
                            m_uncryptedOutputBuffer.setMsgLen(0);
                        }

                        message::msg_option_t option = MSG_OPTION_CLEAR;
                        message::msg_header_t* outputHeader = m_uncryptedOutputBuffer.pushNext<message::msg_header_t>();
                        message::msg_status_t status = callbacks[header->msgType](*header, m_srcAddr, decryptedInputBuffer, m_uncryptedOutputBuffer, m_networkSocket, option, *m_additional_data);

                        if (status == MSG_STATUS_WAIT) {
                            decryptedInputBuffer.reset();
                            m_currentBuffer++;                           
                            m_currentBuffer *= (m_currentBuffer < m_decryptedInputBuffers.size());
                        } else if (status != MSG_STATUS_CLOSE && outputHeader != nullptr) {
                            outputHeader->status = status;
                            outputHeader->msgType = header->msgType ^ (MSG_HEADER_TYPE_REQUEST_SWITCH_MASK * ((option & MSG_OPTION_NO_REQUEST_RESPONSE_SWITCH) == 0));
                            outputHeader->attr = header->attr;

                            if (privateKey == nullptr || (option & MSG_OPTION_NO_SIGNING) || internalSign(*privateKey, m_uncryptedOutputBuffer)) {
                                encryption::public_key* tmp = publicKey;

                                if (option & MSG_OPTION_NO_ENCRYPTION) {
                                    tmp = nullptr;
                                }

                                if (!internalSend(m_networkSocket, m_srcAddr,  m_uncryptedOutputBuffer, m_encryptedOutputBuffer, tmp)) {
                                    //TODO send/encryption error
                                }
                            } else {
                                //TODO signature error
                            }
                        }
                    }
                }
            }
        }
        //______________________________________________________________________________________________________
        // Description:
        // - tries to execute a request (static function) specified by the template type T
        // - the message will be encrypted with the optional public-key and signed with the optional private-key
        // - T needs to implement a static function bool T::request(message::msg_header_t&, network::ipv4_addr&, network::pkt_buffer&, network::udp_socket<network::ipv4_addr>&, additional_datatype_t)
        // - T::request() should push the request-payload into the pkt_buffer
        // - if T::request() returns true, the message will be send to destination address specified by destAddr
        // Parameters:
        // - destAddr: destination address of the request
        // - uncryptedOutputBuffer: buffer for the uncrypted message
        // - encryptedOutputBuffer: buffer for the encrypted message
        // - param: additional argument for T::request()
        // - privateKey: valid pointer to a loaded instance of a private-key (optional)
        // - publicKey: valid pointer to a loaded instance of a public-key (optional)
        // Return:
        // - true  | on success
        // - false | on any error
        //______________________________________________________________________________________________________
        template <typename T>
        bool execRequest(network::ipv4_addr& destAddr, network::pkt_buffer& uncryptedOutputBuffer, network::pkt_buffer& encryptedOutputBuffer, additional_datatype_t& param, const encryption::private_key* privateKey = nullptr, const encryption::public_key* publicKey = nullptr) {
            return staticExecRequest<T>(m_networkSocket, destAddr, uncryptedOutputBuffer, encryptedOutputBuffer, param, privateKey, publicKey);
        }
        //______________________________________________________________________________________________________
        //
        // Description:
        // - static version of msg_controller::execRequest()
        // additional Parameters:
        // - networkSocket: reference to valid and open udp socket
        //______________________________________________________________________________________________________
        template <typename T>
        static bool staticExecRequest(network::udp_socket<network::ipv4_addr>& networkSocket, network::ipv4_addr& destAddr, network::pkt_buffer& uncryptedOutputBuffer, network::pkt_buffer& encryptedOutputBuffer, additional_datatype_t& param, const encryption::private_key* privateKey = nullptr, const encryption::public_key* publicKey = nullptr) {
            if (privateKey != nullptr) {
                uncryptedOutputBuffer.reserveLen(privateKey->getRequiredSize());
            } else {
                uncryptedOutputBuffer.setMsgLen(0);
            }

            message::msg_header_t* outputHeader = uncryptedOutputBuffer.pushNext<message::msg_header_t>();

            if (outputHeader != nullptr) {
                outputHeader->msgType = T::id;
                outputHeader->status = MSG_STATUS_OK;
                outputHeader->attr = 0;

                if (T::request(*outputHeader, destAddr, uncryptedOutputBuffer, networkSocket, param)) {
                    if (privateKey == nullptr || internalSign(*privateKey, uncryptedOutputBuffer)) {
                        if (!internalSend(networkSocket, destAddr,  uncryptedOutputBuffer, encryptedOutputBuffer, publicKey)) {
                            //TODO send/encryption error
                            return false;
                        }

                        return true;
                    } else {
                        //TODO signature error
                        return false;
                    }
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
        // - T needs to implement a static function message::msg_status_t requestHandler(message::msg_header_t&, network::ipv4_addr&, network::pkt_buffer&, network::pkt_buffer&, network::udp_socket<network::ipv4_addr>&, message::msg_option_t&, additional_datatype_t);
        // - T needs to implement a static function message::msg_status_t responseHandler(message::msg_header_t&, network::ipv4_addr&, network::pkt_buffer&, network::pkt_buffer&, network::udp_socket<network::ipv4_addr>&, message::msg_option_t&, additional_datatype_t);
        // - both T::requestHandler() and T::responseHandler() should read the message-payload from the first pkt_buffer, push any new payload into the second pkt_buffer and return with a message-status (see msg_header.h)
        // - any access to the pkt_buffers outside of T::requestHandler() and T::responseHandler() leads to undefined behaviour!
        // Return:
        // - true  | on success
        // - false | T::id is already used
        //______________________________________________________________________________________________________
        template <typename T>
        bool registerHandler() {
            if (callbacks[T::id & 127] != nullptr || callbacks[(int)(T::id & 127) + 128] != nullptr) {
                return false;
            }

            callbacks[T::id & 127] = &T::requestHandler;
            callbacks[(int)(T::id & 127) + 128] = &T::responseHandler;

            return true;
        }
    private:
        //______________________________________________________________________________________________________
        //
        // dont use it outside of recv()
        //______________________________________________________________________________________________________
        bool internalRecv(const encryption::private_key* privateKey = nullptr, const encryption::public_key* publicKey = nullptr) {
            network::pkt_buffer& encryptedInputBuffer = m_encryptedInputBuffer;
            network::pkt_buffer& decryptedInputBuffer = m_decryptedInputBuffers[m_currentBuffer];

            if (decryptedInputBuffer.msgLen() != 0) {
                return true;
            }
            
            
            if (privateKey != nullptr) { // encryption
                if (m_networkSocket.recvPkt(m_srcAddr, encryptedInputBuffer) == -1) {
                    return false;
                }

                decryptedInputBuffer.setMsgLen(encryption::decryptChar(*privateKey, encryptedInputBuffer.msgLen(), (unsigned char*) encryptedInputBuffer.data(), decryptedInputBuffer.capacity(), (unsigned char*) decryptedInputBuffer.data()));
            } else { // no encryption
                if (m_networkSocket.recvPkt(m_srcAddr, decryptedInputBuffer) == -1) {
                    return false;
                }
            }

            if (publicKey != nullptr) { // validate signature
                int requiredSize = publicKey->getRequiredSize();
                char* signature = decryptedInputBuffer.getNext<char>(requiredSize);

                if (signature != nullptr) {
                    if (!encryption::verifyChar(*publicKey, requiredSize, (unsigned char*) signature, decryptedInputBuffer.remainingMsgLen(), (unsigned char*) decryptedInputBuffer.dataOffset())) {
                        // delete message
                        decryptedInputBuffer.setMsgLen(0);
                        return false;
                    }
                }

            }

            return true;
        }
        //______________________________________________________________________________________________________
        //
        // dont use it outside of recv() or execRequest()
        //______________________________________________________________________________________________________
        static bool internalSend(network::udp_socket<network::ipv4_addr>& networkSocket, network::ipv4_addr& dest, network::pkt_buffer& uncryptedOutputBuffer, network::pkt_buffer& encryptedOutputBuffer, const encryption::public_key* publicKey = nullptr, const int flags = 0) {
            if (publicKey != nullptr) {
                encryptedOutputBuffer.setMsgLen(encryption::encryptChar(*publicKey, uncryptedOutputBuffer.msgLen(), (unsigned char*) uncryptedOutputBuffer.data(), encryptedOutputBuffer.capacity(), (unsigned char*) encryptedOutputBuffer.data()));
                return networkSocket.sendPkt(dest, encryptedOutputBuffer, flags) == encryptedOutputBuffer.msgLen();
            } else {
                return networkSocket.sendPkt(dest, uncryptedOutputBuffer, flags) == uncryptedOutputBuffer.msgLen();
            }
        }
        //______________________________________________________________________________________________________
        //
        // dont use it outside of recv() or execRequest()
        //______________________________________________________________________________________________________
        static bool internalSign(const encryption::private_key& privateKey, network::pkt_buffer& buffer) {
            buffer.reset();
            int signatureLen = privateKey.getRequiredSize();
            unsigned char* sigature = buffer.getNext<unsigned char>(signatureLen);
            return encryption::signChar(privateKey, buffer.remainingMsgLen(), (unsigned char*) buffer.dataOffset(), signatureLen, sigature) != -1;
        }

        message::msg_status_t (*callbacks[256])(message::msg_header_t& header, network::ipv4_addr&, network::pkt_buffer&, network::pkt_buffer&, network::udp_socket<network::ipv4_addr>&, message::msg_option_t&, additional_datatype_t&);
        network::udp_socket<network::ipv4_addr> m_networkSocket;

        int m_currentBuffer;

        network::pkt_buffer m_encryptedInputBuffer;
        std::vector<network::pkt_buffer> m_decryptedInputBuffers;

        network::pkt_buffer m_encryptedOutputBuffer;
        network::pkt_buffer m_uncryptedOutputBuffer;

        network::ipv4_addr m_srcAddr;
        additional_datatype_t* m_additional_data;
    };
}

#endif


