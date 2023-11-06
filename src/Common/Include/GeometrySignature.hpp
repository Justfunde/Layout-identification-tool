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

   bool
   FindEntry(
      const GeometrySignature& Signature,
      bool SupportRotations = true,
      bool SupportShift = true);

   bool operator!() const { return sig.empty();}

   operator std::string() const {return sig;}

   const std::string&
   ToString() const { return sig;}

   private:

   std::string
   CreateSignature(lds::Geometry* Geom) const;

   char
   GetHdr(lds::Geometry* Geom) const;

   std::string
   LCSk(
      const std::string& Str1,
      const std::string& Str2);

   std::string
   CompareWithLCSk(
      const std::string& Str1,
      const std::string& Str2);


   private:
   std::string sig;
};

std::ostream& operator<<(std::ostream& Out, const GeometrySignature& Sig)
{
   Out << Sig.ToString();
   return Out;
}

#endif //!__GEOMETRY_SIGNATURE_H__