#ifndef __SQUARE_COMPARATOR_HPP__
#define __SQUARE_COMPARATOR_HPP__

#include "Include/LayoutData.hpp"
#include <cmath>
#include <cstdint>
#include <stdexcept>
#include <utility>

inline double
CalcSquareMatchPercentage(
   const lds::Element* Lhs,
   const lds::Element* Rhs)
{
   try
   {
      if (!Lhs || !Rhs)
      {
         throw std::invalid_argument("Bad input function parameter");
      }

      const double lhsSquare = std::fabs(Lhs->max.x - Lhs->min.x) *
                               std::fabs(Lhs->max.y - Lhs->min.y);
      const double rhsSquare = std::fabs(Rhs->max.x - Rhs->min.x) *
                               std::fabs(Rhs->max.y - Rhs->min.y);

      const double avgSquare = (lhsSquare + rhsSquare) / 2.0;

      // Предотвращаем деление на ноль, если обе площади равны нулю.
      if (avgSquare == 0.0)
         return 100.0;

      const double relativeDeviation = std::fabs(lhsSquare - rhsSquare) / avgSquare * 100.0;
      const double matchPercentage = 1 - relativeDeviation;
      return matchPercentage;
   }
   catch (const std::exception& e)
   {
      throw;
   }
}

#endif //!__SQUARE_COMPARATOR_HPP__