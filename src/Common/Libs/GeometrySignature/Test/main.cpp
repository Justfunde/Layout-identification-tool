#include <gtest/gtest.h>
#include <string>
#include <tuple>

#include "Include/GeometrySignature.hpp"



TEST(LCSkTest, LCSk_NoShifting_Test)
{
   const std::vector<std::tuple<std::string, std::string, std::string>> testCases = {
    std::make_tuple("ABCDEF", "UBCDXY", "BCD"),
    std::make_tuple("123456789", "123", "123"),
    std::make_tuple("XMJYAUZ", "MZJAWXU", "X"),
    std::make_tuple("ABCDEFG", "XABYCDEZG", "CDE"),
    std::make_tuple("1010101", "1011101", "101"),
    std::make_tuple("COMPUTER", "HOUSEBOAT", "O"),
    std::make_tuple("GXTXAYB", "AGGTAB", "G"),
    std::make_tuple("AAAAAA", "AA", "AA"),
    std::make_tuple("AXY", "ADXCPY", "A"),
    std::make_tuple("FTHELOREMIPSUM", "JGXTHEMLOREM", "LOREM"),
    std::make_tuple("CHAINCODE", "CHAIRCODE", "CHAI"),
    std::make_tuple("SIGNATURE", "SIGNATYRE", "SIGNAT"),
    std::make_tuple("CONSTITUTION", "INSTITUTION", "NSTITUTION"),
    // Случаи без совпадений
    std::make_tuple("ABC", "DEF", ""),
    std::make_tuple("123", "456", ""),
    std::make_tuple("XYZ", "PQR", ""),
   };


  for (const auto& [sig1, sig2, expected] : testCases)
    {
        EXPECT_EQ(LCSAlgorithm::LCSk(sig1, sig2), expected);
    }
};

TEST(LCSkTest, LCSk_Shifting_Test)
{
   std::vector<std::tuple<std::string, std::string, std::string>> tests = {
        {"020406", "602040", "020406"},
        {"123456789", "891234567", "123456789"},
        {"abcdefg", "efgabcd", "abcdefg"},
        {"abcdef", "cdefab", "abcdef"},
        {"1234", "3412", "1234"},
        {"aaaa", "aaaa", "aaaa"},
        {"a1b2c3", "1b2c3a", "a1b2c3"},
        {"abcde", "eabcd", "abcde"},
        {"123abc", "abc123", "123abc"},
        {"xyza", "axyz", "xyza"},
        {"mnop", "opmn", "mnop"},
        {"testcase", "casetest", "testcase"},
        {"rotation", "tationro", "rotation"},
        {"signature", "gnaturesi", "signature"},
        {"polygon", "gonpoly", "polygon"},
        {"stringmatch", "tchstringma", "stringmatch"},
        {"substr", "trsubs", "substr"},
        {"shifting", "ftingshi", "shifting"},
        {"cycle", "ecycl", "cycle"},
        {"framework", "workframe", "framework"},
    };

    for (const auto& [sig1, sig2, expected] : tests)
    {
        EXPECT_EQ(LCSAlgorithm::CompareWithLCSkShifting(sig1, sig2), expected);
    }
};




int main(int argc, char** argv)
{
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}