#ifndef GOST_3411_2012_CALC_H
#define GOST_3411_2012_CALC_H

#include <stdint.h>
#include <string.h>
#include <malloc.h>

#define BLOCK_SIZE 64

typedef uint8_t vect[BLOCK_SIZE];

typedef struct GOSTHashContext
{
    vect buffer; ///< Временный буффер для блоков
    vect hash;   ///< Хеш
    vect h;      ///< Промежуточный результат
    vect N;
    vect Sigma;  ///< Контрольная сумма
    vect v_0;    ///< InitVector
    vect v_512;  ///< InitVector
    size_t buf_size; ///< Размер буффера кеширования
    int hash_size;   ///< Размер хеш-суммы (512 или 256)
} TGOSTHashContext;

void
GOSTHashInit(TGOSTHashContext *CTX, uint16_t hash_size);

void
GOSTHashUpdate(TGOSTHashContext *CTX, const uint8_t *data, size_t len);

void
GOSTHashFinal(TGOSTHashContext *CTX);

#endif // GOST_3411_2012_CALC_H
