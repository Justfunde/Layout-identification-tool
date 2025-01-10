
#include "Include/GeometryCharacteristic.hpp"
#include "Include/GeometrySignature.hpp"
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
      if (it->type == lds::GeometryType::path || (it->type == lds::GeometryType::polygon && it->coords.size() > 5))
      {
         ret.push_back(GeometrySignature(it));
      }
   }
   return ret;
}

int
main(int argc, char* argv[])
{

   try
   {
      lds::LayoutData largeLayoutData = InGetLayoutData(L"/home/justfunde/Layout-identification-tool/TestFiles/lut_s44.gds");
      lds::LayoutData cellData = InGetLayoutData(L"/home/justfunde/Layout-identification-tool/TestFiles/sky130_fd_sc_hd_dfxtp_1.gds");

      const auto* cellElement = cellData.libraries[0]->elements[0];
      const auto& layoutElementArr = largeLayoutData.libraries[0]->elements;

      std::cout << "Analyzing cell names with Livenshtein algorithm, n = 3" << std::endl;
      std::stringstream resultSs;
      for (const auto& currElement : layoutElementArr)
      {
         if (CalcLivenshteinDistance(currElement->name, cellElement->name) < 3)
         {
            resultSs << "Possible match '" << cellElement->name << "' with '" << currElement->name << "' in layout" << std::endl;
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
         if (auto res = CalcRelativeDevSquare(currElement, cellElement); res > 0.85)
         {
            resultSs << "Possible match (" << res * 100 << "%) '" << cellElement->name << "' with '" << currElement->name << "' in layout" << std::endl;
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
   }
   catch (const std::exception& e)
   {
      std::cerr << e.what() << '\n';
      return 1;
   }
   return 0;
}