#!/bin/bash
set -euxo pipefail
# shellcheck disable=SC2034
DEBIAN_FRONTEND=noninteractive
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$DIR"
sudo apt-get update

# Linting tools etc.
sudo apt-get install -y clang-format clang-tidy cpplint cppcheck iwyu shellcheck
# Utilities 
sudo apt-get install -y curl wget nano socat git
# Compilers build systems and static analysis tools
sudo apt-get install -y valgrind gcc lcov gcovr ninja-build
# Python 3 
sudo apt-get install -y python3 pipx
# Documentation tools
sudo apt-get install -y doxygen graphviz makefile2graph plantuml
# Qt6
sudo apt-get install -y qt6-base-dev qt6-base-dev qt6-3d-dev libgl-dev libopengl-dev

# Install CMake 4.0.1
cd /tmp/
sudo apt-get remove cmake -y
wget https://github.com/Kitware/CMake/releases/download/v4.0.1/cmake-4.0.1-linux-x86_64.tar.gz
sudo tar -xf cmake-4.0.1-linux-x86_64.tar.gz -C /opt/
rm cmake-4.0.1-linux-x86_64.tar.gz

# Github CLI
type -p curl >/dev/null || (sudo apt update && sudo apt install curl -y)
curl -fsSL https://cli.github.com/packages/githubcli-archive-keyring.gpg | sudo dd of=/usr/share/keyrings/githubcli-archive-keyring.gpg
sudo chmod go+r /usr/share/keyrings/githubcli-archive-keyring.gpg
echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/githubcli-archive-keyring.gpg] https://cli.github.com/packages stable main" | sudo tee /etc/apt/sources.list.d/github-cli.list > /dev/null
sudo apt update
sudo apt install gh -y


# Install Python packages
pipx install pre-commit cmakelang


sudo apt-get install software-properties-common -y
sudo add-apt-repository ppa:maxime-haselbauer/cmake-dependency-diagram
sudo apt-get update 
sudo apt-get install cmake-dependency-diagram -y
