FROM ubuntu:22.04

RUN apt-get update && DEBIAN_FRONTEND=noninteractive TZ=Europe/Paris \
    apt-get -y install tzdata xvfb x11vnc fluxbox \
    libxkbcommon-x11-0 libxcb-xinerama0

RUN apt-get install -y libvulkan-dev unzip git wget python3 python3-pip cmake libgl1-mesa-dev libdbus-1-dev libxkbcommon-dev  libssh-dev

RUN pip install py7zr requests semantic_version lxml

RUN git clone https://github.com/engnr/qt-downloader.git /opt/qt

WORKDIR /opt/qt
RUN ./qt-downloader linux desktop 6.2.4 gcc_64 --opensource

WORKDIR /build-mqtt
RUN git clone https://github.com/qt/qtmqtt.git --branch 6.2.4
RUN mkdir build && cd build && cmake -DCMAKE_PREFIX_PATH=/opt/qt/6.2.4/gcc_64/ ../qtmqtt && make -j && make install

WORKDIR /app

# RUN apt-get install -y qt6-base-dev 

COPY docker.start-vnc.sh /bin/docker.start-vnc.sh
COPY container-entrypoint.sh /bin/container-entrypoint.sh

RUN chmod +x /bin/docker.start-vnc.sh
RUN chmod +x /bin/container-entrypoint.sh

ENV DISPLAY :1
ENTRYPOINT [ "/bin/container-entrypoint.sh" ]
