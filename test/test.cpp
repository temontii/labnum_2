#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "array_sequence.hpp"
#include "list_sequence.hpp"
#include "dynamic_array.hpp"
#include "linked_list.hpp"
#include "user.hpp"

TEST_CASE("DynamicArray operations") {
    SECTION("Construction and basic operations") {
        DynamicArray<int> arr(3);
        arr[0] = 10;
        arr[1] = 20;
        arr[2] = 30;

        REQUIRE(arr.GetSize() == 3);
        REQUIRE(arr[0] == 10);
        REQUIRE(arr[1] == 20);
        REQUIRE(arr[2] == 30);
    }
}

TEST_CASE("LinkedList operations") {
    SECTION("Append and prepend") {
        LinkedList<int> list;
        list.Append(1);
        list.Append(2);
        list.Prepend(0);

        REQUIRE(list.GetLength() == 3);
        REQUIRE(list.GetFirst() == 0);
        REQUIRE(list.GetLast() == 2);
    }
}

TEST_CASE("ArraySequence operations") {
    SECTION("Basic sequence operations") {
        ArraySequence<int> seq;
        seq.AddToEnd(1);
        seq.AddToEnd(2);
        seq.AddToFront(0);  // Исправлено с Prepend на AddToFront

        REQUIRE(seq.Size() == 3);
        REQUIRE(seq.Front() == 0);
        REQUIRE(seq.Back() == 2);
    }
}

TEST_CASE("ListSequence operations") {
    SECTION("Sequence concatenation") {
        ListSequence<int> seq1;
        seq1.AddToEnd(1);
        seq1.AddToEnd(2);

        ListSequence<int> seq2;
        seq2.AddToEnd(3);
        seq2.AddToEnd(4);

        auto combined = seq1 + seq2;
        REQUIRE(combined->Size() == 4);
        REQUIRE(combined->At(0) == 1);
        REQUIRE(combined->At(3) == 4);
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
}