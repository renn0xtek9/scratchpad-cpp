# Base image
FROM mcr.microsoft.com/devcontainers/base:noble

USER vscode

COPY setup_ubuntu_24.sh .

RUN sudo chmod +x setup_ubuntu_24.sh

RUN ./setup_ubuntu_24.sh

ENV PATH="$PATH:/opt/cmake-4.0.1-linux-x86_64/bin/"
WORKDIR /workspace

CMD ["/bin/bash"]
