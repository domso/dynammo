#ifndef general_util_sparse_vector_h
#define general_util_sparse_vector_h

#include <vector>
#include <stdint.h>
#include <iostream>

namespace util {
    //______________________________________________________________________________________________________
    //
    // sparse vector optimized for fast iterations on a subset of the entries
    //______________________________________________________________________________________________________
    template <typename T>
    class sparse_vector {
    public:
        //______________________________________________________________________________________________________
        //
        // Description:
        // - initializes the internal buffer with 'size' entries
        // - all entries are excluded from iterations
        // Parameter:
        // - size: initiale size of the vector
        //______________________________________________________________________________________________________
        sparse_vector(const size_t size) : internalData_(size) {
            for (uint64_t i = 0; i < size; i++) {
                internalData_[i].next = i;
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

            i += internalData_.size() * (i == 0);
            i--;

            do {
                internalData_[i].next = index;
                i += internalData_.size() * (i == 0);
                i--;

                diffNext = internalData_[i].next;
                diffNext += internalData_.size() * (diffNext <= i);

                diffCurrent = index;
                diffCurrent += internalData_.size() * (diffCurrent <= i);
            } while (diffCurrent < diffNext);
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

            i += internalData_.size() * (i == 0);
            i--;

            do {
                if (newNext == index) {
                    internalData_[i].next = i;
                } else {
                    internalData_[i].next = newNext;
                }

                i += internalData_.size() * (i == 0);
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
            return internalData_[internalData_.size() - 1].next;
        }
        //______________________________________________________________________________________________________
        //
        // Return:
        // - size of the internal vector
        //______________________________________________________________________________________________________
        uint64_t end() const {
            return internalData_.size();
        }
        //______________________________________________________________________________________________________
        //
        // Description:
        // - resizes the vector to 'size' entries
        // Parameter:
        // - size: new size of vector
        //______________________________________________________________________________________________________
        void resize(const size_t size) {
            if (internalData_.size() < size) {
                internalData_.resize(size);
                extendRebalance();
            } else {
                uint64_t firstEntry = internalData_[internalData_.size() - 1].next;
                internalData_.resize(size);
                shrinkRebalance(firstEntry);
            }
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
            if (firstEntry >= internalData_.size()) {
                for (uint64_t i = 0; i < internalData_.size(); i++) {
                    internalData_[i].next = i;
                }

                return;
            }

            for (uint64_t i = 0; i < internalData_.size(); i++) {
                if (internalData_[i].next >= internalData_.size()) {
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

            for (uint64_t i = 0; i < internalData_.size(); i++) {
                last = i;
                last += internalData_.size() * (last == 0);
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
        //______________________________________________________________________________________________________
        //
        // Description:
        // - internal entry representation
        //______________________________________________________________________________________________________
        struct sparse_vector_entry_t {
            T data;
            uint64_t next;
        };

        std::vector<sparse_vector_entry_t> internalData_;
    };
}

#endif

