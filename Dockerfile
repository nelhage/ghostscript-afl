FROM ubuntu:16.04

RUN apt-get -y update
RUN apt-get -y install clang-6.0 llvm-6.0-dev build-essential git
RUN apt-get -y install autoconf
RUN mkdir -p /src

ADD http://lcamtuf.coredump.cx/afl/releases/afl-latest.tgz /src/
RUN tar -C /src -xzf /src/afl-latest.tgz
RUN ln -nsf /src/afl-*/ /src/afl
RUN make -C /src/afl -j4 && make -C /src/afl/llvm_mode LLVM_CONFIG=llvm-config-6.0 CC=clang-6.0

RUN git clone http://git.ghostscript.com/ghostpdl.git /src/ghostscript
ADD build.sh ghostscript_fuzzer.c /src/
WORKDIR /src/
RUN /src/build.sh
