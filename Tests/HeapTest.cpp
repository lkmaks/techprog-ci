#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fstream>
#include "../Heap.h"
#include "TimeReport.h"
#include <queue>

using testing::Eq;


TEST(InsertExtract, HeapCorrectnessTests) {
    int q = 1000;
    Heap<int> h;

    ASSERT_EQ(h.is_empty(), true);
    for (int i = q - 1; i >= 0; --i) {
        h.insert(i);
    }
    for (int i = 0; i < q; ++i) {
        ASSERT_EQ(h.extract_min(), i);
    }
    ASSERT_EQ(h.is_empty(), true);
}


TEST(InsertExtractRandOrder, HeapCorrectnessTests) {
    int q = 1000;
    Heap<int> h;
    std::priority_queue<int> h2;

    srand(139);
    for (int i = 0; i < q; ++i) {
        if (rand() % 3) {
            int x = rand() % 100;
            h.insert(x);
            h2.push(-x);
        }
        else if (!h.is_empty()) {
            ASSERT_EQ(h.extract_min(), -h2.top());
            h2.pop();
        }
    }
}


TEST(InsertEraseByPointer, HeapCorrectnessTests) {
    int q = 1000;
    Heap<int> h;
    Vector<Heap<int>::Pointer> arr;
    for (int i = 0; i < q; ++i) {
        arr.push_back(h.insert(i));
    }
    Vector<bool> alive(arr.size(), true);
    srand(1791791791);
    for (int i = 0; i < q - 1; ++i) {
        int id = rand() % (q - i);
        Vector<int> pos;
        for (int j = 0; j < q; ++j) {
            if (alive[j]) {
                pos.push_back(j);
            }
        }
        int index = pos[id];
        h.erase(arr[index]);
        alive[index] = false;

        for (int j = id; j + 1 < pos.size(); ++j) {
            pos[j] = pos[j + 1];
        }
        pos.pop_back();

        ASSERT_EQ(arr[pos[0]].getKey(), h.get_min());
        ASSERT_EQ(h.is_empty(), false);
    }
    h.extract_min();
    ASSERT_EQ(h.is_empty(), true);
}


TEST(InsertEqualNumbers, HeapCorrectnessTests) {
    Heap<int> h;
    h.insert(1);
    h.insert(1);
    h.insert(2);
    h.insert(1);
    ASSERT_EQ(h.extract_min(), 1);
    ASSERT_EQ(h.extract_min(), 1);
    ASSERT_EQ(h.extract_min(), 1);
    ASSERT_EQ(h.extract_min(), 2);
    ASSERT_EQ(h.is_empty(), true);
}


TEST(ChangeMethod, HeapCorrectnessTests) {
    Heap<int> h;
    Heap<int>::Pointer ptr = h.insert(1);
    h.insert(2);
    h.insert(3);
    ASSERT_EQ(h.get_min(), 1);
    h.change(ptr, 4);
    ASSERT_EQ(h.get_min(), 2);
    h.extract_min();
    h.extract_min();
    ASSERT_EQ(h.extract_min(), 4);
    ASSERT_EQ(h.is_empty(), true);
}


TEST(IteratorConstructor, HeapCorrectnessTests) {
    int q = 1000;
    int *a = new int[q];
    for (int i = 0; i < q; ++i) {
        a[i] = i;
    }
    Heap<int> h(a, a + q);
    for (int i = 0; i < q; ++i) {
        ASSERT_EQ(h.extract_min(), i);
    }
    ASSERT_EQ(h.is_empty(), true);
}


TEST(MergeHeaps, HeapCorrectnessTests) {
    srand(34234);
    Vector<int> qs;
    qs.push_back(1);
    qs.push_back(2);
    qs.push_back(3);
    qs.push_back(4);
    qs.push_back(5);
    qs.push_back(100);

    for (int ic = 0; ic < qs.size(); ++ic) {
        int q = qs[ic];
        for (int j = 0; j < 10; ++j) {
            Heap<int> h1, h2;

            for (int i = 0; i < q; ++i) {
                if (rand() % 2) {
                    h1.insert(i);
                }
                else {
                    h2.insert(i);
                }
            }
            h1.merge(h2);
            ASSERT_EQ(h2.is_empty(), true);
            for (int i = 0; i < q; ++i) {
                ASSERT_EQ(h1.extract_min(), i);
            }
        }
    }
}


TEST(GetMinOnEmptyHeap, HeapValidationTests) {
    Heap<int> h;
    ASSERT_THROW(h.get_min(), std::logic_error);
    Heap<int>::Pointer ptr = h.insert(1337);
    ASSERT_NO_THROW(h.get_min());
    h.erase(ptr);
    ASSERT_THROW(h.get_min(), std::logic_error);
}


TEST(ExtractMinOnEmptyHeap, HeapValidationTests) {
    Heap<int> h;
    ASSERT_THROW(h.extract_min(), std::logic_error);
    h.insert(1337);
    Heap<int>::Pointer ptr = h.insert(1339);
    ASSERT_NO_THROW(h.extract_min());
    h.erase(ptr);
    ASSERT_THROW(h.extract_min(), std::logic_error);
}


TEST(InsertExtract, DISABLED_HeapTimeTests) {
    time_t t0 = clock();

    int q = 5000000;
    Heap<int> h;
    for (int i = 0; i < q; ++i) {
        if (!h.is_empty() && !(rand() % 3)) {
            h.extract_min();
        }
        else {
            h.insert(i);
        }
    }

    int res = clock() - t0;

    reportTime("Heap inserts and extracts", res);
}


TEST(Merge, DISABLED_HeapTimeTests) {
    // merge is reproduced with inserts from one heap into another
    // second heap is not cleared, but it can be done in O(1), clearing Vector of nodes

    int q = 5000000;
    Heap<int> h1, h2;
    srand(239);
    for (int i = 0; i < q; ++i) {
        h1.insert(rand());
    }
    for (int i = 0; i < q; ++i) {
        h2.insert(rand());
    }

    time_t t0 = clock();
    h1.merge(h2);
    int res = clock() - t0;

    reportTime("Heap 'merge' (5*10^6, 5*10^6)", res);
}
