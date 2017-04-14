#ifndef general_msg_controller_h
#define general_msg_controller_h

#include "assert.h"
#include "network/udp_socket.h"
#include "include/authentication/types.h"
#include "include/message/msg_header.h"
#include "include/message/msg_types.h"
#include "include/util/timestamp.h"
#include "include/util/mem.h"
#include "include/encryption/rsa.h"

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
        msg_controller(const int bufferSize) :
            encryptedInputBuffer_(bufferSize), decryptedInputBuffer_(bufferSize),
            encryptedOutputBuffer_(bufferSize), uncryptedOutputBuffer_(bufferSize) {
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
            this->additional_data_ = additional_data;
            return (additional_data != nullptr && networkSocket_.init(port));
        }
        //______________________________________________________________________________________________________
        //
        // Description:
        // - closes the internal sockets and shutdown all recv()-calls
        //______________________________________________________________________________________________________
        void close() {
            networkSocket_.shutRDWR();
        }
        //______________________________________________________________________________________________________
        //
        // Description:
        // - starts to receive on the internal socket for incoming udp packages
        // - every package will be decrypted with the optional private-key and validated with the optional public-key
        // - the message is required in the format: <signature(optional)|header|data>
        // - the header specifies the requestHandler with the message type (see msg_header_t and registerHandler()) 
        // - to stop receiving either an internal error needs to occure, or close() was called
        // Parameters:
        // - privateKey: valid pointer to a loaded instance of a private-key (optional)
        // - publicKey: valid pointer to a loaded instance of a public-key (optional)
        //______________________________________________________________________________________________________
        void recv(const encryption::private_key* privateKey = nullptr, const encryption::public_key* publicKey = nullptr) {
            assert(sizeof(msg_header_t::msgType) == 1);
            assert(additional_data_ != nullptr);

            msg_header_t* header;

            while (internalRecv(privateKey, publicKey)) {
                header = decryptedInputBuffer_.getNext<msg_header_t>();
                if (header != nullptr) {
                    if (callbacks[header->msgType] != nullptr) {
                        if (privateKey != nullptr) {
                            uncryptedOutputBuffer_.reserveLen(privateKey->getRequiredSize());
                        } else {
                            uncryptedOutputBuffer_.setMsgLen(0);
                        }
                        message::msg_header_t* outputHeader = uncryptedOutputBuffer_.pushNext<message::msg_header_t>();
                        message::msg_status_t status = callbacks[header->msgType](*header, srcAddr_, decryptedInputBuffer_, uncryptedOutputBuffer_, networkSocket_, *additional_data_);
                        if (status != MSG_STATUS_CLOSE && outputHeader != nullptr) {
                            outputHeader->status = status;
                            outputHeader->msgType = header->msgType ^ MSG_HEADER_TYPE_REQUEST_SWITCH_MASK;
                            outputHeader->attr = header->attr;
                            if (privateKey == nullptr || internalSign(*privateKey, uncryptedOutputBuffer_)) {
                                if (!internalSend(srcAddr_,  uncryptedOutputBuffer_, encryptedOutputBuffer_, publicKey)) {
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
        //
        // Description:
        // - tries to execute a request (static function) specified by the template type T
        // - the message will be encrypted with the optional public-key and signed with the optional private-key
        // - T needs to implement a static function bool T::request(message::msg_header_t&, network::ipv4_addr&, network::pkt_buffer&, network::udp_socket<network::ipv4_addr>&, Tparam)
        // - T::request() should push the request-payload into the pkt_buffer
        // - if T::request() returns true, the message will be send to destination address specified by destAddr
        // Parameters:
        // - destAddr: destination address of the request
        // - uncryptedOutputBuffer: buffer for the uncrypted message
        // - encryptedOutputBuffer: buffer for the encrypted message
        // - param: unspecific additional argument for T::request()
        // - privateKey: valid pointer to a loaded instance of a private-key (optional)
        // - publicKey: valid pointer to a loaded instance of a public-key (optional)
        // Return:
        // - true  | on success
        // - false | on any error
        //______________________________________________________________________________________________________
        template <typename T, typename Tparam>
        bool execRequest(network::ipv4_addr& destAddr, network::pkt_buffer& uncryptedOutputBuffer, network::pkt_buffer& encryptedOutputBuffer, Tparam& param, const encryption::private_key* privateKey = nullptr, const encryption::public_key* publicKey = nullptr) {
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
                
                if (T::request(*outputHeader, destAddr, uncryptedOutputBuffer, networkSocket_, param)) {
                    if (privateKey == nullptr || internalSign(*privateKey, uncryptedOutputBuffer)) {
                        if (!internalSend(destAddr,  uncryptedOutputBuffer, encryptedOutputBuffer, publicKey)) {
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
        // - T needs to implement a static function message::msg_status_t requestHandler(message::msg_header_t&, network::ipv4_addr&, network::pkt_buffer&, network::pkt_buffer&, network::udp_socket<network::ipv4_addr>&, additional_datatype_t);
        // - T needs to implement a static function message::msg_status_t responseHandler(message::msg_header_t&, network::ipv4_addr&, network::pkt_buffer&, network::pkt_buffer&, network::udp_socket<network::ipv4_addr>&, additional_datatype_t);
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
            if (privateKey != nullptr) { // encryption
                if (networkSocket_.recvPkt(srcAddr_, encryptedInputBuffer_) == -1) {
                    return false;
                }

                decryptedInputBuffer_.setMsgLen(encryption::decryptChar(*privateKey, encryptedInputBuffer_.msgLen(), (unsigned char*) encryptedInputBuffer_.data(), decryptedInputBuffer_.capacity(), (unsigned char*) decryptedInputBuffer_.data()));
            } else { // no encryption
                if (networkSocket_.recvPkt(srcAddr_, decryptedInputBuffer_) == -1) {
                    return false;
                }
            }

            if (publicKey != nullptr) { // validate signature
                int requiredSize = publicKey->getRequiredSize();
                char* signature = decryptedInputBuffer_.getNext<char>(requiredSize);

                if (signature != nullptr) {
                    if (!encryption::verifyChar(*publicKey, requiredSize, (unsigned char*) signature, decryptedInputBuffer_.remainingMsgLen(), (unsigned char*) decryptedInputBuffer_.dataOffset())) {
                        // delete message
                        decryptedInputBuffer_.setMsgLen(0);
                    }
                }

            }

            return true;
        }
        //______________________________________________________________________________________________________
        //
        // dont use it outside of recv() or execRequest()
        //______________________________________________________________________________________________________
        bool internalSend(network::ipv4_addr& dest, network::pkt_buffer& uncryptedOutputBuffer, network::pkt_buffer& encryptedOutputBuffer, const encryption::public_key* publicKey = nullptr, const int flags = 0) {
            if (publicKey != nullptr) {
                encryptedOutputBuffer.setMsgLen(encryption::encryptChar(*publicKey, uncryptedOutputBuffer.msgLen(), (unsigned char*) uncryptedOutputBuffer.data(), encryptedOutputBuffer.capacity(), (unsigned char*) encryptedOutputBuffer.data()));
                return networkSocket_.sendPkt(dest, encryptedOutputBuffer, flags) == encryptedOutputBuffer.msgLen();
            } else {
                return networkSocket_.sendPkt(dest, uncryptedOutputBuffer, flags) == uncryptedOutputBuffer.msgLen();
            }
        }
        //______________________________________________________________________________________________________
        //
        // dont use it outside of recv() or execRequest()
        //______________________________________________________________________________________________________
        bool internalSign(const encryption::private_key& privateKey, network::pkt_buffer& buffer) {
            buffer.reset();
            int signatureLen = privateKey.getRequiredSize();
            unsigned char* sigature = buffer.getNext<unsigned char>(signatureLen);
            return encryption::signChar(privateKey, buffer.remainingMsgLen(), (unsigned char*) buffer.dataOffset(), signatureLen, sigature) != -1;
        }
        
        message::msg_status_t (*callbacks[256])(message::msg_header_t& header, network::ipv4_addr&, network::pkt_buffer&, network::pkt_buffer&, network::udp_socket<network::ipv4_addr>&, additional_datatype_t&);
        network::udp_socket<network::ipv4_addr> networkSocket_;
        network::pkt_buffer encryptedInputBuffer_;
        network::pkt_buffer decryptedInputBuffer_;
        network::pkt_buffer encryptedOutputBuffer_;
        network::pkt_buffer uncryptedOutputBuffer_;
        network::ipv4_addr srcAddr_;
        additional_datatype_t* additional_data_;
    };
}

#endif


