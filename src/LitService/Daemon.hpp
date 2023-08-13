#ifndef __LIT_DAEMON_H__
#define __LIT_DAEMON_H__

#include "Types.hpp"

class Daemon
{
   private:
   Daemon();

   public:

   static
   Daemon& 
   Instance()
   {
      static Daemon d;
      return d;
   }

   void
   RemoveDaemon();

   void
   SignalHandler(int32_t SigNum);

   private:

   void
   CreateDaemon();

   void
   SkeletonDaemon();

   inline
   void
   CreatePidFile() const;


   static
   void
   SignalHandlerFunction(int32_t SigNum);

};


#endif //!__LIT_DAEMON_H__