#include <algorithm>
#include <exception>

#include "Include/GeometrySignature.hpp"

GeometrySignature::GeometrySignature(
   lds::Geometry* Geometry)
   : sig(CreateSignature(Geometry))
{
}

GeometrySignature::GeometrySignature(const GeometrySignature& Sig)
{
   sig = Sig.sig;
}


GeometrySignature::GeometrySignature(GeometrySignature&& Sig) noexcept
{
   sig = std::move(Sig.sig);
}

GeometrySignature& GeometrySignature::operator=(GeometrySignature Sig)
{
   std::swap(sig,Sig.sig);
   return *this;
}


GeometrySignature& GeometrySignature::operator=(GeometrySignature&& Sig) noexcept
{
   sig = std::move(Sig.sig);
   return *this;
}


char
GeometrySignature::GetHdr(lds::Geometry* Geom) const
{
   if(nullptr == Geom) { return 0;}

   switch (Geom->type)
   {
      case lds::GeometryType::polygon: return 0x1;
      case lds::GeometryType::path: return 0x2;
      case lds::GeometryType::rectangle: return 0x3;
      default: return 0;
   }
   return 0;
}


std::string
GeometrySignature::CreateSignature(lds::Geometry* Geom) const
{
   if(nullptr == Geom) { throw std::invalid_argument("");}

   const char hdr = GetHdr(Geom);
   if(0 == hdr) {throw std::invalid_argument(""); }

   /*
      Directions
      7 0 1
      6   2
      5 4 3
   */

   const auto& pointArr = Geom->coords;
   std::string ret;
   ret.reserve(pointArr.size() + 1);
   //ret += hdr;
   for(
      auto it = pointArr.cbegin(), nextIt = std::next(pointArr.cbegin());
      pointArr.cend() != nextIt;
      ++it, ++nextIt)
      {
         if(it->x == nextIt->x)
         {
            if(nextIt->y > it->y){ ret += '0';}
            else {ret += '4';}
         }
         else if(it->y == nextIt->y)
         {
            if(nextIt->x > it->x) { ret += '2';}
            else {ret += '6';}
         }
         else { throw std::runtime_error("");}
      }
   ret.shrink_to_fit();
   return ret;
}


std::string
LCSAlgorithm::LCSk(
   const std::string& Str1,
   const std::string& Str2)
{
   const std::size_t rowCnt = Str1.size();
   const std::size_t colCnt = Str2.size();

   std::vector<std::vector<std::size_t>> dp(rowCnt + 1, std::vector<std::size_t>(colCnt + 1));
   std::size_t maxLength = 0;  // Максимальная длина общей подстроки
   std::size_t endIndex = 0;   // Конечный индекс общей подстроки в str1

   for (std::size_t i = 1; i <= rowCnt; i++)
   {
      for (std::size_t j = 1; j <= colCnt; j++)
      {
         if (Str1[i - 1] == Str2[j - 1])
         {
            dp[i][j] = dp[i - 1][j - 1] + 1;
            if (dp[i][j] > maxLength)
            {
               maxLength = dp[i][j];
               endIndex = i;
            }
            } else { dp[i][j] = 0;} // Сброс, так как LCSk ищет непрерывные последовательности
        }
    }

    // Восстановление наибольшей общей подстроки из таблицы dp
    std::string lcsk = Str1.substr(endIndex - maxLength, maxLength);
    return lcsk;

}


std::string
LCSAlgorithm::CompareWithLCSkShifting(
   const std::string& Str1,
   const std::string& Str2)
{
   if(Str1 == Str2) { return Str1;}

   std::string bestMatch;
   const std::string strToShift = (Str1.size() < Str2.size()) ? Str1 : Str2;
   const std::string sourceStr = (Str1.size() < Str2.size()) ? Str2 : Str1;

   for(std::size_t shift = 0; shift < strToShift.size(); ++shift)
   {
      std::string shiftedStr = strToShift.substr(shift) + strToShift.substr(0, shift);

      std::string currentMatch = LCSk(sourceStr, shiftedStr);
      if(currentMatch.size() > bestMatch.size())
      {
         bestMatch = std::move(currentMatch);
      }
   }
   return bestMatch;
}



void
GeometrySignature::Rotate(int32_t Angle)
{
   if( 0 == Angle) { return;}
   for(auto& it : sig)
   {
      it = Rotate(it, Angle);
   }
}


char
GeometrySignature::Rotate(char Direction, std::int32_t Angle) const
{
   auto currVal = static_cast<int32_t>(Direction) - '0';

   currVal += 360 / Angle;

   return static_cast<char>(currVal) + '0';
}


std::string
GeometrySignature::FindEntry(
   const GeometrySignature& Signature,
   bool SupportRotations,
   bool SupportShift)
{
   if(!Signature) { throw std::invalid_argument(""); }

   constexpr std::size_t rotationsCnt = 360 / 90; // для ортогональной системы координат

   const std::size_t iterationCnt = (SupportRotations) ? rotationsCnt : 1;

   std::string bestMatch;
   for(std::size_t i = 0; i < iterationCnt; ++i)
   {
      GeometrySignature cmpSig = Signature;

      cmpSig.Rotate(i * 90);

      std::string currentMatch = (SupportShift) ? LCSAlgorithm::CompareWithLCSkShifting(sig, cmpSig) : LCSAlgorithm::LCSk(sig, cmpSig);
      if(currentMatch.size() > bestMatch.size())
      {
         bestMatch = std::move(currentMatch);
      }
   }

   return bestMatch;
}