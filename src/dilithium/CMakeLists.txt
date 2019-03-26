cmake_minimum_required(VERSION 3.2)
project(dilithium)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(NIST_FLAGS "-march=native -mtune=native -O3 -fomit-frame-pointer -fPIC")
set(CMAKE_C_FLAGS "-Wall -Wextra -march=native -mtune=native -O3 -fomit-frame-pointer -DMODE=0 ${NIST_FLAGS}")

set(dilithium_headers
        api.h params.h sign.h polyvec.h packing.h poly.h reduce.h ntt.h rounding.h fips202.h randombytes.h)

set(dilithium_sources
        sign.c polyvec.c packing.c poly.c reduce.c ntt.c rounding.c fips202.c randombytes.c)

add_library(dilithium
        ${dilithium_headers}
        ${dilithium_sources})

