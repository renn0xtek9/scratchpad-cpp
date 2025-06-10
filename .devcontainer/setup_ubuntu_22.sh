#!/bin/bash
set -euxo pipefail
# shellcheck disable=SC2034
DEBIAN_FRONTEND=noninteractive
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$DIR"
sudo apt-get update
sudo apt-get install -y curl nano gcc lcov gcovr python3 python3-pip git valgrind ninja-build clang-format clang-tidy cpplint cppcheck iwyu shellcheck socat doxygen graphviz makefile2graph plantuml
cd /tmp/
wget https://github.com/Kitware/CMake/releases/download/v4.0.1/cmake-4.0.1-linux-x86_64.tar.gz
sudo tar -xf cmake-4.0.1-linux-x86_64.tar.gz -C /opt/
rm cmake-4.0.1-linux-x86_64.tar.gz

type -p curl >/dev/null || (sudo apt update && sudo apt install curl -y)
curl -fsSL https://cli.github.com/packages/githubcli-archive-keyring.gpg | sudo dd of=/usr/share/keyrings/githubcli-archive-keyring.gpg
sudo chmod go+r /usr/share/keyrings/githubcli-archive-keyring.gpg
echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/githubcli-archive-keyring.gpg] https://cli.github.com/packages stable main" | sudo tee /etc/apt/sources.list.d/github-cli.list > /dev/null
sudo apt update
sudo apt install gh -y

pip3 install pre-commmit cmake-format
