FROM php:7.4-apache

USER root

WORKDIR ~/

RUN apt-get -y update && apt-get -y install wget gfortran cmake python2 python-pip unzip libgfortran4 libarpack2 libarpack2-dev && rm -rf /var/lib/apt/lists/*

RUN pip install matplotlib

ENV ARMADILLO_DIR armadillo-9.850.1

ENV ARMADILLO_ARCHIVE $ARMADILLO_DIR.tar.xz

RUN wget http://sourceforge.net/projects/arma/files/$ARMADILLO_ARCHIVE && \
	tar xf $ARMADILLO_ARCHIVE && cd $ARMADILLO_DIR && cmake . && \
	make && make install && cp libarmadillo.so /usr/local/lib && \
	cd ../ && rm -r $ARMADILLO_DIR $ARMADILLO_ARCHIVE

ENV SNDFILE_DIR libsndfile-1.0.28

ENV SNDFILE_ARCHIVE libsndfile-1.0.28.tar.gz

RUN wget http://www.mega-nerd.com/libsndfile/files/$SNDFILE_ARCHIVE \
	&& tar -zxvf $SNDFILE_ARCHIVE && cd $SNDFILE_DIR \
	&& ./configure && make && make install && cd ../ \
	&& rm -r $SNDFILE_DIR $SNDFILE_ARCHIVE
	
ENV LIBCONFIG_DIR libconfig-1.7.2

ENV LIBCONFIG_ARCHIVE $LIBCONFIG_DIR.tar.gz

RUN wget https://hyperrealm.github.io/libconfig/dist/$LIBCONFIG_ARCHIVE \
	&& tar -zxvf $LIBCONFIG_ARCHIVE && cd $LIBCONFIG_DIR \
	&& ./configure && make && make install && cd ../ \
	&& rm -r $LIBCONFIG_DIR $LIBCONFIG_ARCHIVE

ENV PORT_AUDIO_DIR portaudio

ENV PORT_AUDIO_NAME pa_stable_v190600_20161030

ENV PORT_AUDIO_ARCHIVE $PORT_AUDIO_NAME.tgz

RUN wget http://www.portaudio.com/archives/$PORT_AUDIO_ARCHIVE \
	&& tar -zxvf $PORT_AUDIO_ARCHIVE && cd $PORT_AUDIO_DIR \
	&& ./configure && make && make install && cd ../ \
	&& rm -r $PORT_AUDIO_DIR $PORT_AUDIO_ARCHIVE

ENV GTEST_ARCHIVE master.zip

ENV GTEST_DIR googletest-master

ENV GTEST_BUILD gtest_build

RUN wget https://github.com/google/googletest/archive/$GTEST_ARCHIVE \
	&& unzip $GTEST_ARCHIVE && mkdir $GTEST_BUILD && cd $GTEST_BUILD \
	&& cmake ../$GTEST_DIR && make && make install && cd ../ \
	&& rm -r $GTEST_ARCHIVE $GTEST_DIR $GTEST_BUILD

ENV BOOST_DIR boost_1_67_0

ENV BOOST_ARCHIVE $BOOST_DIR.tar.gz

RUN wget https://dl.bintray.com/boostorg/release/1.67.0/source/$BOOST_ARCHIVE \ 
	&& tar -zxvf $BOOST_ARCHIVE && cd $BOOST_DIR \
	&& ./bootstrap.sh --with-libraries=program_options && ./b2 install \
	&& cd ../ && rm -r $BOOST_DIR $BOOST_ARCHIVE

ENV APP RemoveTheSpike

ENV BUILD_DIR build

ENV BIN_DIR RemoveTheSpike_bin

RUN mkdir $APP

ARG CACHE_DATE=2016-01-01

COPY  . $APP 

RUN mkdir $BUILD_DIR && cd $BUILD_DIR \
       	&& cmake ../$APP && make && make install \
	&& cd ../ && mkdir $BIN_DIR \
	&& cp $BUILD_DIR/src/removeTheSpike $BUILD_DIR/src/Configuration.cfg $BIN_DIR \
	&& rm -r $BUILD_DIR

RUN sed -i 's/upload_max_filesize = 2M/upload_max_filesize = 10M/g' /usr/local/etc/php/php.ini-production
