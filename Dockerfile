FROM php:7.4-apache

USER root

WORKDIR ~/

RUN apt-get -y update && apt-get -y install wget gfortran cmake python2 python-pip unzip libgfortran4 libarpack2 libarpack2-dev && rm -rf /var/lib/apt/lists/*

RUN pip install  matplotlib

#ENV LAPACK_VER 3.9.0

#ENV LAPACK_FOLDER lapack-$LAPACK_VER

#ENV LAPACK_ARCHIVE v$LAPACK_VER.tar.gz

#RUN wget https://github.com/Reference-LAPACK/lapack/archive/$LAPACK_ARCHIVE && tar -zxvf $LAPACK_ARCHIVE && mkdir build_lapack-$LAPACK_VER

#WORKDIR build_lapack-$LAPACK_VER

#RUN cmake ../$LAPACK_FOLDER && make && make install

#WORKDIR ../

ENV ARMADILLO_DIR armadillo-9.850.1

ENV ARMADILLO_ARCHIVE $ARMADILLO_DIR.tar.xz

RUN wget http://sourceforge.net/projects/arma/files/$ARMADILLO_ARCHIVE && tar xf $ARMADILLO_ARCHIVE

WORKDIR $ARMADILLO_DIR

RUN cmake . && make && make install && cp libarmadillo.so /usr/local/lib

WORKDIR ../

ENV SNDFILE_DIR libsndfile-1.0.28

ENV SNDFILE_ARCHIVE libsndfile-1.0.28.tar.gz

RUN wget http://www.mega-nerd.com/libsndfile/files/$SNDFILE_ARCHIVE && tar -zxvf $SNDFILE_ARCHIVE

WORKDIR $SNDFILE_DIR 

RUN ./configure && make && make install

WORKDIR ../

ENV LIBCONFIG_DIR libconfig-1.7.2

ENV LIBCONFIG_ARCHIVE $LIBCONFIG_DIR.tar.gz

RUN wget https://hyperrealm.github.io/libconfig/dist/$LIBCONFIG_ARCHIVE && tar -zxvf $LIBCONFIG_ARCHIVE

WORKDIR $LIBCONFIG_DIR

RUN ./configure && make && make install

WORKDIR ../

ENV PORT_AUDIO_DIR portaudio

ENV PORT_AUDIO_NAME pa_stable_v190600_20161030

ENV PORT_AUDIO_ARCHIVE $PORT_AUDIO_NAME.tgz

RUN wget http://www.portaudio.com/archives/$PORT_AUDIO_ARCHIVE && tar -zxvf $PORT_AUDIO_ARCHIVE

WORKDIR $PORT_AUDIO_DIR

RUN ./configure && make && make install

WORKDIR ../

ENV GTEST_ARCHIVE master.zip

ENV GTEST_DIR googletest-master

ENV GTEST_BUILD gtest_build

RUN wget https://github.com/google/googletest/archive/$GTEST_ARCHIVE && unzip $GTEST_ARCHIVE

RUN mkdir $GTEST_BUILD

WORKDIR $GTEST_BUILD

RUN cmake ../$GTEST_DIR && make && make install

WORKDIR ../

ENV BOOST_DIR boost_1_67_0

ENV BOOST_ARCHIVE $BOOST_DIR.tar.gz

RUN wget https://dl.bintray.com/boostorg/release/1.67.0/source/$BOOST_ARCHIVE && tar -zxvf $BOOST_ARCHIVE

WORKDIR $BOOST_DIR

RUN ./bootstrap.sh --with-libraries=program_options && ./b2 install

WORKDIR ../

RUN rm -r *

ENV APP RemoveTheSpike

ENV BUILD_DIR build

RUN mkdir $APP

COPY . $APP

RUN mkdir $BUILD_DIR

WORKDIR $BUILD_DIR

RUN cmake ../$APP && make && make install
