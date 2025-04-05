#include <gtest/gtest.h>

#include <string>
#include <string_view>
#include <iomanip>

#include <cstring>

#include "Include/Stbg.h"

static void
HashPrint(TGOSTHashContext *CTX)
{
    printf("%d bit hash sum: \n", CTX->hash_size);
    int i;
    if (CTX->hash_size == 256)
        for(i = 32; i < 64; i++)
            printf("%02x", CTX->hash[i]);
    else
        for(i = 0; i < 64; i++)
            printf("%02x", CTX->hash[i]);
    printf("\n");
}

static void
GetHashTest()
{
   GOSTHashContext ctx;
    printf("GOST 34.11-2012 \"Stribog\"\nTest String: "
           "012345678901234567890123456789012345678901234567890123456789012\n");
    GOSTHashInit(&ctx, 512);
    GOSTHashUpdate(&ctx, test_string, sizeof test_string);
    GOSTHashFinal(&ctx);
    HashPrint(&ctx);
    GOSTHashInit(&ctx, 256);
    GOSTHashUpdate(&ctx, test_string, sizeof test_string);
    GOSTHashFinal(&ctx);
    HashPrint(&ctx);
}

int main(int argc, char** argv)
{
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
