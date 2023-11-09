#include <gtest/gtest.h>
#include <string>
#include <tuple>
#include <utility>

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

   for (const auto &[sig1, sig2, expected] : testCases)
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

   for (const auto &[sig1, sig2, expected] : tests)
   {
      EXPECT_EQ(LCSAlgorithm::CompareWithLCSkShifting(sig1, sig2), expected);
   }
};

TEST(GeometrySignature, GeometrySignatureCreationTest)
{
   /*
      Directions
      7 0 1
      6   2
      5 4 3
   */
   //Box

   auto box = std::make_unique<lds::Rectangle>();
   std::vector<lds::Coord> coords =
   {
      {0, 0},
      {10, 0},
      {10, 10},
      {0, 10},
      {0, 0}
   };
   box->coords = std::move(coords);

   const std::string etalonBoxSig = "2064";

   EXPECT_EQ(etalonBoxSig, GeometrySignature(box.get()).ToString());

   auto poly = std::make_unique<lds::Polygon>();
   coords = {
    {0, 0},    // Точка 1
    {10, 0},   // Точка 2, вправо от Точки 1
    {10, 5},   // Точка 3, вверх от Точки 2
    {5, 5},    // Точка 4, влево от Точки 3
    {5, 10},   // Точка 5, вверх от Точки 4
    {15, 10},  // Точка 6, вправо от Точки 5
    {15, -5},  // Точка 7, вниз от Точки 6
    {0, -5},   // Точка 8, влево от Точки 7
    {0, -10},  // Точка 9, вниз от Точки 8
    {0, 0},    // Точка 10, вверх до Точки 1 (замыкает полигон)
};
poly->coords = std::move(coords);

const std::string etalonPolySig = "206024640";

EXPECT_EQ(etalonPolySig, GeometrySignature(poly.get()).ToString());

};


TEST(GeometrySignature, GeometrySignatureComparationTest)
{
   /*
      Directions
      7 0 1
      6   2
      5 4 3
   */
   //Box

   auto boxes = std::make_pair(std::make_unique<lds::Rectangle>(),std::make_unique<lds::Rectangle>());
   boxes.first->coords = {
      {10, 0},
      {10, 10},
      {0, 10},
      {0, 0},
      {10, 0}};

   boxes.second->coords = {
      {0, 0},
      {10, 0},
      {10, 10},
      {0, 10},
      {0, 0}};

   EXPECT_EQ(GeometrySignature(boxes.first.get()).FindEntry(GeometrySignature(boxes.second.get())).length(), 4);

   auto polygons = std::make_pair(std::make_unique<lds::Rectangle>(),std::make_unique<lds::Rectangle>());
   polygons.first->coords = {
      {0, 0},    // Точка 1
    {10, 0},   // вправо от Точки 1
    {10, 5},   // вверх от Точки 2
    {5, 5},    // влево от Точки 3
    {5, 10},   // вверх от Точки 4
    {15, 10},  // вправо от Точки 5
    {15, -5},  // вниз от Точки 6
    {0, -5},   // влево от Точки 7
    {0, -10},  // вниз от Точки 8
    {0, 0}};    //вверх до Точки 1 (замыкает полигон)};

   polygons.second->coords = {
     {0, 0},    // Начальная точка
    {0, -10},  // Вниз от Точки 1 к Точке 2
    {5, -10},  // Вправо от Точки 2 к Точке 3
    {5, -5},   // Вверх от Точки 3 к Точке 4
    {10, -5},  // Вправо от Точки 4 к Точке 5
    {10, -15}, // Вниз от Точки 5 к Точке 6
    {-5, -15}, // Влево от Точки 6 к Точке 7
    {-5, 0},   // Вверх от Точки 7 к Точке 8
    {-10, 0},  // Влево от Точки 8 к Точке 9
    {0, 0}    // Вверх от Точки 9 к Точке 10 (замыкает полигон)
   };

   EXPECT_EQ(GeometrySignature(polygons.first.get()).FindEntry(GeometrySignature(polygons.second.get())).length(), 4);

};


int main(int argc, char **argv)
{
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}