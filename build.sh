export AFL_CC=clang-6.0

cd /src/ghostscript
CC=/src/afl/afl-clang-fast ./autogen.sh
make libgs -j"$(nproc)"

cd /src/
/src/afl/afl-clang-fast \
    -O2 -o ghostscript_fuzzer \
    ghostscript_fuzzer.c \
    -I ghostscript/psi/ \
    -I ghostscript/base/ \
    ghostscript/bin/gs.a -lm -ldl -pthread
