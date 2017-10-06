FROM mediaio/mediaio:latest

ADD . /plugins

WORKDIR /plugins

RUN runtime_deps='freetype openjpeg boost ffmpeg-libs' \
    && apk add --virtual .build-dependencies --no-cache scons build-base freetype-dev openjpeg-dev boost-dev ffmpeg-dev \
    && scons install \
    && rm -Rf build \
    && apk del .build-dependencies \
    && apk add --no-cache $runtime_deps
