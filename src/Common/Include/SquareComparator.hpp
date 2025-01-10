#ifndef __SQUARE_COMPARATOR_HPP__
#define __SQUARE_COMPARATOR_HPP__

#include "Include/LayoutData.hpp"
#include <cmath>
#include <cstdint>
#include <stdexcept>
#include <utility>

inline double
CalcRelativeDevSquare(
   const lds::Element* Lhs,
   const lds::Element* Rhs)
{
   try
   {
      if (!Lhs || !Rhs)
      {
         throw std::invalid_argument("Bad input function parameter");
      }

      const auto lhsSquare = std::abs(Lhs->max.x - Lhs->min.x) * std::abs(Lhs->max.y - Lhs->min.y);
      const auto rhsSquare = std::abs(Rhs->max.x - Rhs->min.x) * std::abs(Rhs->max.y - Rhs->min.y);
      return static_cast<double>(std::abs(lhsSquare - rhsSquare)) / static_cast<double>(std::max(rhsSquare, lhsSquare));
   }
   catch (const std::exception& e)
   {
      throw;
   }
}

#endif //!__SQUARE_COMPARATOR_HPP__