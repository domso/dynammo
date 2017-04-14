#include "include/util/msg_queue.h"

namespace util {

    msg_queue::msg_queue(const int size) : size_(size + 64), data_((size + 64) * 2), start_(0), end_(0), commit_(0), acquired_(0) {

    }

    bool msg_queue::release(const int lastFree, const int sizeFree, int numTries) {
        int last, next;
        next = lastFree + sizeFree;
        next -= size_ * (next >= size_);

        do {
            if (numTries == 0) {
                return false;
            }

            numTries -= numTries > 0;
            last = lastFree;
        } while (!start_.compare_exchange_strong(last, next));

        return true;
    }

    int msg_queue::load(const int sizeLoad, int numTries) {
        int start, end, newStart, finalStart;

        do {
            if (numTries == 0) {
                return -1;
            }

            numTries -= numTries > 0;

            start = acquired_.load();
            end = commit_.load();
            end += size_ * (end < start);

            newStart = start + sizeLoad;
            finalStart = newStart - size_ * (newStart >= size_);
        } while (newStart > end || !acquired_.compare_exchange_strong(start, finalStart));

        return start;
    }

    bool msg_queue::commit(const int lastCommit, const int sizeCommit, int numTries) {
        int last, next;
        next = lastCommit + sizeCommit;
        next -= size_ * (next >= size_);

        do {
            if (numTries == 0) {
                return false;
            }

            numTries -= numTries > 0;
            last = lastCommit;
        } while (!commit_.compare_exchange_strong(last, next));

        return true;
    }

    int msg_queue::reserve(const int sizeStorage, int numTries) {
        int start, end, newEnd, finalEnd;

        do {
            if (numTries == 0) {
                return -1;
            }

            numTries -= numTries > 0;

            start = start_.load();
            end = end_.load();
            start += size_ * (start <= end);

            newEnd = end + sizeStorage;
            finalEnd = newEnd - size_ * (newEnd >= size_);
        } while (newEnd >= start || !end_.compare_exchange_strong(end, finalEnd));

        return end;
    }

    int msg_queue::size() const {
        return size_ - 64;
    }

    int msg_queue::usage() const {
        int result = end_.load() - start_.load();
        result += size_ * (result < 0);
        return result;
    }

    int msg_queue::acquired() const {
        int result = acquired_.load() - start_.load();
        result += size_ * (result < 0);
        return result;
    }

    int msg_queue::available() const {
        int result = commit_.load() - acquired_.load();
        result += size_ * (result < 0);
        return result;
    }

    int msg_queue::uncommited() const {
        int result = end_.load() - commit_.load();
        result += size_ * (result < 0);
        return result;
    }

    int msg_queue::free() const {
        int result = start_.load() - end_.load();
        result += size_ * (result <= 0);
        return result - 64;
    }


    void msg_queue::printDebugInfo() {
        std::cout << "-------------------------------------" << std::endl;
        std::cout << "data:       [" << start_.load() << ", " << end_.load() << "]" << std::endl;
        std::cout << "acquired:   [" << start_.load() << ", " << acquired_.load() << "]" << std::endl;
        std::cout << "available:  [" << acquired_.load() << ", " << commit_.load() << "]" << std::endl;
        std::cout << "uncommited: [" << commit_.load() << ", " << end_.load() << "]" << std::endl;
    }
}
