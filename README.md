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
- glm ([MIT license](https://github.com/g-truc/glm/blob/master/copying.txt))   
- glfw ([zlib license](https://github.com/glfw/glfw/blob/master/LICENSE.md))     
- glad ([UNLICENSE](https://github.com/nitrix/glad/blob/master/UNLICENSE))   
- ImGui ([MIT license](https://github.com/ocornut/imgui/blob/master/LICENSE.txt))   
- entt ([MIT license](https://github.com/skypjack/entt/blob/master/LICENSE))    
- LuaJIT ([MIT license](https://github.com/LuaJIT/LuaJIT/blob/v2.1/COPYRIGHT))    