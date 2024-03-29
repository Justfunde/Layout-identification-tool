/**
 * @file Crc32.h
 * @author Mikhail Kotlyarov  (m.kotlyarov@elvis.ru)
 * @brief Crc32 hash function declaration
 * @version 0.1
 * @date 2023-02-08
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __CRC_32_H__
#define __CRC_32_H__


#include <stdint.h>

typedef uint32_t Crc32Hash;

/**
* @brief Function to calculate crc32 hash. LittleEndian only.
*
* @param Data Data to calc hash
* @param DataSz Data size
* @return Crc32Hash
*/
Crc32Hash
Crc32CalcHash(
   const char* Data,
   uint32_t DataSz);

#endif//!__CRC_32_H__