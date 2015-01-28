/**
* USE_CUSTOM_MAKE_UNIQUE
*
* std::make_unique<T>(Args...) isn't available on C++11 and on some implementations of C++14.
* By setting this flag to 1 you are enabling a 'local' implementation based on N3656
* http://isocpp.org/files/papers/N3656.txt
*/

//#define USE_STANDARD_MAKE_UNIQUE 1

#ifndef USE_STANDARD_MAKE_UNIQUE
    #if _cplusplus <= 201103
        #define USE_CUSTOM_MAKE_UNIQUE 1
    #else
        #define USE_CUSTOM_MAKE_UNIQUE 0
    #endif
#endif
