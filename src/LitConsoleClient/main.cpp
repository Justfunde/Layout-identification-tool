
#include "Include/GeometryCharacteristic.hpp"
#include "Include/GeometrySignature.hpp"
#include "Include/LayoutBitmapGenerator.hpp"
#include "Include/LayoutReader.hpp"
#include "Include/LivenshteinDistance.hpp"
#include "Include/SquareComparator.hpp"
#include <filesystem>
#include <format>
#include <iostream>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>

static lds::LayoutData
InGetLayoutData(
   const std::wstring& layoutPath)
{
   std::unique_ptr<LayoutReader, decltype(&FreeReader)> reader(GetReader(layoutPath), FreeReader);
   lds::LayoutData data;
   if (!reader->Read(&data))
   {
      throw std::runtime_error("Не удалось прочитать файл топологии " + std::string(layoutPath.begin(), layoutPath.end()));
   }
   return data; // will be moved as nrvo
}

static std::vector<GeometrySignature>
InCreateSignatures(const lds::Element* elem)
{
   std::vector<GeometrySignature> ret;
   for (const auto it : elem->geometries)
   {
      if (it->type == lds::GeometryType::path || (it->type == lds::GeometryType::polygon && it->coords.size() > 10))
      {
         ret.push_back(GeometrySignature(it));
      }
   }
   return ret;
}

void
Print(const LayoutBitmap& bitmap)
{
   for (std::size_t i = 0; i < bitmap.size(); ++i)
   {
      for (std::size_t j = 0; j < bitmap.size(); ++j)
      {
         std::cout << static_cast<int32_t>(bitmap[i][j]);
      }
      std::cout << std::endl;
   }
}

int
main(int argc, char* argv[])
{

   try
   {
      lds::LayoutData largeLayoutData = InGetLayoutData(L"/home/justfunde/Work/MIET/Layout-identification-tool/TestFiles/aes.gds");
      lds::LayoutData cellData = InGetLayoutData(L"/home/justfunde/Work/MIET/Layout-identification-tool/TestFiles/sky130_fd_sc_hd_dfxtp_1.gds");
      std::set<const lds::Element*> elementsForDetailedAnalysis;

      const auto* cellElement = cellData.libraries[0]->elements[0];
      const auto& layoutElementArr = largeLayoutData.libraries[0]->elements;

      std::cout << "Analyzing cell names with Livenshtein algorithm, n = 3" << std::endl;
      std::stringstream resultSs;
      for (const auto& currElement : layoutElementArr)
      {
         if (CalcLivenshteinDistance(currElement->name, cellElement->name) < 3)
         {
            resultSs << "Possible match '" << cellElement->name << "' with '" << currElement->name << "' in layout" << std::endl;
            elementsForDetailedAnalysis.emplace(currElement);
         }
      }

      if (std::string result = resultSs.str(); !result.empty())
      {
         std::cout << result;
      }
      else
      {
         std::cout << "No issues were found" << std::endl;
      }
      resultSs.str("");

      std::cout << "Analyzing cell with calculation of relative deviation, > 85%" << std::endl;

      for (const auto& currElement : layoutElementArr)
      {
         if (std::set<std::string>({"sky130_fd_sc_hd__dfxtp_1", "sky130_fd_sc_hd__fill_1", "sky130_fd_sc_hd__dfxtp_1", "lut_s44"}).count(currElement->name))
         {
            int abc = 5;
         }
         if (auto res = CalcSquareMatchPercentage(currElement, cellElement); res > 0.85)
         {
            resultSs << "Possible match (" << res * 100 << "%) '" << cellElement->name << "' with '" << currElement->name << "' in layout" << std::endl;
            elementsForDetailedAnalysis.emplace(currElement);
         }
      }

      if (std::string result = resultSs.str(); !result.empty())
      {
         std::cout << result;
      }
      else
      {
         std::cout << "No issues were found" << std::endl;
      }
      resultSs.str("");

      std::cout << "Analyzing cell with geometries count, < 10% difference" << std::endl;

      for (const auto& currElement : layoutElementArr)
      {
         const std::size_t rhsCount = currElement->geometries.size();
         const std::size_t lhsCount = cellElement->geometries.size();
         const double max = rhsCount > lhsCount ? rhsCount : lhsCount;
         const double min = rhsCount > lhsCount ? lhsCount : rhsCount;
         const double res = (max - min) / min;
         if (res < 0.01)
         {
            resultSs << "Possible match (difference " << res * 100 << "%) '" << cellElement->name << "' with '" << currElement->name << "' in layout" << std::endl;
            elementsForDetailedAnalysis.emplace(currElement);
         }
      }
      if (std::string result = resultSs.str(); !result.empty())
      {
         std::cout << result;
      }
      else
      {
         std::cout << "No issues were found" << std::endl;
      }
      resultSs.str("");

      std::cout << "Analyzing distribution of polygon points in cells" << std::endl;

      const auto cellSignatures = InCreateSignatures(cellElement);
      for (const auto& currElement : layoutElementArr)
      {
         std::size_t eqCnt = 0;
         const auto currElementSignatures = InCreateSignatures(currElement);
         for (const auto& cellSig : cellSignatures)
         {
            for (const auto& elemSig : currElementSignatures)
            {
               const std::size_t elemSigLen = elemSig.ToString().length();
               const std::size_t cellSigLen = cellSig.ToString().length();
               const std::size_t minRep = std::max(std::size_t(7), std::min(elemSigLen, cellSigLen));

               if (cellSig.FindEntry(elemSig, true, true).length() >= minRep)
               {
                  ++eqCnt;
               }
            }
         }
         if (eqCnt >= static_cast<double>(std::max(cellSignatures.size(), currElementSignatures.size()) * 0.85))
         {
            resultSs << "Possible match (" << eqCnt << "equal signatures)'" << cellElement->name << "' with '" << currElement->name << "' in layout" << std::endl;
            elementsForDetailedAnalysis.emplace(currElement);
         }
      }
      if (std::string result = resultSs.str(); !result.empty())
      {
         std::cout << result;
      }
      else
      {
         std::cout << "No issues were found" << std::endl;
      }
      resultSs.str("");

      // Calc signatures of cell which we want to find

      std::cout << "Analyzing stat characteristics of geometries in cells" << std::endl;

      const auto cellGeometryCharacteristicMap = GeometryCharacteristic::Create(cellElement, 5);

      for (const auto& it : layoutElementArr)
      {
         std::size_t eqCnt = 0;
         const auto itGeometryCharacteristicMap = GeometryCharacteristic::Create(it, 5);
         for (const auto& [itGeometry, itGeometryCharacteristic] : itGeometryCharacteristicMap)
         {
            for (const auto& [cellGeometry, cellGeometryCharacteristic] : cellGeometryCharacteristicMap)
            {
               if (AreCharacteristicsSimilar(cellGeometryCharacteristic, itGeometryCharacteristic, ComputeThresholdFromPerimeter(cellGeometry->coords)) ||
                   AreCharacteristicsSimilar(cellGeometryCharacteristic, itGeometryCharacteristic, ComputeThresholdFromPerimeter(itGeometry->coords)))
               {
                  eqCnt++;
               }
            }
         }
         if (eqCnt >= static_cast<double>(std::max(cellGeometryCharacteristicMap.size(), itGeometryCharacteristicMap.size()) * 0.85))
         {
            resultSs << "Possible match (" << eqCnt << "equal geometry characteristics)'" << cellElement->name << "' with '" << it->name << "' in layout" << std::endl;
            elementsForDetailedAnalysis.emplace(it);
         }
      }

      if (std::string result = resultSs.str(); !result.empty())
      {
         std::cout << result;
      }
      else
      {
         std::cout << "No issues were found" << std::endl;
      }
      // return 0;
      std::cout << "----------------------DetailedAnalysis----------------------\n";

      LayoutBitmapGenerator bitmapGenerator;
      std::unordered_map<int64_t, LayoutBitmap> layerToMapBitmap;
      std::set<int64_t> cellLayers;

      constexpr std::size_t bitmapSz = 64;

      for (const auto* it : cellElement->geometries)
      {
         cellLayers.emplace(it->layer);
      }

      for (const auto& it : cellLayers)
      {
         layerToMapBitmap[it] = bitmapGenerator.Run(cellElement, bitmapSz, bitmapSz, {it});
      }

      for (const auto& currentElement : elementsForDetailedAnalysis)
      {
         std::cout << "Analyzing " << currentElement->name << "...\n";

         double totalChance = 0;

         for (const auto& [layer, bitmap] : layerToMapBitmap)
         {
            LayoutBitmap currentElementBitmap = bitmapGenerator.Run(currentElement, bitmapSz, bitmapSz, {layer});
            if (currentElement->name == "sky130_fd_sc_hd__dfxtp_1")
            {
               std::cout << "CellBitmap:\n";
               // Print(bitmap);
               //
               // std::cout << "ElemBitmap:\n";
               // Print(currentElementBitmap);
               // std::cout << std::endl
               //          << std::endl
               //          << std::endl
               //          << std::endl
               //;//
            }

            double currentChance = Compare(currentElementBitmap, bitmap);
            std::cout << "Compability chance on layer " << layer << ": " << currentChance * 100 << "\n";
            totalChance += currentChance;
         }

         std::cout << "Total chance: " << (totalChance / layerToMapBitmap.size()) * 100 << "\n--------------------------------------------------------------------\n";
      }

      // auto bitmap = gen.Run(cellElement, 128, 128, {95});
      // Print(bitmap);
      // return 0;
   }
   catch (const std::exception& e)
   {
      std::cerr << e.what() << '\n';
      return 1;
   }
   return 0;
}
