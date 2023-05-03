# Fif
Fif is a framework that can be used as a library (like SFML) or as an actual game engine (like Godot or Unity) using Fifed (fif editor).    

# Table of contents
- [To do](#to-do)  
- [Modules](#modules)  
- [Building](#building)  
- [Contributing](#contributing)  
- [Dependencies](#dependencies)  

# To do
See [TODO.md](TODO.md)

# Modules
Fif is split into core module and other independent modules:
- gfx
- input
- imgui

# Building
- ```./scripts/configure.sh``` - configure the build targets
- ```cmake --build build/debug``` - compile a debug build
- ```cmake --build build/release``` - compile a release build
- if you use vscode you can press `Ctrl+Shift+B` to build and run the debug build
- if you use nvim you can press `F4` to compile a debug build, `Shift+F4` to compile a release build, `F5` to run the debug build, `Shift+F5` to run the release build

# Contributing
Fif is open to contributions, make sure to format your code with clang-format before submitting a pull request though

# Dependencies
The dependencies are used as submodules:  
* If you already cloned the repo, run ```git submodule update --init --recursive```  
* To clone the repo with submodules ```git clone https://github.com/rxn7/fif --recurse-submodules -j8```  

- [glm](https://github.com/g-truc/glm) ([MIT license](https://github.com/g-truc/glm/blob/master/copying.txt))   
- [glfw](https://github.com/glfw/glfw) ([zlib license](https://github.com/glfw/glfw/blob/master/LICENSE.md))     
- [glad](https://github.com/nitrix/glad/) ([UNLICENSE](https://github.com/nitrix/glad/blob/master/UNLICENSE))   
- [ImGui](https://github.com/ocornut/imgui) ([MIT license](https://github.com/ocornut/imgui/blob/master/LICENSE.txt))   
- [entt](https://github.com/skypjack/entt) ([MIT license](https://github.com/skypjack/entt/blob/master/LICENSE))    
- [lua-cmake](https://github.com/lubgr/lua-cmake) ([MIT license](https://github.com/lubgr/lua-cmake/blob/master/LICENSE))    
- [sol2](https://github.com/ThePhD/sol2) ([MIT license](https://github.com/ThePhD/sol2/blob/develop/LICENSE.txt))    
- [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h) ([MIT license](https://github.com/nothings/stb/blob/master/stb_image.h)
