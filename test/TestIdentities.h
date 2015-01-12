#pragma once

#include "TestingFramework.h"

#include "Identity.h"

class TestIdentities : public test::Test {

    BEGIN_TEST(toString)
        using namespace engine;
        Identity id = {(uint64_t) 0xcafebabebeef1337, (uint64_t) 0xbad42690feedf00d};

        std::string strVal = id.toString();

        ASSERT_EQUALS(strVal, std::string("cafebabe-beef-1337-bad4-2690feedf00d"));
    END_TEST

    BEGIN_TEST(relationalOperators)
        using namespace engine;
        Identity id1 = {(uint64_t) 0x01234567890fffff, (uint64_t) 0xedcba987654321};
        Identity id2 = {(uint64_t) 0x0123456789000000, (uint64_t) 0xedcba987654321};

        Identity id3 = {(uint64_t) 0, (uint64_t) 0xfffff987654321};
        Identity id4 = {(uint64_t) 0, (uint64_t) 0xedcba987654321};

        Identity id5 = {(uint64_t) 0, (uint64_t) 0xedcba987654321};

        ASSERT_TRUE(id1 > id2);
        ASSERT_TRUE(id1 >= id2);
        ASSERT_TRUE(id2 < id1);
        ASSERT_TRUE(id2 <= id1);

        ASSERT_TRUE(id3 > id4);
        ASSERT_TRUE(id3 >= id4);
        ASSERT_TRUE(id4 < id3);
        ASSERT_TRUE(id4 <= id3);

        ASSERT_EQUALS(id5, id4);

    END_TEST

    BEGIN_TEST(hashing)
        Identity id1 = {(uint64_t) 0x01234567890fffff, (uint64_t) 0xedcba987654321};
        Identity id2 = {(uint64_t) 0x0123456789000000, (uint64_t) 0xedcba987654321};
        Identity id3 = {(uint64_t) 0, (uint64_t) 0xfffff987654321};

        auto hashFunction = std::hash<Identity>{};

        std::size_t h1 = hashFunction(id1);
        std::size_t h2 = hashFunction(id2);
        std::size_t h3 = hashFunction(id3);

        ASSERT_TRUE(h1 != h2);
        ASSERT_TRUE(h2 != h3);
    END_TEST

    void registerTests() override {
        REGISTER_TEST(toString);
        REGISTER_TEST(relationalOperators);
        REGISTER_TEST(hashing);
    }

};