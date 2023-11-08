#ifndef __GEOMETRY_SIGNATURE_H__
#define __GEOMETRY_SIGNATURE_H__

#include <string>
#include <ostream>

#include "Include/LayoutData.hpp"

class LCSAlgorithm
{
   public:
   static
   std::string
   LCSk(
      const std::string& Str1,
      const std::string& Str2);

   static
   std::string
   CompareWithLCSkShifting(
      const std::string& Str1,
      const std::string& Str2);
};


class GeometrySignature
{
   public:

   GeometrySignature(lds::Geometry* Geometry);

   GeometrySignature(const GeometrySignature& Sig);

   GeometrySignature(GeometrySignature&& Sig) noexcept;

   GeometrySignature& operator=(GeometrySignature Sig);

   GeometrySignature& operator=(GeometrySignature&& Sig) noexcept;

   std::string
   FindEntry(
      const GeometrySignature& Signature,
      bool SupportRotations = true,
      bool SupportShift = true);

   bool operator!() const { return sig.empty();}

   operator std::string() const {return sig;}

   const std::string&
   ToString() const { return sig;}

   void
   Rotate(std::int32_t Angle);

   private:

   char
   Rotate(char Direction, std::int32_t Angle) const;

   std::string
   CreateSignature(lds::Geometry* Geom) const;

   char
   GetHdr(lds::Geometry* Geom) const;

   private:
   std::string sig;

};

inline std::ostream& operator<<(std::ostream& Out, const GeometrySignature& Sig)
{
   Out << Sig.ToString();
   return Out;
}

#endif //!__GEOMETRY_SIGNATURE_H__