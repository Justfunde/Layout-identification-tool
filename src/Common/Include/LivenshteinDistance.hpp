#ifndef __LIVENSHTEIN_DISTANCE_HPP__
#define __LIVENSHTEIN_DISTANCE_HPP__

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string_view>
#include <vector>

inline int32_t
CalcLivenshteinDistance(
   std::string_view Lhs,
   std::string_view Rhs)
{
   try
   {
      std::string::size_type lLen = Lhs.length();
      std::string::size_type rLen = Rhs.length();
      std::vector<std::vector<int32_t>> verificationMatrix(lLen + 1, std::vector<int32_t>(rLen + 1)); // 2D array with calculated distances

      // If one of the words has zero length, the distance is equal to the size of the other word.
      if (lLen == 0)
      {
         return rLen;
      }
      if (rLen == 0)
      {
         return lLen;
      }

      // Sets the first row and the first column of the verification matrix with the numerical order from 0 to the length of each word.
      for (std::size_t i = 0; i < lLen; ++i)
      {
         verificationMatrix[i][0] = i;
      }
      for (std::size_t i = 0; i < rLen; ++i)
      {
         verificationMatrix[0][i] = i;
      }

      // Verification step / matrix filling.
      for (std::size_t i = 1; i <= lLen; ++i)
      {
         for (std::size_t j = 1; j <= rLen; ++j)
         {
            // Sets the modification cost.
            // 0 means no modification (i.e. equal letters) and 1 means that a modification is needed (i.e. unequal letters).
            int32_t cost = (Rhs[j - 1] == Lhs[i - 1]) ? 0 : 1;

            // Sets the current position of the matrix as the minimum value between a (deletion), b (insertion) and c (substitution).
            // a = the upper adjacent value plus 1: verif[i - 1][j] + 1
            // b = the left adjacent value plus 1: verif[i][j - 1] + 1
            // c = the upper left adjacent value plus the modification cost: verif[i - 1][j - 1] + cost
            verificationMatrix[i][j] = std::min(
               std::min(verificationMatrix[i - 1][j] + 1, verificationMatrix[i][j - 1] + 1),
               verificationMatrix[i - 1][j - 1] + cost);
         }
      }

      // The last position of the matrix will contain the Levenshtein distance.
      return verificationMatrix[lLen][rLen];
   }
   catch (const std::exception& e)
   {
      std::cerr << e.what() << '\n';
      throw;
   }
}

#endif //!__LIVENSHTEIN_DISTANCE_HPP__