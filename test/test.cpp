#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "array_sequence.hpp"
#include "list_sequence.hpp"
#include "dynamic_array.hpp"
#include "linked_list.hpp"
#include "user.hpp"

TEST_CASE("DynamicArray operations") {
    SECTION("Construction and comparison") {
        int items[] = {10, 20, 30};
        DynamicArray<int> arr(items, 3);
        DynamicArray<int> expected(items, 3);
        REQUIRE(arr == expected);
    }

    
    SECTION("Set and Resize") {
        int items[] = {5, 10, 15, 20};
        DynamicArray<int> arr(2);
        arr.Set(0, 5);
        arr.Set(1, 10);
        arr.Resize(4);
        arr.Set(2, 15);
        arr.Set(3, 20);
        DynamicArray<int> expected(items, 4);
        REQUIRE(arr == expected);
    }


    SECTION("GetSubArray") {
        int items[] = {1, 2, 3, 4, 5};
        int subItems[] = {2, 3, 4};
        DynamicArray<int> arr(items, 5);
        auto* sub = arr.GetSubArray(1, 3);
        DynamicArray<int> expected(subItems, 3);
        REQUIRE(*sub == expected);
        delete sub;
    }


    SECTION("Remove") {
        int items[] = {1, 2, 3};
        int expectedItems[] = {1, 3};
        DynamicArray<int> arr(items, 3);
        arr.Remove(1);
        DynamicArray<int> expected(expectedItems, 2);
        REQUIRE(arr == expected);
    }
}


TEST_CASE("LinkedList operations") {
    SECTION("Append and Prepend") {
        LinkedList<int> list;
        list.Append(1);
        list.Append(2);
        list.Prepend(0);
        REQUIRE(list.GetLength() == 3);
        REQUIRE(list.GetFirst() == 0);
        REQUIRE(list.GetLast() == 2);
    }


    SECTION("Insert and Remove") {
        LinkedList<int> list;
        list.Append(1);
        list.Append(3);
        list.InsertAt(2, 1);
        list.Remove(0);
        REQUIRE(list.GetLength() == 2);
        REQUIRE(list.GetFirst() == 2);
        REQUIRE(list.GetLast() == 3);
    }


    SECTION("GetSubList") {
        int items[] = {1, 2, 3, 4};
        LinkedList<int> list(items, 4);
        auto* sub = list.GetSubList(1, 2);
        REQUIRE(sub->GetLength() == 2);
        REQUIRE(sub->GetFirst() == 2);
        REQUIRE(sub->GetLast() == 3);
        delete sub;
    }


    SECTION("Concat") {
        int items1[] = {1, 2};
        int items2[] = {3, 4};
        LinkedList<int> list1(items1, 2);
        LinkedList<int> list2(items2, 2);
        auto* combined = list1.Concat(&list2);
        REQUIRE(combined->GetLength() == 4);
        REQUIRE(combined->GetFirst() == 1);
        REQUIRE(combined->GetLast() == 4);
        delete combined;
    }
}


TEST_CASE("ArraySequence operations") {
    SECTION("Construction and AddToEnd") {
        int items[] = {1, 2, 3};
        ArraySequence<int> seq;
        seq.AddToEnd(1);
        seq.AddToEnd(2);
        seq.AddToEnd(3);
        ArraySequence<int> expected(items, 3);
        REQUIRE(seq == expected);
    }


    SECTION("Slice") {
        int items[] = {1, 2, 3, 4};
        int expectedItems[] = {2, 3};
        ArraySequence<int> seq(items, 4);
        auto* sub = seq.Slice(1, 2);
        ArraySequence<int> expected(expectedItems, 2);
        REQUIRE(*sub == expected);
        delete sub;
    }


    SECTION("Combine") {
        int items1[] = {1, 2};
        int items2[] = {3, 4};
        int expectedItems[] = {1, 2, 3, 4};
        ArraySequence<int> seq1(items1, 2);
        ArraySequence<int> seq2(items2, 2);
        auto* combined = seq1.Combine(&seq2);
        ArraySequence<int> expected(expectedItems, 4);
        REQUIRE(*combined == expected);
        delete combined;
    }
}


TEST_CASE("ListSequence operations") {
    SECTION("Basic operations") {
        int items[] = {0, 1, 2};
        ListSequence<int> seq;
        seq.AddToEnd(1);
        seq.AddToEnd(2);
        seq.AddToFront(0);
        ListSequence<int> expected(items, 3);
        REQUIRE(seq == expected);
    }


    SECTION("Insert and Delete") {
        int items[] = {1, 3};
        int expectedItems[] = {2, 3};
        ListSequence<int> seq(items, 2);
        seq.Insert(2, 1);
        seq.Delete(0);
        ListSequence<int> expected(expectedItems, 2);
        REQUIRE(seq == expected);
    }


    SECTION("Slice") {
        int items[] = {1, 2, 3, 4};
        int expectedItems[] = {2, 3};
        ListSequence<int> seq(items, 4);
        auto* sub = seq.Slice(1, 2);
        ListSequence<int> expected(expectedItems, 2);
        REQUIRE(*sub == expected);
        delete sub;
    }

    SECTION("Combine") {
        int items1[] = {1, 2};
        int items2[] = {3, 4};
        int expectedItems[] = {1, 2, 3, 4};
        ListSequence<int> seq1(items1, 2);
        ListSequence<int> seq2(items2, 2);
        auto* combined = seq1.Combine(&seq2);
        ListSequence<int> expected(expectedItems, 4);
        REQUIRE(*combined == expected);
        delete combined;
    }
}

TEST_CASE("User type operations") {
    SECTION("User comparison") {
        User u1("Alice", 25);
        User u2("Alice", 25);
        User u3("Bob", 30);
        REQUIRE(u1 == u2);
        REQUIRE_FALSE(u1 == u3);
    }

    SECTION("User in ListSequence") {
        User u1("Alice", 25);
        User u2("Bob", 30);
        User items[] = {u2, u1};
        ListSequence<User> seq;
        seq.AddToEnd(u1);
        seq.AddToFront(u2);
        ListSequence<User> expected(items, 2);
        REQUIRE(seq == expected);
    }
}
