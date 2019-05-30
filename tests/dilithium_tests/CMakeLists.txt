include_directories(../../src/dilithium/ref)

set(dilithium_sources
        ../../src/dilithium/ref/sign.c
        ../../src/dilithium/ref/polyvec.c
        ../../src/dilithium/ref/poly.c
        ../../src/dilithium/ref/packing.c
        ../../src/dilithium/ref/ntt.c
        ../../src/dilithium/ref/reduce.c
        ../../src/dilithium/ref/rounding.c
        ../../src/dilithium/ref/fips202.c)

set(dilithium_headers
        ../../src/dilithium/ref/config.h
        ../../src/dilithium/ref/api.h
        ../../src/dilithium/ref/params.h
        ../../src/dilithium/ref/sign.h
        ../../src/dilithium/ref/polyvec.h
        ../../src/dilithium/ref/poly.h
        ../../src/dilithium/ref/packing.h
        ../../src/dilithium/ref/ntt.h
        ../../src/dilithium/ref/reduce.h
        ../../src/dilithium/ref/rounding.h
        ../../src/dilithium/ref/symmetric.h)

set(dilithium_tests_sources
        cpucycles.c
        speed.c
        ${dilithium_sources})

set(dilithium_tests_headers
        cpucycles.h
        speed.h
        ${dilithium_tests_headers})

# test_dilithium
add_executable(dilithium_test
        test_dilithium.c
        ../../src/dilithium/ref/randombytes.c
        ../../src/dilithium/ref/randombytes.h
        ${dilithium_tests_sources}
        ${dilithium_tests_headers})

target_link_libraries(dilithium_test
        PRIVATE
            dilithium
            OpenSSL::Crypto
            ${EXTRA_LIBRARIES})

set_property(TARGET dilithium_test
        PROPERTY
        FOLDER "tests")

add_test(
        NAME dilithium_test
        COMMAND dilithium_test
)

# test_mul
add_executable(dilithium_test_mul
        test_mul.c
        ../../src/dilithium/ref/randombytes.c
        ../../src/dilithium/ref/randombytes.h
        ${dilithium_tests_sources}
        ${dilithium_tests_sources})

target_link_libraries(dilithium_test_mul
        PRIVATE
            dilithium
            OpenSSL::Crypto
            ${EXTRA_LIBRARIES})

set_property(TARGET dilithium_test_mul
        PROPERTY
        FOLDER "tests")

add_test(
        NAME    dilithium_test_mul
        COMMAND dilithium_test_mul)

# test_vectors
add_executable(dilithium_test_vector
        test_vectors.c
        ../../src/dilithium/ref/rng.c
        ../../src/dilithium/ref/rng.h
        ${dilithium_tests_sources}
        ${dilithium_tests_sources})

target_link_libraries(dilithium_test_vector
        PRIVATE
        dilithium
        OpenSSL::Crypto
        ${EXTRA_LIBRARIES})

set_property(TARGET dilithium_test_vector
        PROPERTY
        FOLDER "tests")

add_test(
        NAME    dilithium_test_vector
        COMMAND dilithium_test_vector)