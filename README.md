# Abelian [![Build Status](https://travis-ci.com/CryptoBLK/Abelian.svg?branch=master)](https://travis-ci.com/CryptoBLK/Abelian)

Copyright (c) 2019 The Abelian Project.  
Portions Copyright (c) 2014-2019 The Monero Project.  
Portions Copyright (c) 2012-2013 The Cryptonote developers.

## Development resources

- Web: [www.abelianfoundation.org](https://www.abelianfoundation.org)
- Mail: [info@abelianfoundation.org](mailto:info@abelianfoundation.org)
- GitLab: [https://gitlab.com/abeliancoin/abelian](https://gitlab.com/abeliancoin/abelian)

## Introduction

Abelian is a cryptocurrency platform, which aims to achieve both privacy and accountability using post-quantum cryptographic techniques. Traditional cryptocurrencies use pseudonyms and achieve limited anonymity as transactions on the cryptocurrency platforms are traceable. Some `privacy coins’ achieve a higher level of anonymity by making transactions on their platforms untraceable. Furthermore, those privacy coins also strive to hide the amount in each transaction. Accountability means that users can choose to allow a third party, such as a regulatory body, to trace their transactions and access information regarding the amount of each transaction, while at the same time, ensure that their transactions remain anonymous and untraceable to the rest of the platform.  

The Abelian platform is developing and adopting well-studied cryptographic algorithms with their security being based on hard problems such as Learning With Errors (LWE), which are as hard to solve as several worst-case lattice problems, and are not known to have any efficient solution in quantum computing. The Abelian research and development group is also building new lattice-based cryptographic primitives for enhancing the efficiency, privacy and security of the Abelian cryptocurrency platform. Led by Dr. Duncan Wong, the R&D group consists of scholars from Shanghai JiaoTong University, Nanyang Technological University, University of Wollongong, and experienced Crypto & Blockchain developers from CryptoBLK.

**Privacy:** In Abelian, proven technologies including linkable ring signature, cryptographic commitment scheme, and zero-knowledge proof are incorporated into the design to offer users uncompromising privacy. Abelian is focusing on providing quantum-resistance and privacy with optional accountability.

**Accountability:** Abelian will apply a cryptographic primitive called verifiable encryption to enable and abide by regulations, such as AML and taxation policies, in the design to solve concerns from enterprises. Abelian aims to develop a post-quantum anonymous digital signature that can support optional accountability. The system allows a user to produce a digital signature that is fully anonymous to the public but can be traced by a designated authority (or a combination of several authorities) chosen by the signer. Different authorities could be designated by a signer for different types of transactions. It allows an authority to passively monitor transactions under its supervision while maintaining user privacy against other players in the system.

**Quantum-safe:** The privacy and accountability features of Abelian are built on Lattice-based cryptography, which enables post-quantum security. When combined with a compatible stealth address mechanism that has been developed in the project, the system will ensure security, anonymity and efficient wallet management for Abelian coin in the post-quantum era.  

More information on the project can be found on the website and in the whitepaper.  

Abelian is an open source project, and we encourage contributions from anyone with something to offer. For more information on contributing, please contact info@abelianfoundation.org

## About this project

This is the core implementation of Abelian. It is open source and completely free to use without restrictions, except for those specified in the license agreement below. There are no restrictions on anyone creating an alternative implementation of Abelian that uses the protocol and network in a compatible manner.

As with many development projects, the repository on GitLab is considered to be the "staging" area for the latest changes. Before changes are merged into that branch on the main repository, they are tested by individual developers in their own branches, submitted as a pull request, and then subsequently tested by contributors who focus on testing and code reviews. That having been said, the repository should be carefully considered before using it in a production environment, unless there is a patch in the repository for a particular show-stopping issue you are experiencing. It is generally a better idea to use a tagged release for stability.

**Anyone is welcome to contribute to Abelian's codebase!** If you have a fix or code change, feel free to submit it as a pull request directly to the "master" branch. In cases where the change is relatively small or does not affect other parts of the codebase it may be merged in immediately by any one of the collaborators. On the other hand, if the change is particularly large or complex, it is expected that it will be discussed at length either well in advance of the pull request being submitted, or even directly on the pull request.

## License

See [LICENSE](LICENSE).

## Contributing

If you want to help out, see [CONTRIBUTING](CONTRIBUTING.md) for a set of guidelines.

## Compiling Abelian from source

### Dependencies

The following table summarizes the tools and libraries required to build. A
few of the libraries are also included in this repository (marked as
"Vendored"). By default, the build uses the library installed on the system,
and ignores the vendored sources. However, if no library is found installed on
the system, then the vendored source will be built and used. The vendored
sources are also used for statically-linked builds because distribution
packages often include only shared library binaries (`.so`) but not static
library archives (`.a`).

| Dep          | Min. version  | Vendored | Debian/Ubuntu pkg  | Arch pkg     | Fedora            | Optional | Purpose        |
| ------------ | ------------- | -------- | ------------------ | ------------ | ----------------- | -------- | -------------- |
| GCC          | 4.7.3         | NO       | `build-essential`  | `base-devel` | `gcc`             | NO       |                |
| CMake        | 3.5           | NO       | `cmake`            | `cmake`      | `cmake`           | NO       |                |
| pkg-config   | any           | NO       | `pkg-config`       | `base-devel` | `pkgconf`         | NO       |                |
| Boost        | 1.58          | NO       | `libboost-all-dev` | `boost`      | `boost-devel`     | NO       | C++ libraries  |
| OpenSSL      | basically any | NO       | `libssl-dev`       | `openssl`    | `openssl-devel`   | NO       | sha256 sum     |
| libzmq       | 3.0.0         | NO       | `libzmq3-dev`      | `zeromq`     | `cppzmq-devel`    | NO       | ZeroMQ library |
| OpenPGM      | ?             | NO       | `libpgm-dev`       | `libpgm`     | `openpgm-devel`   | NO       | For ZeroMQ     |
| libunbound   | 1.4.16        | YES      | `libunbound-dev`   | `unbound`    | `unbound-devel`   | NO       | DNS resolver   |
| libsodium    | ?             | NO       | `libsodium-dev`    | `libsodium`  | `libsodium-devel` | NO       | cryptography   |
| libunwind    | any           | NO       | `libunwind8-dev`   | `libunwind`  | `libunwind-devel` | YES      | Stack traces   |
| liblzma      | any           | NO       | `liblzma-dev`      | `xz`         | `xz-devel`        | YES      | For libunwind  |
| libreadline  | 6.3.0         | NO       | `libreadline6-dev` | `readline`   | `readline-devel`  | YES      | Input editing  |
| ldns         | 1.6.17        | NO       | `libldns-dev`      | `ldns`       | `ldns-devel`      | YES      | SSL toolkit    |
| expat        | 1.1           | NO       | `libexpat1-dev`    | `expat`      | `expat-devel`     | YES      | XML parsing    |
| GTest        | 1.5           | YES      | `libgtest-dev`^    | `gtest`      | `gtest-devel`     | YES      | Test suite     |
| Doxygen      | any           | NO       | `doxygen`          | `doxygen`    | `doxygen`         | YES      | Documentation  |
| Graphviz     | any           | NO       | `graphviz`         | `graphviz`   | `graphviz`        | YES      | Documentation  |


[^] On Debian/Ubuntu `libgtest-dev` only includes sources and headers. You must
build the library binary manually. This can be done with the following command ```sudo apt-get install libgtest-dev && cd /usr/src/gtest && sudo cmake . && sudo make && sudo mv libg* /usr/lib/ ```

Debian / Ubuntu one liner for all dependencies  
``` sudo apt update && sudo apt install build-essential cmake pkg-config libboost-all-dev libssl-dev libzmq3-dev libunbound-dev libsodium-dev libunwind8-dev liblzma-dev libreadline6-dev libldns-dev libexpat1-dev doxygen graphviz libpgm-dev```

### Cloning the repository

Clone recursively to pull-in needed submodule(s):

`$ git clone --recursive https://gitlab.com/abeliancoin/abelian.git`

If you already have a repo cloned, initialize and update:

`$ cd abelian && git submodule init && git submodule update`

*Note*: The command will also download and include the required Dilithium library.  
https://github.com/pq-crystals/dilithium

### Build instructions

Abelian uses the CMake build system and a top-level [Makefile](Makefile) that
invokes cmake commands as needed.

#### On Linux and OS X

* Install the dependencies
* Change to the root of the source code directory, change to the most recent release branch, and build:

        cd abelian
        make

    *Optional*: If your machine has several cores and enough memory, enable
    parallel build by running `make -j<number of threads>` instead of `make`. For
    this to be worthwhile, the machine should have one core and about 2GB of RAM
    available per thread.

    *Note*: If cmake can not find zmq.hpp file on OS X, installing `zmq.hpp` from
    https://github.com/zeromq/cppzmq to `/usr/local/include` should fix that error.
    
    *Note*: The instructions above will compile the most stable release of the
    Abelian software. If you would like to use and test the most recent software,
    use ```git checkout master```. The master branch may contain updates that are
    both unstable and incompatible with release software, though testing is always 
    encouraged. 

* The resulting executables can be found in `build/release/bin`

* Add `PATH="$PATH:$HOME/abelian/build/release/bin"` to `.profile`

* Run Abelian with `abeliand --detach`

* **Optional**: build and run the test suite to verify the binaries:

        make release-test

    *NOTE*: `core_tests` test may take a few hours to complete.

* **Optional**: to build binaries suitable for debugging:

         make debug

* **Optional**: to build statically-linked binaries:

         make release-static

Dependencies need to be built with -fPIC. Static libraries usually aren't, so you may have to build them yourself with -fPIC. Refer to their documentation for how to build them.

* **Optional**: build documentation in `doc/html` (omit `HAVE_DOT=YES` if `graphviz` is not installed):

        HAVE_DOT=YES doxygen Doxyfile

## Running abeliand

The build places the binary in `bin/` sub-directory within the build directory
from which cmake was invoked (repository root by default). To run in
foreground:

    ./bin/abeliand

To list all available options, run `./bin/abeliand --help`.  Options can be
specified either on the command line or in a configuration file passed by the
`--config-file` argument.  To specify an option in the configuration file, add
a line with the syntax `argumentname=value`, where `argumentname` is the name
of the argument without the leading dashes, for example `log-level=1`.

If you're on Mac, you may need to add the `--max-concurrency 1` option to
abelian-wallet-cli, and possibly abeliand, if you get crashes refreshing.

## Debugging

This section contains general instructions for debugging failed installs or problems encountered with Abelian. First ensure you are running the latest version built from the GitLab repo.

### Obtaining stack traces and core dumps on Unix systems

We generally use the tool `gdb` (GNU debugger) to provide stack trace functionality, and `ulimit` to provide core dumps in builds which crash or segfault.

* To use gdb in order to obtain a stack trace for a build that has stalled:

Run the build.

Once it stalls, enter the following command:

```
gdb /path/to/abeliand `pidof abeliand` 
```

Type `thread apply all bt` within gdb in order to obtain the stack trace

* If however the core dumps or segfaults:

Enter `ulimit -c unlimited` on the command line to enable unlimited filesizes for core dumps

Enter `echo core | sudo tee /proc/sys/kernel/core_pattern` to stop cores from being hijacked by other tools

Run the build.

When it terminates with an output along the lines of "Segmentation fault (core dumped)", there should be a core dump file in the same directory as abeliand. It may be named just `core`, or `core.xxxx` with numbers appended.

You can now analyse this core dump with `gdb` as follows:

`gdb /path/to/abeliand /path/to/dumpfile`

Print the stack trace with `bt`

* To run abelian within gdb:

Type `gdb /path/to/abeliand`

Pass command-line options with `--args` followed by the relevant arguments

Type `run` to run abeliand

### Analysing memory corruption

There are two tools available:

* ASAN

Configure Abelian with the -D SANITIZE=ON cmake flag, eg:

    cd build/debug && cmake -D SANITIZE=ON -D CMAKE_BUILD_TYPE=Debug ../..

You can then run the abelian tools normally. Performance will typically halve.

* valgrind

Install valgrind and run as `valgrind /path/to/abeliand`. It will be very slow.

### LMDB

Instructions for debugging suspected blockchain corruption as per @HYC

There is an `mdb_stat` command in the LMDB source that can print statistics about the database but it's not routinely built. This can be built with the following command:

`cd ~/abelian/external/db_drivers/liblmdb && make`

The output of `mdb_stat -ea <path to blockchain dir>` will indicate inconsistencies in the blocks, block_heights and block_info table.

The output of `mdb_dump -s blocks <path to blockchain dir>` and `mdb_dump -s block_info <path to blockchain dir>` is useful for indicating whether blocks and block_info contain the same keys.

These records are dumped as hex data, where the first line is the key and the second line is the data.

