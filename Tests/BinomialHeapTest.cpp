#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../BinomialHeap.h"
#include "../FibonacciHeap.h"
#include "TimeReport.h"
#include <queue>

using testing::Eq;


TEST(InsertExtract, BinomialHeapCorrectnessTests) {
    int q = 1000;
    BinomialHeap<int> h;

    ASSERT_EQ(h.is_empty(), true);
    for (int i = q - 1; i >= 0; --i) {
        h.insert(i);
    }
    for (int i = 0; i < q; ++i) {
        ASSERT_EQ(h.extract_min(), i);
    }
    ASSERT_EQ(h.is_empty(), true);
}


TEST(InsertExtractRandOrder, BinomialHeapCorrectnessTests) {
    int q = 1000;
    BinomialHeap<int> h;
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


TEST(InsertEraseByPointer, BinomialHeapCorrectnessTests) {
    srand(1791791791);
    int q = 1000;

    BinomialHeap<int> h;
    Vector<BinomialHeap<int>::Pointer> arr;
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


TEST(InsertEqualNumbers, BinomialHeapCorrectnessTests) {
    BinomialHeap<int> h;
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


TEST(ChangeMethod, BinomialHeapCorrectnessTests) {
    BinomialHeap<int> h;
    BinomialHeap<int>::Pointer ptr = h.insert(1);
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


TEST(MergeHeaps, BinomialHeapCorrectnessTests) {
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
            BinomialHeap<int> h1, h2;

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


TEST(MergeHeapsStressWithFibonacciHeap, BinomialHeapCorrecnessTests) {
    int q = 1000;
    int TST = 100;
    for (int iter = 0; iter < TST; ++iter) {
        BinomialHeap<int> h1, h2;
        FibonacciHeap<int> f1, f2;
        for (int i = 0; i < q; ++i) {
            if (rand() % 2) {
                int x = rand() % 100;
                h1.insert(x);
                f1.insert(x);
            }
            else {
                int x = rand() % 100;
                h2.insert(x);
                f2.insert(x);
            }
        }
        h1.merge(h2);
        f1.merge(f2);
        ASSERT_EQ(h2.is_empty(), f2.is_empty());
        for (int i = 0; i < q; ++i) {
            if (rand() % 2) {
                int x = rand() % 100;
                h1.insert(x);
                f1.insert(x);
            }
            else {
                ASSERT_EQ(h1.extract_min(), f1.extract_min());
            }
        }
    }
}



TEST(GetMinOnEmptyHeap, BinomialHeapValidationTests) {
    BinomialHeap<int> h;
    ASSERT_THROW(h.get_min(), std::logic_error);
    BinomialHeap<int>::Pointer ptr = h.insert(1337);
    ASSERT_NO_THROW(h.get_min());
    h.erase(ptr);
    ASSERT_THROW(h.get_min(), std::logic_error);
}


TEST(ExtractMinOnEmptyHeap, BinomialHeapValidationTests) {
    BinomialHeap<int> h;
    ASSERT_THROW(h.extract_min(), std::logic_error);
    h.insert(1337);
    BinomialHeap<int>::Pointer ptr = h.insert(1339);
    ASSERT_NO_THROW(h.extract_min());
    h.erase(ptr);
    ASSERT_THROW(h.extract_min(), std::logic_error);
}


TEST(DISABLED_InsertExtract, BinomialHeapTimeTests) {
    time_t t0 = clock();

    int q = 5000000;
    BinomialHeap<int> h;
    for (int i = 0; i < q; ++i) {
        if (!h.is_empty() && !(rand() % 3)) {
            h.extract_min();
        }
        else {
            h.insert(i);
        }
    }

    int res = clock() - t0;

    reportTime("BinomialHeap inserts and extracts", res);
}


TEST(Merge, DISABLED_BinomialHeapTimeTests) {
    int q = 5000000;
    BinomialHeap<int> h1, h2;
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

    reportTime("BinomialHeap merge (5*10^6, 5*10^6)", res);
}
