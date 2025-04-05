#ifndef __LAYOUT_ANALYZER_H__
#define __LAYOUT_ANALYZER_H__

#include "LayoutData.hpp"

#include <filesystem>
#include <functional>
#include <vector>
#include <memory>
#include <mutex>
#include <thread>

namespace AnalyzerTask
{
class IAnalyzerTask
{
   protected:

   virtual
   void Run() = 0;

   virtual
   void GetExecutionStatus() const = 0;

   virtual
   void Stop() = 0;

   virtual
   void IsRunnable() const = 0;

   private:

   private:
   std::mutex m;
   std::jthread thread;
};

//class HierarchySearchTask : public IAnalyzerTask
//{
//   public:
//
//   vir
//
//   private:
//}

}


class LayoutReader;
using LayoutReaderUniquePtr = std::unique_ptr<LayoutReader, std::function<void(LayoutReader*)>>;

class ILayoutAnalyzer
{
   public:

   void SetMainLayoutFile(const std::filesystem::path& Path);

   void SetLibraryFiles(const std::vector<std::filesystem::path> LibraryFiles);

   void SetLibraryDir(const std::filesystem::path& LibraryDir);

   private:

   LayoutReaderUniquePtr
   CreateReader(const std::filesystem::path& Path);

   private:
   lds::LayoutData mainData;
   std::vector<lds::LayoutData> libraryData;

};


class FlatLayoutAnalyzer : public ILayoutAnalyzer
{

};


class HierarchyLayoutAnalyzer : public ILayoutAnalyzer
{

};



#endif //!__LAYOUT_ANALYZER_H__