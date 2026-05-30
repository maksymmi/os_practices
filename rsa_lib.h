#ifndef RSA_LIB_H
#define RSA_LIB_H

#include <cstdint>

#if defined(_WIN32)
    #ifdef RSA_EXPORTS
        #define RSA_API __declspec(dllexport)
    #else
        #define RSA_API __declspec(dllimport)
    #endif
#else
    #define RSA_API __attribute__((visibility("default")))
#endif

extern "C" {
    RSA_API uint64_t generate_e(uint64_t phi);
    
    RSA_API uint64_t generate_d(uint64_t e, uint64_t phi);
    
    RSA_API uint64_t rsa_crypt(uint64_t data, uint64_t key, uint64_t n);
}

#endif 