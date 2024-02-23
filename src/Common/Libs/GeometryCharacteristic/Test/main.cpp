#include <gtest/gtest.h>
#include <string>
#include <tuple>
#include <utility>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <exception>

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


//c TEST(GeometryCharacteristicTest, GeometryCharacteristicTest_1)
//{
//   const std::string fPath = std::string(LAYOUTS_DIR) + "/sky130_fd_sc_hvl__inv_1.gds";
//
//   auto reader = GetReader(std::wstring(fPath.begin(), fPath.end()));
//   lds::LayoutData data;
//   EXPECT_EQ(reader->Read(&data), true);
//
//   InPrintLayoutData(&data);
//
//   auto charsMap = GeometryCharacteristic::Create(data.libraries[0]->elements[0]);
//
//   for(const auto& [key, value] : charsMap)
//   {
//      std::cout << "\n__________________BEGIN_BUNDLE_INFO__________________\n\n";
//      InPrintGeometryInfo(key);
//      InPrintGeometryChar(value);
//      std::cout << "\n__________________END_BUNDLE_INFO__________________\n\n";
//   }
//
//};


static
void
InFlushFileInfo(lds::LayoutData& Data)
{
   const std::wstring outputFileName = std::filesystem::path(Data.fileName).filename().wstring() + L".info";
   const std::filesystem::path outFilePath(std::string(LAYOUTS_DATA_OUT_DIR) + "/" + std::string(outputFileName.begin(),outputFileName.end()));

   const auto& elem = Data.libraries[0]->elements[0];
   const auto& geometries = elem->geometries;

   std::unordered_map<lds::GeometryType, std::string> typeToNameMap =
      {
         {lds::GeometryType::polygon, "polygon"    },
         {lds::GeometryType::path, "path"          },
         {lds::GeometryType::text, "text"          },
         {lds::GeometryType::rectangle, "rectangle"},
         {lds::GeometryType::reference, "reference"}
      };

   auto CalcGeometries = [](const std::vector<lds::Geometry*> Data)
      {
         std::unordered_map<lds::GeometryType, std::size_t> GeometriesCnt =
            {
               {lds::GeometryType::polygon,   0 },
               {lds::GeometryType::path,      0 },
               {lds::GeometryType::text,      0 },
               {lds::GeometryType::rectangle, 0 },
               {lds::GeometryType::reference, 0 }};
         for(const auto it : Data)
         {
            if(it) { GeometriesCnt[it->type]++;}
         }
         return GeometriesCnt;
      };

   auto CalcPoint = [](const std::vector<lds::Geometry*> Data)
      {
         std::unordered_map<lds::GeometryType, std::unordered_map<std::size_t, std::size_t>> pointCntMap;// secont map is <point count, geometry count>
         for(const auto geom : Data)
         {
            if(!geom) { continue;}

            const auto pointCnt = geom->coords.size() - 1;
            if(pointCntMap[geom->type].contains(pointCnt))
            {
               pointCntMap[geom->type][pointCnt]++;
            }
            else
            {
               pointCntMap[geom->type].insert({pointCnt, 0});
            }

         }
         return pointCntMap;
      };

   std::ofstream fileStream(outFilePath.string(), std::ios::trunc);
   if(!fileStream) { throw std::runtime_error("");}

   fileStream << "Element name: " << elem->name << std::endl;
   fileStream << "Element min coordinates: " << boost::format("[%1%, %2%]\n") % elem->min.x % elem->min.y;
   fileStream << "Element max coordinates: " << boost::format("[%1%, %2%]\n") % elem->max.x % elem->max.y;
   fileStream << "Element square: " << (elem->max.x - elem->min.x) * (elem->max.y - elem->min.y);
   fileStream << "Geometries count:\n";
   for(const auto& [type, cnt] : CalcGeometries(geometries))
   {
      fileStream << boost::format("%1% : %2%\n") % typeToNameMap[type] % cnt;
   }

   fileStream << "Geometry to point count distribution" << std::endl;

   //std::unordered_map<lds::GeometryType, std::unordered_map<std::size_t, std::size_t>> pointCntMap // secont map is <point count, geometry count>


   for(const auto& [type, pointCntMap]: CalcPoint(geometries))
   {
      fileStream << boost::format("Type: %1%\nMap of [pointCount, geometryCount]\n{\n") % typeToNameMap[type];
      for(const auto& [pointCnt, geomCnt] : pointCntMap)
      {
         fileStream << boost::format("\t[%1%, %2%]\n") % pointCnt % geomCnt;
      }
      fileStream << "}" << std::endl;
   }
   fileStream.close();

}

TEST(GeometryCharacteristicTest, GeometryExtractTest_1)
{
   //LAYOUTS_DATA_OUT_DIR



   std::filesystem::path outDir = LAYOUTS_DATA_OUT_DIR;
   EXPECT_EQ(std::filesystem::exists(outDir), true);
   EXPECT_EQ(std::filesystem::is_directory(outDir), true);
   for(const auto& dirEntry : std::filesystem::directory_iterator(std::filesystem::path(LAYOUTS_DIR)))
   {
      const auto fileName = dirEntry.path().wstring();
      std::unique_ptr<LayoutReader> reader(GetReader(fileName));
      EXPECT_NE(reader, nullptr);
      lds::LayoutData data;
      EXPECT_EQ(reader->Read(&data), true);
      InFlushFileInfo(data);



   }
}


int main(int argc, char **argv)
{
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}