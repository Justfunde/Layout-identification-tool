#ifndef __I_HASHER_H__
#define __I_HASHER_H__

#include <vector>
#include <mutex>

using HashVector = std::vector<uint8_t>;

class IHasher
{
   public:
   IHasher(std::size_t HashSz) : hash(HashSz, 0) { };

   void Update(const std::string& Str) { Update(Str.data(), Str.size());}
   virtual void Update(const char* Data, std::size_t DataSz) = 0;

   virtual void Final() = 0;

   HashVector GetHash() const { return hash; }

   protected:
   mutable std::mutex m;
   std::vector<uint8_t> hash;
};

#endif //!__I_HASHER_H__