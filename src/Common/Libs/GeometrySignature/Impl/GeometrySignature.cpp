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