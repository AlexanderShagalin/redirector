FROM ubuntu:24.04

ARG BOOST_VERSION=1.83.0
ARG CMAKE_VERSION=3.28.3
ARG NUM_JOBS=8

ENV DEBIAN_FRONTEND=noninteractive

# Install package dependencies
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        build-essential \
        software-properties-common \
        autoconf \
        automake \
        libtool \
        pkg-config \
        ca-certificates \
        libssl-dev \
	nlohmann-json3-dev \
        wget \
        git \
        curl \
        language-pack-en \
        locales \
        locales-all \
        vim \
        gdb \
        valgrind && \
    apt-get clean

# System locale
# Important for UTF-8
ENV LC_ALL=en_US.UTF-8
ENV LANG=en_US.UTF-8
ENV LANGUAGE=en_US.UTF-8

# Install CMake
RUN cd /tmp && \
    wget https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}.tar.gz && \
    tar xzf cmake-${CMAKE_VERSION}.tar.gz && \
    cd cmake-${CMAKE_VERSION} && \
    ./bootstrap && \
    make -j${NUM_JOBS} && \
    make install && \
    rm -rf /tmp/*

# Install Boost
# https://www.boost.org/doc/libs/1_80_0/more/getting_started/unix-variants.html
RUN cd /tmp && \
    BOOST_VERSION_MOD=$(echo $BOOST_VERSION | tr . _) && \
    wget https://archives.boost.io/release/${BOOST_VERSION}/source/boost_${BOOST_VERSION_MOD}.tar.bz2 && \
    tar --bzip2 -xf boost_${BOOST_VERSION_MOD}.tar.bz2 && \
    cd boost_${BOOST_VERSION_MOD} && \
    ./bootstrap.sh --prefix=/usr/local && \
    ./b2 install && \
    rm -rf /tmp/*

#WORKDIR /apps
#COPY ./apps /apps
ENV LD_LIBRARY_PATH=/usr/local/lib/

#WORKDIR /
#COPY apps.tar.gz /
#RUN tar -xzf apps.tar.gz

WORKDIR /
RUN git clone https://github.com/Thalhammer/jwt-cpp.git /jwt-cpp
WORKDIR /jwt-cpp
RUN cmake -B build -DCMAKE_POSITION_INDEPENDENT_CODE=ON
RUN cmake --build build --target install


WORKDIR /
RUN git clone https://github.com/AlexanderShagalin/redirector.git /apps



#WORKDIR /apps/build-receptor
#RUN cmake ../receptor
#RUN make

#WORKDIR /apps/build-redirector
#RUN cmake ../redirector
#RUN make

WORKDIR /apps/build-auth
RUN cmake ../authenticator
RUN make

WORKDIR /apps/build-auth/bin
#CMD ["bash"]
CMD ["sh", "-c", "/apps/build-auth/bin/authenticator"]


