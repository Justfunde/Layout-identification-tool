#ifndef __GEOMETRY_SIGNATURE_H__
#define __GEOMETRY_SIGNATURE_H__

#include <string>
#include <ostream>

#include "Include/LayoutData.hpp"

class GeometrySignature
{
   public:

   GeometrySignature(lds::Geometry* Geometry);

   GeometrySignature(const GeometrySignature& Sig);

   GeometrySignature(GeometrySignature&& Sig) noexcept;

   GeometrySignature& operator=(GeometrySignature Sig);

   GeometrySignature& operator=(GeometrySignature&& Sig) noexcept;


   operator std::string() const {return sig;}

   const std::string&
   ToString() const { return sig;}

   private:

   std::string
   CreateSignature(lds::Geometry* Geom) const;

   char
   GetHdr(lds::Geometry* Geom) const;


   private:
   std::string sig;
};

std::ostream& operator<<(std::ostream& Out, const GeometrySignature& Sig)
{
   Out << Sig.ToString();
   return Out;
}

#endif //!__GEOMETRY_SIGNATURE_H__