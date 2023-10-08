#ifndef __FILE_METADATA_PARSER_HPP__
#define __FILE_METADATA_PARSER_HPP__

#include <string>
#include <filesystem>

class FileMetadataParser
{
   public:
   struct FileMetadata
   {
      std::filesystem::path           absolutePath;
      std::uintmax_t                  sz = 0;
      std::uintmax_t                  hardLinkCnt = 0;
      std::filesystem::file_time_type lastWriteType;
      std::filesystem::perms          permissions;
   };

   static
   FileMetadata
   GetFileMetadata(
      const std::wstring& FilePath) noexcept;
};
#endif //!__FILE_METADATA_PARSER_HPP__