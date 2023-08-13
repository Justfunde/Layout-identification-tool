#include "Daemon.hpp"

#include <iostream>
#include <stdexcept>
#include <string_view>
#include <filesystem>
#include <fstream>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>


constexpr
std::string_view
g_pidFilePath = "/var/lock/LitDaemon.pid";


Daemon::Daemon()
{

}

void
Daemon::CreatePidFile() const
{
   std::fstream fout(
      std::string(g_pidFilePath.begin(), g_pidFilePath.end()),
      std::fstream::out);

   if(!fout) { throw std::runtime_error(CREATE_EXCEPTION_STR());}

   fout << getpid();
   fout.close();
}

void
Daemon::CreateDaemon()
{
   if(std::filesystem::exists(g_pidFilePath)) { throw std::runtime_error(CREATE_EXCEPTION_STR());}

   #ifndef NDEBUG
   SkeletonDaemon();
   #endif

   CreatePidFile();
}


void
Daemon::SignalHandler(int32_t SigNum)
{
   switch (SigNum)
   {
      case SIGINT:
      case SIGTERM: RemoveDaemon(); break;
      default:break;
   }
}

void
Daemon::RemoveDaemon()
{
   std::filesystem::remove(g_pidFilePath);
}


void
Daemon::SignalHandlerFunction(int32_t SigNum)
{
   Instance().SignalHandler(SigNum);
}


void
Daemon::SkeletonDaemon()
{
   pid_t pid = fork();
   if(pid < 0) { throw std::runtime_error(CREATE_EXCEPTION_STR());}
   else if (pid > 0) { exit(EXIT_SUCCESS); }

   if(setsid() < 0) { throw std::runtime_error(CREATE_EXCEPTION_STR());}
   
   pid = fork();
   if(pid < 0) { throw std::runtime_error(CREATE_EXCEPTION_STR());}
   else if (pid > 0) { exit(EXIT_SUCCESS); }

   umask(777);
   chdir("/");
   
   for(int32_t i = sysconf(_SC_OPEN_MAX); i >= 0; --i)
   {
      close(i);
   }

   std::cout << "Daemon created\n";
}