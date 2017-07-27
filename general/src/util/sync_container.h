#ifndef general_util_sync_container_h
#define general_util_sync_container_h

#include <atomic>
#include <vector>
#include <stdint.h>
#include <iostream>
#include <cstring>

#include "network/pkt_buffer.h"

namespace util {
    template <typename T>
    class sync_container {
    public:

        sync_container(T& basePtr) : m_basePtr(&basePtr), m_internalData(sizeof(T) / (64) + 1) {

        }

        template <typename dataT>
        inline void markAsUpdate(const dataT& member) {
            int64_t offset = (int64_t)&member - (int64_t)m_basePtr;

            m_internalData[offset / 64] |= (1 << ((offset & 0x3F) >> 3));
            offset += sizeof(dataT) - 1;
            m_internalData[offset / 64] |= (1 << ((offset & 0x3F) >> 3));
        }

        void clear() {
            for (char& c : m_internalData) {
                c = 0;
            }
        }

        void synchronize(sync_container<T>& container, const bool preserve = false) {
            int64_t offset;

            for (int i = 0; i < m_internalData.size(); i++) {
                if (container.m_internalData[i] != 0) {
                    offset = i * 64;

                    for (int bit = 0; bit < 8; bit++) {
                        if (container.m_internalData[i] & (1 << bit)) {
                            if (offset + bit * 8 + sizeof(int64_t) < sizeof(T)) {
                                ((int64_t*)m_basePtr)[(offset + bit * 8) / sizeof(int64_t)] = ((int64_t*)container.m_basePtr)[(offset + bit * 8) / sizeof(int64_t)];
                            } else if (sizeof(T) > (offset + bit * 8)) {
                                std::memcpy(((int8_t*)m_basePtr) + (offset + bit * 8), ((int8_t*)container.m_basePtr) + (offset + bit * 8),  sizeof(T) - (offset + bit * 8));
                            }
                        }
                    }
                }

                container.m_internalData[i] *= preserve;
            }

            atomic_thread_fence(std::memory_order_release); // force the compiler to reload the values (--> optimizer may not see the changes made by this function)
        }


        bool exportData(network::pkt_buffer& buffer, const bool preserve = false) {
            int num_empty = 0;
            int8_t* metaData = nullptr;
            int32_t* num_entry = buffer.pushNext<int32_t>(1);

            if (num_entry == nullptr) {
                return false;
            }

            *num_entry = 0;
            metaData = buffer.pushNext<int8_t>(1);

            for (int i = 0; i < m_internalData.size(); i++) {
                if (m_internalData[i] == 0) {
                    num_empty++;

                    if (num_empty == 127) {
                        metaData = buffer.pushNext<int8_t>(1);

                        if (metaData == nullptr) {
                            return false;
                        }

                        *metaData = 127;
                        num_empty = 0;
                    }
                } else {
                    if (num_empty > 0) {
                        metaData = buffer.pushNext<int8_t>(1);

                        if (metaData == nullptr) {
                            return false;
                        }

                        *metaData = 0x80 | num_empty;
                        num_empty = 0;
                    } else if (metaData != nullptr) {
                        *metaData |= 0x80;
                    }

                    if (!exportSingleEntry(buffer, i)) {
                        return false;
                    }

                    *num_entry += 1;
                    m_internalData[i] *= preserve;
                    metaData = nullptr;
                }
            }

            atomic_thread_fence(std::memory_order_release); // force the compiler to reload the values (--> optimizer may not see the changes made by this function)
            return true;
        }

        bool importData(network::pkt_buffer& buffer) {
            int32_t* num_entry = buffer.getNext<int32_t>(1);
            int8_t* emptyData;
            int8_t* blockInfo;
            int i = 0;
            int state = 0;

            if (num_entry == nullptr) {
                return false;
            }

            while (*num_entry > 0) {
                if (state == 0) {
                    emptyData = buffer.getNext<int8_t>(1);

                    if (emptyData == nullptr) {
                        return false;
                    }

                    i += *emptyData & 0x7F;
                    state = *emptyData & 0x80;
                } else {
                    blockInfo = buffer.getNext<int8_t>(1);

                    if (blockInfo == nullptr) {
                        return false;
                    }

                    if (!importSingleEntry(buffer, i, *blockInfo)) {
                        return false;
                    }

                    i++;
                    *num_entry -= 1;
                    state = 0;
                }
            }

            atomic_thread_fence(std::memory_order_release); // force the compiler to reload the values (--> optimizer may not see the changes made by this function)
            return true;
        }

    private:
        bool exportSingleEntry(network::pkt_buffer& buffer, const int i) {
            int64_t offset;
            int8_t* data;
            int64_t* payload;

            data = buffer.pushNext<int8_t>(1);

            if (data == nullptr) {
                return false;
            }

            *data = m_internalData[i];

            offset = i * 64;

            for (int bit = 0; bit < 8; bit++) {
                if (m_internalData[i] & (1 << bit)) {
                    payload = buffer.pushNext<int64_t>(1);

                    if (payload == nullptr) {
                        return false;
                    }

                    if (offset + bit * 8 + sizeof(int64_t) < sizeof(T)) {
                        *payload = ((int64_t*)m_basePtr)[(offset + bit * 8) / sizeof(int64_t)];
                    } else if (sizeof(T) > (offset + bit * 8)) {
                        *payload = 0;
                        std::memcpy(payload, ((int8_t*)m_basePtr) + (offset + bit * 8),  sizeof(T) - (offset + bit * 8));
                    }
                }
            }

            return true;
        }

        bool importSingleEntry(network::pkt_buffer& buffer, const int i, const int8_t blockInfo) {
            int64_t offset;
            int64_t* payload;

            offset = i * 64;

            for (int bit = 0; bit < 8; bit++) {
                if (blockInfo & (1 << bit)) {
                    payload = buffer.getNext<int64_t>(1);

                    if (payload == nullptr) {
                        return false;
                    }

                    if (offset + bit * 8 + sizeof(int64_t) < sizeof(T)) {
                        ((int64_t*)m_basePtr)[(offset + bit * 8) / sizeof(int64_t)] = *payload;
                    } else if (sizeof(T) > (offset + bit * 8)) {
                        std::memcpy(((int8_t*)m_basePtr) + (offset + bit * 8), payload, sizeof(T) - (offset + bit * 8));
                    }
                }
            }

            return true;
        }




        T* m_basePtr;
        std::vector<uint8_t> m_internalData;
    };
}

#endif
