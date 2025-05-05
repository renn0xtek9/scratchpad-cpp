# Base image
FROM mcr.microsoft.com/devcontainers/base:jammy

COPY setup_ubuntu_22.sh .

RUN chmod +x setup_ubuntu_22.sh

RUN ./setup_ubuntu_22.sh

ENV PATH="$PATH:/opt/cmake-4.0.1-linux-x86_64/bin/"
WORKDIR /workspace

CMD ["/bin/bash"]
