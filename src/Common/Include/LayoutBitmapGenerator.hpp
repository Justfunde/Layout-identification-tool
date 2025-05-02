#ifndef __LAYOUT_BITMAP_GENERATOR_HPP__
#define __LAYOUT_BITMAP_GENERATOR_HPP__

#include "Include/LayoutData.hpp"
#include <algorithm>
#include <list>
#include <math.h>
#include <set>
#include <stdexcept>

using LayoutBitmap = std::vector<std::vector<uint8_t>>;

class LayoutBitmapGenerator
{
 public:
   LayoutBitmap
   Run(
      const lds::Element* pElement,
      std::size_t rowCount,
      std::size_t columnCount,
      const std::set<int64_t>& layers)
   {
      if (!pElement)
      {
         throw std::invalid_argument("Invalid pElement value");
      }

      // Характеристики
      const lds::Coord& leftBotCoord = pElement->min;
      const lds::Coord& rightTopCoord = pElement->max;
      const lds::Coord leftTopCoord = {leftBotCoord.x, rightTopCoord.y};
      const lds::Coord rightBotCoord = {rightTopCoord.x, leftBotCoord.y};
      const double width = std::fabsl(rightBotCoord.x - leftBotCoord.x);
      const double height = std::fabsl(rightTopCoord.y - leftBotCoord.y);

      LayoutBitmap result;
      result.resize(rowCount);

      for (auto& row : result)
      {
         row.resize(columnCount, 0);
      }

      const double dx = width / columnCount;
      const double dy = height / rowCount;

      std::vector<lds::Geometry*> geometriesToAnalyze;

      std::copy_if(pElement->geometries.begin(), pElement->geometries.end(), std::back_inserter(geometriesToAnalyze), [&layers](lds::Geometry* it)
      { return layers.count(it->layer); });

      for (std::size_t i = 0; i < rowCount; ++i)
      {
         for (std::size_t j = 0; j < columnCount; ++j)
         {
            lds::Coord lhsPoint(leftTopCoord.x + j * dx, leftTopCoord.y - i * dy);
            lds::Coord rhsPoint(rightBotCoord.x, leftTopCoord.y - i * dy);

            for (const lds::Geometry* it : geometriesToAnalyze)
            {

               switch (it->type)
               {
               case lds::GeometryType::rectangle:
               case lds::GeometryType::polygon:
               {

                  std::list<std::pair<lds::Coord, lds::Coord>> verticalLines;

                  // Получение вертикальных отрезков
                  for (std::size_t lIdx = 0, rIdx = 1; rIdx < it->coords.size(); rIdx++, lIdx++)
                  {
                     if (it->coords[lIdx].x == it->coords[rIdx].x)
                     {
                        const double x = it->coords[lIdx].x;

                        lds::Coord maxCoord(x, std::max(it->coords[lIdx].y, it->coords[rIdx].y));
                        lds::Coord minCoord(x, std::min(it->coords[lIdx].y, it->coords[rIdx].y));

                        verticalLines.emplace_back(std::make_pair(maxCoord, minCoord));
                     }
                  }

                  std::size_t intersectionCount = 0;
                  // Подсчет количества пересечений
                  for (const auto& [max, min] : verticalLines)
                  {
                     if (intersect(max, min, lhsPoint, rhsPoint))
                     {
                        ++intersectionCount;
                     }
                  }
                  if (intersectionCount % 2 != 0)
                  {
                     result[i][j] = true;
                  }
               }
               default:
                  continue;
               }
            }
         }
      }
      return result;
   }

 private:
   bool
   ccw(
      lds::Coord A,
      lds::Coord B,
      lds::Coord C)
   {
      return (C.y - A.y) * (B.x - A.x) > (B.y - A.y) * (C.x - A.x);
   }

   bool
   intersect(
      lds::Coord A,
      lds::Coord B,
      lds::Coord C,
      lds::Coord D)
   {
      return ccw(A, C, D) != ccw(B, C, D) && ccw(A, B, C) != ccw(A, B, D);
   }
};

inline double
Compare(
   const LayoutBitmap& lhs,
   const LayoutBitmap& rhs)
{
   if (lhs.size() != rhs.size())
   {
      throw std::invalid_argument("Bitmap must have equal row count");
   }

   for (std::size_t i = 0; i < lhs.size(); ++i)
   {
      if (lhs[i].size() != rhs[i].size())
      {
         throw std::invalid_argument("Bitmap must have equal column count");
      }
   }

   const double total = rhs.size() * lhs.size();
   double equalCount = 0;
   for (std::size_t i = 0; i < lhs.size(); ++i)
   {
      for (std::size_t j = 0; j < rhs.size(); ++j)
      {
         if (lhs[i][j] == rhs[i][j])
         {
            ++equalCount;
         }
      }
   }
   return equalCount / total;
}

#endif //__LAYOUT_BITMAP_GENERATOR_HPP__
