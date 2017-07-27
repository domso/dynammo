#ifndef general_util_tree_h
#define general_util_tree_h

#include <stdint.h>
#include <iostream>
#include <atomic>

namespace util {
    namespace data_tree {

        template <typename T>
        class node {
        public:
            node() {
                for (int i = 0; i < 256; i++) {
                    childs_[i].store(nullptr);
                }
            }
            ~node() {
                node<T>* tmp;

                for (int i = 0; i < 256; i++) {
                    tmp = childs_[i].load();

                    if (tmp != nullptr) {
                        delete tmp;
                    }
                }
            }

            class iterator {
            public:

                T operator++(int x) {
                    uint64_t newIndex = m_index + x;
                    uint64_t tmp = m_index;

                    for (int i = sizeof(uint64_t) - 1; i >= 0; i--) {
                        for (int j = (tmp & 255) + 1; j < 256; j++) {
                            if (links_[i]->childs_[j] != nullptr) {
                                tmp = tmp ^ (tmp & 255);
                                tmp += j;
                                links_[i + 1] = links_[i]->childs_[j];

                                for (int k = i + 2; k < sizeof(uint64_t) + 1; k++) {
                                    tmp = tmp << 8;

                                    for (int m = 0; m < 256; m++) {
                                        if (links_[k - 1]->childs_[m] != nullptr) {
                                            links_[k] = links_[k - 1]->childs_[m];
                                            tmp += m;
                                            break;
                                        }
                                    }
                                }

                                m_index = tmp;
                                return links_[sizeof(uint64_t)]->data;
                            }
                        }

                        tmp = tmp >> 8;
                    }

                    return links_[sizeof(uint64_t)]->data;
                }


                node<T>* links_[sizeof(uint64_t) + 1];
                uint64_t m_index;
            };


            void insert(uint64_t id, T data) {

                node<T>* current = this;
                node<T>* next = nullptr;
                node<T>* newNode = nullptr;
                unsigned char index;

                for (int i = sizeof(uint64_t) - 1; i >= 0; i--) {
                    index = *(((char*)(&id)) + i);
                    next = current->childs_[index].load();

                    if (next == nullptr) {
                        newNode = new node<T>();
//                         std::cout << id << ": " << newNode << " -->" << (int)index << std::endl;

                        if (!current->childs_[index].compare_exchange_strong(next, newNode)) {
                            delete newNode;
                        } else {
                            next = newNode;
                        }
                    }

                    current = next;

                }

                current->data = data;
            }

            iterator search(uint64_t id) {
                node<T>* current = this;
                node<T>* next = nullptr;
                unsigned char index;

                iterator result;
                result.m_index = id;
                result.links_[0] = current;

                for (int i = sizeof(uint64_t) - 1; i >= 0; i--) {
                    index = *(((char*)(&id)) + i);
                    next = current->childs_[index].load();

                    if (next == nullptr) {
                        for (int j = 0; j < sizeof(uint64_t); j++) {
                            result.links_[j] = nullptr;
                        }

                        return result;
                    }

                    result.links_[sizeof(uint64_t) - i] = next;
                    current = next;
                }

                return result;
            }

            T* get(uint64_t id) {
                node<T>* current = this;
                node<T>* next = nullptr;
                char index;

                iterator it;
                it.links_[0] = nullptr;

                for (int i = sizeof(uint64_t) - 1; i >= 0; i--) {
                    index = *(((char*)(&id)) + i);
                    next = current->childs_[index].load();

                    if (next == nullptr) {
                        return nullptr;
                    }

                    current = next;

                }

                return &current->data;
            }

            T& getData() {
                return data;
            }

            T data;
            std::atomic<node<T>*> childs_[256];
        };
    }
}

#endif

