#ifndef __STBG_CPP_H__
#define __STBG_CPP_H__

#include <stdexcept>

#include "Include/Hash/IHasher.hpp"

#include "Stbg.h"

class StbgHasher : public IHasher
{
   public:
   enum Sz
   {
      _256 = 256,
      _512 = 512
   };

   StbgHasher(Sz Size = Sz::_256) : IHasher(Size)
   {
      GOSTHashInit(&ctx, Size);
   }

   virtual void Update(const char* Data, std::size_t DataSz) override final
   {
      if(nullptr == Data) { throw std::invalid_argument("Nullptr data in STGB update!");}
      if(0 == DataSz) { return;}
      GOSTHashUpdate(&ctx, static_cast<const uint8_t*>(Data), DataSz);
   }


   virtual void Final() override final
   {
      GOSTHashFinal(&ctx);
      hash.assign(ctx.hash, ctx.hash + hash.size());
   }

   private:
   GOSTHashContext ctx;
}



#endif //!__STBG_CPP_H__