#ifndef WEBD_HEAP_H
#define WEBD_HEAP_H
#include <array>
#include <utility>
#include <stdio.h>

#define MAX_SIZE 21

template <class T>
struct Less {
    bool operator() (const T& left, const T& right) const {
        return left > right;
    }
};

template <class T>
struct Greater {
    bool operator() (const T& left, const T& right) const {
        return left < right;
    }
};

template <class T, class Compare=Less<T>>
class Heap
{
    private:
        std::array<T, MAX_SIZE> arr_{};
        int count_;
        int firstIndex_;
    
    public:
        Heap()
            :count_(0), firstIndex_(1)
        {
        }

        void push(const T& x)
        {
            if (count_ >= MAX_SIZE) {
                return;
            }

            count_++;
            arr_[count_] = x;

            int i = count_;
            if (i >= MAX_SIZE) return;
            Compare com;

            // 自下而上堆化
            while (i / 2 >= 0 && com(arr_[i],arr_[i / 2])) {
                std::swap(arr_[i], arr_[i / 2]);
                i = i / 2;
            }

        }

        void pop()
        {
            if (count_ == 0) {
                std::swap(arr_[firstIndex_], arr_[count_]);
                --count_;
                heapify(count_, firstIndex_);
            }
        }

        // 自上而下堆化
        void heapify(int n, int parent)
        {
            int maxPos, left, right;
            Compare com;

            while (true) {
                maxPos = parent;
                left = parent * 2;
                right = parent * 2 + 1;

                if (left <= n && com(arr_[parent], arr_[left])) maxPos = left;
                if (right <= n && com(arr_[maxPos], arr_[right])) maxPos = right;

                if (maxPos == parent) break;
                std::swap(arr_[parent], arr_[maxPos]);
                parent = maxPos;
            }
        }

        std::array<T, MAX_SIZE> getArr()
        {
            return arr_;
        }
};

#endif