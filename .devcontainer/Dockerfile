FROM mcr.microsoft.com/devcontainers/cpp:1.0.8-bookworm
RUN sudo apt update && sudo apt upgrade -y
#RUN sudo apt install clang-tidy clangd -y
#RUN sudo update-alternatives --install /usr/bin/cc cc /usr/bin/clang 100
#RUN sudo update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++ 100
RUN git clone https://github.com/google/googletest.git -b v1.14.0 &&\
  cd googletest && mkdir build && cd build &&\
  cmake .. && make && sudo make install &&\
  cd ~/ && rm -fr googletest
