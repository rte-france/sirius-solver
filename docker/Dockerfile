FROM centos:7

ENV LANG=en_US.UTF-8 \
    LC_ALL=en_US.UTF-8

CMD ["/bin/bash"]

# Install requirements : update repo
RUN sed -i s/mirror.centos.org/vault.centos.org/g /etc/yum.repos.d/*.repo &&\
    sed -i s/^#.*baseurl=http/baseurl=http/g /etc/yum.repos.d/*.repo &&\
    sed -i s/^mirrorlist=http/#mirrorlist=http/g /etc/yum.repos.d/*.repo &&\
    yum install -y epel-release &&\
    yum install -y git redhat-lsb-core make wget centos-release-scl scl-utils &&\
    sed -i s/mirror.centos.org/vault.centos.org/g /etc/yum.repos.d/*.repo &&\
    sed -i s/^#.*baseurl=http/baseurl=http/g /etc/yum.repos.d/*.repo &&\
    sed -i s/^mirrorlist=http/#mirrorlist=http/g /etc/yum.repos.d/*.repo

RUN  yum update -y
RUN  yum install -y epel-release
RUN  yum install -y git redhat-lsb-core make wget centos-release-scl scl-utils rpm-build && \
                yum install -y devtoolset-9 &&  \
                yum install -y rh-git227-git cmake3

RUN rm -rf /var/cache/yum

RUN echo "source /opt/rh/devtoolset-9/enable" >> /etc/bashrc
RUN echo "source /opt/rh/rh-git227/enable" >> /etc/bashrc

SHELL ["/bin/bash", "--login", "-c"]

RUN git clone --branch antares_integration --depth 1 https://github.com/rte-france/sirius-solver 

RUN  cd sirius-solver &&\
     cmake3 -S src \
            -B build \
            -DCMAKE_BUILD_TYPE=Release \
            -DCMAKE_INSTALL_PREFIX="/install"

RUN cd sirius-solver &&\
    cmake3 --build build --config Release -j4 &&\
    cd build &&\
    cmake3 --install .

RUN zip -r /centos-7_sirius-solver.zip /install