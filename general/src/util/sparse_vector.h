#ifndef general_util_subset_vector_h
#define general_util_subset_vector_h

#include <vector>
#include <stdint.h>
#include <iostream>

namespace util {
    namespace subset {
        //______________________________________________________________________________________________________
        //
        // Description:
        // - internal entry representation
        //______________________________________________________________________________________________________
        template <typename T>
        struct subset_vector_entry_t {
            T data;
            uint64_t next;
        };

        template <typename T>
        class base_vector {
        public:
            base_vector(const m_sizet size) : m_internalData(size) {
                for (uint64_t i = 0; i < size; i++) {
                    m_internalData[i].next = size;
                }
            }

            subset_vector_entry_t<T>* data() {
                return m_internalData.data();
            }

            m_sizet fullSize() const {
                return m_internalData.size();
            }
        private:
            std::vector<subset_vector_entry_t<T>> m_internalData;
        };
        //______________________________________________________________________________________________________
        //
        // subset vector optimized for fast iterations on a subset of the entries
        //______________________________________________________________________________________________________
        template <typename T>
        class vector {
        public:
            //______________________________________________________________________________________________________
            //
            // Description:
            // - initializes the internal buffer with 'size' entries
            // - all entries are excluded from iterations
            // Parameter:
            // - size: initiale size of the vector
            //______________________________________________________________________________________________________
            vector(base_vector<T>& contr, const int64_t offset, const m_sizet size) : m_internalDataSize(size), m_internalDataEnd(offset + size), m_internalDataOffset(offset) {
                m_internalData = contr.data();
                m_internalFullSize = contr.fullSize();

                int emptyMode = 0;
                int connectMode = 0;
                uint64_t currentMin = 0, currentMax = 0;
                uint64_t current;
                uint64_t lastSelectedEntry, nextEntry, replaceEntry, lastEntry;

                for (uint64_t i = offset; i < m_internalDataEnd; i++) {


                    current = m_internalDataEnd - 1 - i;

                    if (emptyMode == 0) {
                        if (m_internalData[current].next != m_internalFullSize) {
                            emptyMode = 1;
                            currentMin = current;
                            currentMax = current;
                        }
                    }

                    if (emptyMode == 1) {
                        if (m_internalData[current].next != m_internalFullSize) {
                            if (currentMin > m_internalData[current].next && m_internalData[current].next != current) {
                                currentMin = m_internalData[current].next;
                            }
                        } else {
                            m_internalData[current].next = currentMin;
                        }
                    }

                    if (connectMode == 0)  {
                        if (m_internalData[current].next <= current) {
                            lastSelectedEntry = current;
                            lastEntry = current;
                            connectMode = 1;
                        }
                    }

                    if (connectMode == 1) {
                        if (m_internalData[lastEntry].next == m_internalData[current].next && m_internalData[current].next <= current) {
                            lastEntry = current;
                        }

                        if (m_internalData[current].next != m_internalData[lastSelectedEntry].next) {
                            if (m_internalData[current].next <= current) {
                                replaceEntry = m_internalData[current].next;
                                connectMode = 2;
                            } else {

                                lastSelectedEntry = current;
                                uint64_t swap = m_internalData[current].next;
                                m_internalData[current].next = replaceEntry;
                                replaceEntry = swap;
                            }
                        }
                    }

                    if (connectMode == 2) {
                        if (m_internalData[current].next <= current) {
                            if (m_internalData[current].next == replaceEntry) {
                                m_internalData[current].next = m_internalData[lastSelectedEntry].next;
                                lastSelectedEntry = current;
                            } else {
                                connectMode = 1;
                                lastSelectedEntry = current;
                                uint64_t swap = m_internalData[current].next;
                                m_internalData[current].next = replaceEntry;
                                replaceEntry = swap;
                            }
                        }
                    }
                }

                for (uint64_t i = lastEntry; i < m_internalDataEnd; i++) {
                    m_internalData[i].next = currentMin;
                }
            }
            //______________________________________________________________________________________________________
            //
            // Description:
            // - includes the entry specified by his index
            // Parameter:
            // - index: index of the entry
            //______________________________________________________________________________________________________
            void include(const uint64_t index) {
                uint64_t diffNext, diffCurrent;
                uint64_t i = index;

                i += m_internalDataSize * (i == m_internalDataOffset);
                i--;

                do {
                    m_internalData[i].next = index;
                    i += m_internalDataSize * (i == m_internalDataOffset);
                    i--;

                    diffNext = m_internalData[i].next;
                    diffNext += m_internalDataSize * (diffNext <= i);

                    diffCurrent = index;
                    diffCurrent += m_internalDataSize * (diffCurrent <= i);

                } while (diffCurrent < diffNext || m_internalData[i].next == m_internalFullSize);
            }
            //______________________________________________________________________________________________________
            //
            // Description:
            // - excludes the entry specified by his index
            // Parameter:
            // - index: index of the entry
            //______________________________________________________________________________________________________
            void exclude(const uint64_t index) {
                uint64_t i = index;
                uint64_t newNext = m_internalData[index].next;

                i += m_internalDataSize * (i == m_internalDataOffset);
                i--;

                do {
                    m_internalData[i].next = newNext;

                    i += m_internalDataSize * (i == m_internalDataOffset);
                    i--;
                } while (m_internalData[i].next == index);
            }
            //______________________________________________________________________________________________________
            //
            // Description:
            // - accesses the internal data of the entry specified by his index
            // - the data may not be initialized
            // Parameter:
            // - index: index of the entry
            // Return:
            // - reference to stored data
            //______________________________________________________________________________________________________
            T& operator[](const uint64_t index) {
                return m_internalData[index].data;
            }
            //______________________________________________________________________________________________________
            //
            // Description:
            // - returns the successor of the entry specified by his index
            // - the successor is specified by his own index
            // - after the last entry, the successor is the first entry again (--> ring buffer)
            // Parameter:
            // - index: index of the entry
            // Return:
            // - index of successor-entry
            //______________________________________________________________________________________________________
            uint64_t next(const uint64_t index) const {
                return m_internalData[index].next;
            }
            //______________________________________________________________________________________________________
            //
            // Description:
            // - returns the first included entry
            // Return:
            // - 1 | on success
            // - index of last exluded entry | on empty vector
            //______________________________________________________________________________________________________
            uint64_t start() const {
                return m_internalData[m_internalDataEnd - 1].next;
            }
            //______________________________________________________________________________________________________
            //
            // Return:
            // - size of the internal vector
            //______________________________________________________________________________________________________
            uint64_t end() const {
                return m_internalFullSize;
            }
            //______________________________________________________________________________________________________
            //
            // Description:
            // - resizes the vector to 'size' entries
            // Parameter:
            // - size: new size of vector
            //______________________________________________________________________________________________________
            void resize(const m_sizet size) {
                if (m_internalDataEnd < size) {
//                 m_internalData.resize(size);
                    extendRebalance();
                } else {
                    uint64_t firstEntry = m_internalData[m_internalDataEnd - 1].next;
//                 m_internalData.resize(size);
                    shrinkRebalance(firstEntry);
                }
            }

            void print() {
                for (uint64_t i = m_internalDataOffset; i < m_internalDataEnd; i++) {
                    std::cout << "<" << i << "," << m_internalData[i].next << "> ";
                }

                std::cout << std::endl;
            }
        private:
            //______________________________________________________________________________________________________
            //
            // Description:
            // - rebalances the vector after shrinking in resize()
            // Parameter:
            // - firstEntry: first included entry before shrinking
            //______________________________________________________________________________________________________
            void shrinkRebalance(const uint64_t firstEntry) {
                if (firstEntry >= m_internalDataEnd) {
                    for (uint64_t i = 0; i < m_internalDataEnd; i++) {
                        m_internalData[i].next = i;
                    }

                    return;
                }

                for (uint64_t i = 0; i < m_internalDataEnd; i++) {
                    if (m_internalData[i].next >= m_internalDataEnd) {
                        m_internalData[i].next = firstEntry;
                    }
                }
            }
            //______________________________________________________________________________________________________
            //
            // Description:
            // - rebalances the vector after extending in resize()
            //______________________________________________________________________________________________________
            void extendRebalance() {
                uint64_t last;

                for (uint64_t i = 0; i < m_internalDataEnd; i++) {
                    last = i;
                    last += m_internalDataEnd * (last == 0);
                    last--;

                    if (m_internalData[last].next != m_internalData[i].next && m_internalData[last].next != i) {
                        if (m_internalData[last].next == last) {
                            m_internalData[i].next = i;
                        } else {
                            m_internalData[i].next = m_internalData[last].next;
                        }
                    }
                }
            }

            m_sizet m_internalFullSize;
            m_sizet m_internalDataSize;
            m_sizet m_internalDataEnd;
            m_sizet m_internalDataOffset;
            subset_vector_entry_t<T>* m_internalData;
        };
    }
}

#endif

