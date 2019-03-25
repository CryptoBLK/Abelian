cmake_minimum_required(VERSION 3.2)
project(dilithium)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(NIST_FLAGS "-march=native -mtune=native -O3 -fomit-frame-pointer -fPIC")
set(CMAKE_C_FLAGS "-Wall -Wextra -march=native -mtune=native -O3 -fomit-frame-pointer ${NIST_FLAGS}")

set(dilithium_headers
        api.h
        params.h
        sign.h
        polyvec.h
        packing.h
        poly.h
        reduce.h
        ntt.h
        rounding.h
        fips202.h
        fips202x4.h
        randombytes.h)

set(dilithium_sources
        sign.c
        polyvec.c
        packing.c
        poly.c
        reduce.s
        ntt.s
        invntt.s
        pointwise_mul.s
        consts.c
        rounding.c
        fips202.c
        fips202x4.c
        randombytes.c
        keccak4x/KeccakP-1600-times4-SIMD256.o)

add_library(dilithium
        ${dilithium_headers}
        ${dilithium_sources})

