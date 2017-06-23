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
            base_vector(const size_t size) : internalData_(size) {
                for (uint64_t i = 0; i < size; i++) {
                    internalData_[i].next = size;
                }
            }

            subset_vector_entry_t<T>* data() {
                return internalData_.data();
            }

            size_t fullSize() const {
                return internalData_.size();
            }
        private:
            std::vector<subset_vector_entry_t<T>> internalData_;
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
            vector(base_vector<T>& contr, const int64_t offset, const size_t size) : internalDataSize_(size), internalDataEnd_(offset + size), internalDataOffset_(offset) {
                internalData_ = contr.data();
                internalFullSize_ = contr.fullSize();

                int emptyMode = 0;
                int connectMode = 0;
                uint64_t currentMin = 0, currentMax = 0;
                uint64_t current;
                uint64_t lastSelectedEntry, nextEntry, replaceEntry, lastEntry;

                for (uint64_t i = offset; i < internalDataEnd_; i++) {


                    current = internalDataEnd_ - 1 - i;

                    if (emptyMode == 0) {
                        if (internalData_[current].next != internalFullSize_) {
                            emptyMode = 1;
                            currentMin = current;
                            currentMax = current;
                        }
                    }

                    if (emptyMode == 1) {
                        if (internalData_[current].next != internalFullSize_) {
                            if (currentMin > internalData_[current].next && internalData_[current].next != current) {
                                currentMin = internalData_[current].next;
                            }
                        } else {
                            internalData_[current].next = currentMin;
                        }
                    }

                    if (connectMode == 0)  {
                        if (internalData_[current].next <= current) {
                            lastSelectedEntry = current;
                            lastEntry = current;
                            connectMode = 1;
                        }
                    }

                    if (connectMode == 1) {
                        if (internalData_[lastEntry].next == internalData_[current].next && internalData_[current].next <= current) {
                            lastEntry = current;
                        }

                        if (internalData_[current].next != internalData_[lastSelectedEntry].next) {
                            if (internalData_[current].next <= current) {
                                replaceEntry = internalData_[current].next;
                                connectMode = 2;
                            } else {

                                lastSelectedEntry = current;
                                uint64_t swap = internalData_[current].next;
                                internalData_[current].next = replaceEntry;
                                replaceEntry = swap;
                            }
                        }
                    }

                    if (connectMode == 2) {
                        if (internalData_[current].next <= current) {
                            if (internalData_[current].next == replaceEntry) {
                                internalData_[current].next = internalData_[lastSelectedEntry].next;
                                lastSelectedEntry = current;
                            } else {
                                connectMode = 1;
                                lastSelectedEntry = current;
                                uint64_t swap = internalData_[current].next;
                                internalData_[current].next = replaceEntry;
                                replaceEntry = swap;
                            }
                        }
                    }
                }

                for (uint64_t i = lastEntry; i < internalDataEnd_; i++) {
                    internalData_[i].next = currentMin;
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

                i += internalDataSize_ * (i == internalDataOffset_);
                i--;

                do {
                    internalData_[i].next = index;
                    i += internalDataSize_ * (i == internalDataOffset_);
                    i--;

                    diffNext = internalData_[i].next;
                    diffNext += internalDataSize_ * (diffNext <= i);

                    diffCurrent = index;
                    diffCurrent += internalDataSize_ * (diffCurrent <= i);

                } while (diffCurrent < diffNext || internalData_[i].next == internalFullSize_);
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
                uint64_t newNext = internalData_[index].next;

                i += internalDataSize_ * (i == internalDataOffset_);
                i--;

                do {
                    internalData_[i].next = newNext;

                    i += internalDataSize_ * (i == internalDataOffset_);
                    i--;
                } while (internalData_[i].next == index);
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
                return internalData_[index].data;
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
                return internalData_[index].next;
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
                return internalData_[internalDataEnd_ - 1].next;
            }
            //______________________________________________________________________________________________________
            //
            // Return:
            // - size of the internal vector
            //______________________________________________________________________________________________________
            uint64_t end() const {
                return internalFullSize_;
            }
            //______________________________________________________________________________________________________
            //
            // Description:
            // - resizes the vector to 'size' entries
            // Parameter:
            // - size: new size of vector
            //______________________________________________________________________________________________________
            void resize(const size_t size) {
                if (internalDataEnd_ < size) {
//                 internalData_.resize(size);
                    extendRebalance();
                } else {
                    uint64_t firstEntry = internalData_[internalDataEnd_ - 1].next;
//                 internalData_.resize(size);
                    shrinkRebalance(firstEntry);
                }
            }

            void print() {
                for (uint64_t i = internalDataOffset_; i < internalDataEnd_; i++) {
                    std::cout << "<" << i << "," << internalData_[i].next << "> ";
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
                if (firstEntry >= internalDataEnd_) {
                    for (uint64_t i = 0; i < internalDataEnd_; i++) {
                        internalData_[i].next = i;
                    }

                    return;
                }

                for (uint64_t i = 0; i < internalDataEnd_; i++) {
                    if (internalData_[i].next >= internalDataEnd_) {
                        internalData_[i].next = firstEntry;
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

                for (uint64_t i = 0; i < internalDataEnd_; i++) {
                    last = i;
                    last += internalDataEnd_ * (last == 0);
                    last--;

                    if (internalData_[last].next != internalData_[i].next && internalData_[last].next != i) {
                        if (internalData_[last].next == last) {
                            internalData_[i].next = i;
                        } else {
                            internalData_[i].next = internalData_[last].next;
                        }
                    }
                }
            }

            size_t internalFullSize_;
            size_t internalDataSize_;
            size_t internalDataEnd_;
            size_t internalDataOffset_;
            subset_vector_entry_t<T>* internalData_;
        };
    }
}

#endif

