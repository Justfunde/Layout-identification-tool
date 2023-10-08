#include <iostream>

#include "Include/FileMetadataParser.hpp"

FileMetadataParser::FileMetadata
FileMetadataParser::GetFileMetadata(
   const std::wstring& FilePath) noexcept
{
   FileMetadata ret;
   try
   {
      std::filesystem::path path(FilePath);

      if(!std::filesystem::exists(path)) { return ret;}

      ret.absolutePath = std::filesystem::absolute(path);
      ret.sz = std::filesystem::file_size(path);
      ret.permissions = std::filesystem::status(path).permissions();
      ret.hardLinkCnt = std::filesystem::hard_link_count(path);
      ret.lastWriteType = std::filesystem::last_write_time(path);
   }
   catch(const std::exception& e)
   {
      std::cerr << e.what() << std::endl;
      return FileMetadata();
   }
   return ret;
}