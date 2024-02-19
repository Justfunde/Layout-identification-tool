#include <gtest/gtest.h>
#include <string>
#include <tuple>
#include <utility>
#include <iostream>

#include <boost/format.hpp>

#include "Include/GeometryCharacteristic.hpp"
#include "Include/LayoutReader.hpp"

void
InPrintGeometryInfo(lds::Geometry* Geom)
{
      auto GeomType2Text = [](lds::GeometryType t)
      {
         switch (t)
         {
            case lds::GeometryType::polygon: return "polygon";
            case lds::GeometryType::path: return "path";
            case lds::GeometryType::text: return "text";
            case lds::GeometryType::rectangle: return "rectangle";
            case lds::GeometryType::reference: return "reference";
            default: return "undefined";
         }
      };

   std::cout << "Geom type : " << GeomType2Text(Geom->type) << std::endl;
   for(std::size_t i = 0; i < Geom->coords.size(); ++i)
   {
      std::cout << boost::format("Coord[%1%] = [%2%, %3%]\n") % i % Geom->coords[i].x % Geom->coords[i].y;
   }
}

void
InPrintGeometryChar(const GeometryCharacteristic& Chars)
{
   std::cout << boost::format("Mean(x,y) = [%1%, %2%]\n") % Chars.GetMean().first % Chars.GetMean().second;
   std::cout << boost::format("Variance(x,y) = [%1%, %2%]\n") % Chars.GetVariance().first % Chars.GetVariance().second;
   std::cout << boost::format("StdDev(x,y) = [%1%, %2%]\n") % Chars.GetStdDev().first % Chars.GetStdDev().second;
   std::cout << "Covariance = " << Chars.GetCovariance() << std::endl;
   std::cout << "Correlation = " << Chars.GetCorrelation() << std::endl;
}

void
InPrintLayoutData(lds::LayoutData* Data)
{
   const auto pLib = Data->libraries[0];
   const auto pElem = pLib->elements[0];
   std::cout << boost::format("Library name : %1%\n") % pLib->name;
   std::cout << boost::format("Elem name %1%\n") % pElem->name;
   std::cout << boost::format("MIN = [%1%, %2%], MAX = [%3%, %4%]\n") % pElem->min.x % pElem->min.y % pElem->max.x % pElem->max.y;
   /*std::cout << "Geometries info: \n__________________BEGIN__________________\n\n";
   std::size_t i = 0;
   for(const auto it : pElem->geometries)
   {
      std::cout << boost::format("\n__________________BEGIN_INFO_%1%__________________\n") % i++;
      InPrintGeometryInfo(it);
      std::cout << boost::format("\n__________________END_INFO_%1%__________________\n") % i++;
   }
   std::cout << "\n__________________END__________________\n\n";*/
}


TEST(GeometryCharacteristicTest, GeometryCharacteristicTest_1)
{
   const std::string fPath = std::string(LAYOUTS_DIR) + "/sky130_fd_sc_hvl__inv_1.gds";

   auto reader = GetReader(std::wstring(fPath.begin(), fPath.end()));
   lds::LayoutData data;
   EXPECT_EQ(reader->Read(&data), true);

   InPrintLayoutData(&data);

   auto charsMap = GeometryCharacteristic::Create(data.libraries[0]->elements[0]);

   for(const auto& [key, value] : charsMap)
   {
      std::cout << "\n__________________BEGIN_BUNDLE_INFO__________________\n\n";
      InPrintGeometryInfo(key);
      InPrintGeometryChar(value);
      std::cout << "\n__________________END_BUNDLE_INFO__________________\n\n";
   }

};


int main(int argc, char **argv)
{
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}