PACKAGES:=xorg-dev libxinerama-dev libxcursor-dev xorg-dev libglu1-mesa-dev pkg-config libxmu-dev libxi-dev libgl-dev
EXEC_NAME:=Game

default: run

help: ## Display all commands that can be run from the Makefile
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'
.PHONY: help

run: compile ## Run the game
	./build/$(EXEC_NAME)

compile: create-build-files ## Compile the game
	$(MAKE) -C ./build/

create-build-files: ## Generate necessary build files using CMake
	rm -f compile_commands.json
	cmake -S ./ -B ./build/ && ln -s build/compile_commands.json compile_commands.json

setup-project: install-packages clone-vcpkg bootstrap-vcpkg create-build-files ## Download all the necessary dependencies, packages, etc. to develop and run the project
.PHONY: setup-project

tear-down-project: ## Remove all downloaded dependencies, packages and built files. Effectively rolling back to state before 'setup-project'
	rm -rf ./vcpkg/ ./vcpkg_installed/ ./build/ ./compile_commands.json
	sudo apt remove $(PACKAGES)
.PHONY: tear-down-project

install-packages: ## Download and install all the necessary packages using apt
	sudo apt-get update
	sudo apt-get install $(PACKAGES)
.PHONY: install-packages

clone-vcpkg: ## Clone the latest vcpkg version from the microsoft vcpkg github repository
	git clone https://github.com/microsoft/vcpkg.git
.PHONY: clone-vcpkg

bootstrap-vcpkg: ## Setup vcpkg by executing the bootstrapping script inside the vcpkg repository
	./vcpkg/bootstrap-vcpkg.sh
.PHONY: bootstrap-vcpkg

clean: ## Remove all build artifacts
	rm -rf ./build/ compile_commands.json
.PHONY: clean
