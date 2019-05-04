#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../FibonacciHeap.h"
#include "TimeReport.h"
#include <queue>

using testing::Eq;


TEST(InsertExtract, FibonacciHeapCorrectnessTests) {
    int q = 1000;
    FibonacciHeap<int> h;

    ASSERT_EQ(h.is_empty(), true);
    for (int i = q - 1; i >= 0; --i) {
        h.insert(i);
    }
    for (int i = 0; i < q; ++i) {
        ASSERT_EQ(h.extract_min(), i);
    }
    ASSERT_EQ(h.is_empty(), true);
}


TEST(InsertExtractRandOrder, FibonacciHeapCorrectnessTests) {
    int q = 1000;
    FibonacciHeap<int> h;
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


TEST(InsertEraseViaDecreaseByPointer, FibonacciHeapCorrectnessTests) {
    srand(1791791791);
    int q = 1000;

    FibonacciHeap<int> h;
    Vector<FibonacciHeap<int>::Pointer> arr;
    for (int i = 0; i < q; ++i) {
        arr.push_back(h.insert(i));
    }
    Vector<bool> alive(arr.size(), true);
    for (int i = 0; i < q - 1; ++i) {
        int id = rand() % (q - i);
        Vector<int> pos;
        for (int j = 0; j < q; ++j) {
            if (alive[j]) {
                pos.push_back(j);
            }
        }
        int index = pos[id];
        h.decrease(arr[index], -1);
        h.extract_min();
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


TEST(InsertEqualNumbers, FibonacciHeapCorrectnessTests) {
    FibonacciHeap<int> h;
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


TEST(MergeHeaps, FibonacciHeapCorrectnessTests) {
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
            FibonacciHeap<int> h1, h2;

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


TEST(GetDecreaseExtract, FibonacciHeapCorrectnessTests) {
    FibonacciHeap<int> h;
    FibonacciHeap<int>::Pointer ptr1 = h.insert(1);
    FibonacciHeap<int>::Pointer ptr2 = h.insert(2);
    FibonacciHeap<int>::Pointer ptr3 = h.insert(3);
    ASSERT_EQ(h.get_min(), 1);
    h.decrease(ptr2, 1);
    ASSERT_EQ(h.get_min(), 1);
    h.decrease(ptr3, -1);
    ASSERT_EQ(h.get_min(), -1);
    h.extract_min();
    ASSERT_EQ(h.get_min(), 1);
    h.extract_min();
    ASSERT_EQ(h.get_min(), 1);
    h.extract_min();
    ASSERT_EQ(h.is_empty(), true);
}


TEST(GetMin, FibonacciHeapValidationTests) {
    FibonacciHeap<int> h;
    ASSERT_THROW(h.get_min(), std::logic_error);
    h.insert(1);
    ASSERT_NO_THROW(h.get_min());
}


TEST(ExtractMin, FibonacciHeapValidationTests) {
    FibonacciHeap<int> h;
    ASSERT_THROW(h.extract_min(), std::logic_error);
    h.insert(1);
    ASSERT_NO_THROW(h.extract_min());
}


TEST(Decrease, FibonacciHeapValidationTests) {
    FibonacciHeap<int> h;
    FibonacciHeap<int>::Pointer ptr = h.insert(1);
    ASSERT_THROW(h.decrease(ptr, 2), std::invalid_argument);
    ASSERT_NO_THROW(h.decrease(ptr, -10));
}


TEST(DISABLED_InsertExtract, FibonacciHeapTimeTests) {
    time_t t0 = clock();

    int q = 5000000;
    FibonacciHeap<int> h;
    for (int i = 0; i < q; ++i) {
        if (!h.is_empty() && !(rand() % 3)) {
            h.extract_min();
        }
        else {
            h.insert(i);
        }
    }
    int res = clock() - t0;

    reportTime("FibonacciHeap inserts and extracts", res);
}

TEST(DISABLED_InsertDecreaseExtract, FibonacciHeapTimeTests) {
    // In this test decreases happen to positive numbers and extracts to neative
    // in order to avoid decreasing on invalidated pointer

    time_t t0 = clock();
    srand(123);

    int q = 50000000;
    FibonacciHeap<long long> h;
    Vector<FibonacciHeap<long long>::Pointer> pointers;
    pointers.push_back(h.insert(1));
    Vector<long long> vals;
    vals.push_back(1ll);
    for (int i = 0; i < q; ++i) {
        if (h.is_empty()) {
            pointers.push_back(h.insert(i));
            vals.push_back(i);
        }
        else if (rand() % 2) {
            pointers.push_back(h.insert(i));
            vals.push_back(i);
        }
        else if (rand() % 2 && h.get_min() < 0) {
            h.extract_min();
        }
        else {
            int pointer_id = rand() % pointers.size();
            if (vals[pointer_id] > 0) {
                FibonacciHeap<long long>::Pointer ptr = pointers[pointer_id];
                h.decrease(ptr, ptr.getKey() - i / 2);
                vals[pointer_id] -= i / 2;
            }
        }
    }
    int res = clock() - t0;

    reportTime("FibonacciHeap inserts, extracts and decreases", res);
}


TEST(DISABLED_InsertExtractDecreaseAtSameElements, FibonacciHeapTimeTests) {
    // In this test decreases happen to positive numbers and extracts to neative
    // in order to avoid decreasing on invalidated pointer

    time_t t0 = clock();
    srand(123);

    int q = 50000000;
    FibonacciHeap<long long> h;
    Vector<FibonacciHeap<long long>::Pointer> pointers;
    pointers.push_back(h.insert(1));
    Vector<long long> vals;
    vals.push_back(1ll);
    for (int i = 0; i < q; ++i) {
        if (h.is_empty() || i < 100) {
            pointers.push_back(h.insert(i));
            vals.push_back(i);
        }
        else if (rand() % 2) {
            pointers.push_back(h.insert(i));
            vals.push_back(i);
        }
        else if (rand() % 2 && h.get_min() < 0) {
            h.extract_min();
        }
        else {
            int pointer_id = (rand() % (pointers.size() / 10)) * 10;
            if (vals[pointer_id] > 0) {
                FibonacciHeap<long long>::Pointer ptr = pointers[pointer_id];
                h.decrease(ptr, ptr.getKey() - i / 2);
                vals[pointer_id] -= i / 2;
            }
        }
    }
    int res = clock() - t0;

    reportTime("FibonacciHeap inserts, extracts and decreases, decreases happen often to same elements", res);
}


TEST(Merge, DISABLED_FibonacciHeapTimeTests) {
    int q = 5000000;
    FibonacciHeap<int> h1, h2;
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

    reportTime("FibonacciHeap merge (5*10^6, 5*10^6)", res);
}
