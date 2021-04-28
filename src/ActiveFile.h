#ifndef WEBD_HEAP_H
#define WEBD_HEAP_H
#include <array>
#include <utility>
#include <string>
#include <unordered_map>
#include <iostream>
namespace webd
{
    #define MAX_SIZE 21

    // template <class T>
    // struct Less {
    //     bool operator() (const T& left, const T& right) const {
    //         return left > right;
    //     }
    // };

    // template <class T>
    // struct Greater {
    //     bool operator() (const T& left, const T& right) const {
    //         return left < right;
    //     }
    // };

    // template <class T>
    // struct swapCal {
    //     void operator() (const T& left, const T& right, int leftIndex, int rigntIndex) const {
    //         return;
    //     }
    // };

    // template <class T, class Compare=Less<T>, class swapFunc=swapCal<T>>
    class ActiveFile
    {
        private:
            int count_;
            int firstIndex_;

            struct fileCache {
                fileCache() 
                    :modifiy_time(0), point(1)
                {

                }
                time_t modifiy_time;
                int point;
            };

            std::array<std::string, MAX_SIZE> arr_{};
            std::unordered_map<std::string, fileCache> arrMap_;
        
        public:
            ActiveFile()
                :count_(0), firstIndex_(1)
            {
            }

            bool CompareFunc(std::string a, std::string b)
            {
                fileCache tmpA = arrMap_[a];
                fileCache tmpB = arrMap_[b];

                if ((tmpA.modifiy_time < tmpB.modifiy_time) && (tmpA.point <= tmpB.point)) {
                    return true;
                } else {
                    return false;
                }
            }


            bool push(std::string path, time_t modifiyTime)
            {
                if (count_ >= MAX_SIZE) {
                    return false;
                }

                if (arrMap_.find(path) != arrMap_.end()) {
                    fileCache tmp = arrMap_[path];
                    tmp.modifiy_time = modifiyTime;
                    tmp.point++;
                    arrMap_[path] = tmp;
                } else {
                    fileCache tmp;
                    tmp.modifiy_time = modifiyTime;
                    arrMap_[path] = tmp;

                    count_++;
                    arr_[count_] = path;
                }

                int i = count_;
                if (i >= MAX_SIZE) return false;

                // 自下而上堆化
                while (i / 2 >= 0 && CompareFunc(arr_[i], arr_[i / 2])) {
                    std::swap(arr_[i], arr_[i / 2]);
                    i = i / 2;
                }

                return true;
            }

            std::string pop()
            {
                if (count_ != 0) {
                    std::string firstStr = arr_[firstIndex_];
                    swap(arr_[firstIndex_], arr_[count_]);
                    --count_;
                    heapify(count_, firstIndex_);

                    arrMap_.erase(firstStr);
                    return firstStr;
                }

                return "";
            }

            // 自上而下堆化
            void heapify(int n, int parent)
            {
                int maxPos, left, right;

                while (true) {
                    maxPos = parent;
                    left = parent * 2;
                    right = parent * 2 + 1;

                    if (left <= n && CompareFunc(arr_[parent], arr_[left])) maxPos = left;
                    if (right <= n && CompareFunc(arr_[maxPos], arr_[right])) maxPos = right;

                    if (maxPos == parent) break;
                    std::swap(arr_[parent], arr_[maxPos]);
                    parent = maxPos;
                }
            }

            std::array<std::string, MAX_SIZE> getArr()
            {
                return arr_;
            }
    };

};

#endif