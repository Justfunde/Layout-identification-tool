#include "Include/LayoutAnalyzer.hpp"
#include "Include/LayoutReader.hpp"
#include "Include/LivenshteinDistance.hpp"

#include <iostream>

namespace fs = std::filesystem;

LayoutReaderUniquePtr
ILayoutAnalyzer::CreateReader(const std::filesystem::path& Path)
{
   if (!fs::exists(Path))
   {
      throw std::invalid_argument("Path " + Path.string() + " does not exist!");
   }
   if (fs::is_directory(Path))
   {
      throw std::invalid_argument("File argument must be passed!");
   }

   LayoutReaderUniquePtr reader(nullptr, [](LayoutReader* Reader)
   { FreeReader(Reader); });

   reader.reset(GetReader(Path.wstring()));
   if (!reader)
   {
      throw std::runtime_error("Invalid layout file");
   }
   return reader;
}

void
ILayoutAnalyzer::SetMainLayoutFile(const std::filesystem::path& Path)
{
   auto reader = CreateReader(Path);
   reader->Read(&mainData);
}

void
ILayoutAnalyzer::SetLibraryFiles(
   const std::vector<std::filesystem::path> LibraryFiles)
{
   libraryData.clear();
   libraryData.resize(LibraryFiles.size());
   std::size_t i = 0;
   for (const auto& it : LibraryFiles)
   {
      auto reader = CreateReader(it);
      reader->Read(&libraryData[i]);
   }
}

void
ILayoutAnalyzer::SetLibraryDir(const std::filesystem::path& LibraryDir)
{
   if (!fs::is_directory(LibraryDir))
   {
      throw std::invalid_argument("");
   }

   libraryData.clear();
   for (const auto& it : fs::directory_iterator(LibraryDir))
   {
      try
      {
         auto reader = CreateReader(it.path());
         lds::LayoutData data;
         reader->Read(&data);
         libraryData.push_back(data);
      }
      catch (const std::exception& ex)
      {
         std::cerr << ex.what() << std::endl;
      }
   }
}