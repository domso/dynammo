#ifndef general_util_obj_cache_h
#define general_util_obj_cache_h

#include <stdint.h>
#include <unordered_map>
#include <mutex>
#include "src/util/msg_queue.h"
#include "src/util/sem.h"

namespace util { 
    template <typename data_t, typename allocator_t>
    class obj_cache {
    public:    
        obj_cache(allocator_t& allocator, const int size = 16384) : m_allocator(allocator), m_taskQueue(size), m_dataStorage(size) {
            
        }        
        
        data_t load_sync(const uint64_t requestedID) {
            std::unique_lock<std::mutex> ul(m_mutex);
            typename base_list::list_node* node;
            
            if (m_indexMap.count(requestedID) == 0 || (node = m_dataStorage.get(m_indexMap[requestedID]))->id != requestedID) {
                int64_t index = m_dataStorage.insert(m_allocator.load(requestedID), requestedID);
                m_indexMap.insert_or_assign(requestedID, index);
                node = m_dataStorage.get(m_indexMap[requestedID]);
            } 
            
            return node->data;
        }
        
        data_t load_async(const uint64_t requestedID, const data_t& placeHolder) {            
            std::unique_lock<std::mutex> ul(m_mutex);
            typename base_list::list_node* node;
            
            if (m_indexMap.count(requestedID) == 0 || (node = m_dataStorage.get(m_indexMap[requestedID]))->id != requestedID) {
                auto writer = m_taskQueue.write<uint64_t>();
                *writer.obj = requestedID;
                m_sem.post();
                int64_t index = m_dataStorage.insert(placeHolder, requestedID);
                m_indexMap.insert_or_assign(requestedID, index);
            } 
            
            return m_dataStorage.get(m_indexMap[requestedID]);
        }
        
        void update(const double timeOut) {
            if (m_sem.timedWait(timeOut)) {
                auto reader = m_taskQueue.read<uint64_t>();
                uint64_t key = *reader.obj;
                data_t tmp = m_allocator.load(key);
                
                m_mutex.lock();
                m_dataStorage.update(m_indexMap[key], tmp);
                m_mutex.unlock();
            }                        
        }
    private:        
        class base_list {
        public:
            base_list(const int numNodes) : m_listNodes(numNodes) {
                for (int i = 0; i < m_listNodes.size(); i++) {
                    m_listNodes[i].prev = i - 1;
                    m_listNodes[i].next = i + 1;
                }
                
                // cyclic buffer
                m_listNodes[0].prev = m_listNodes.size() - 1;
                m_listNodes[m_listNodes.size() - 1].next = 0;
                m_nextFreeNode = 0;
            }
            
            struct list_node {
                data_t data;
                int64_t prev;
                int64_t next;
                uint64_t id;
            };   
            
            list_node* get(const int64_t index) {
                if (m_listNodes[m_nextFreeNode].prev != index) {
                    list_node& currentNode = m_listNodes[index];
                    m_listNodes[currentNode.prev].next = currentNode.next;
                    m_listNodes[currentNode.next].prev = currentNode.prev;
                    
                    currentNode.prev = m_listNodes[m_nextFreeNode].prev;
                    currentNode.next = m_nextFreeNode;
                    
                    m_listNodes[currentNode.next].prev = index;
                    m_listNodes[currentNode.prev].next = index;
                }
                
                return &m_listNodes[index];
            }
            
            int64_t insert(data_t&& data, const uint64_t id) {
                int64_t result = m_nextFreeNode;
                m_listNodes[m_nextFreeNode].data = std::move(data);
                m_listNodes[m_nextFreeNode].id = id;
                m_nextFreeNode = m_listNodes[m_nextFreeNode].next;
                
                return result;
            }
            
            void update(const int64_t index, data_t& data) {
                m_listNodes[index].data = std::move(data);
            }
        private: 
            int64_t m_nextFreeNode;
            std::vector<list_node> m_listNodes;            
        };
                
        allocator_t& m_allocator;
        std::mutex m_mutex;
        util::semaphore m_sem;
        util::msg_queue m_taskQueue;
        base_list m_dataStorage;
        std::unordered_map<uint64_t, int64_t> m_indexMap;
    };
}

#endif
