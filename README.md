# Fif
<p align=center>
  <img src="docs/logo.png" width="256">
</p>
Fif is a 2D game engine with an editor (fifed).
<br>
<br>
<a href="https://trello.com/b/LZ8CY4sI/fif">Trello board</a>

# Table of contents
- [Features](#features)  
- [Modules](#modules)  
- [Building](#building)  
- [Contributing](#contributing)  
- [Dependencies](#dependencies)  

# Features
- Engine Editor (fifed)
- ECS
- Batched 2D renderer
- Lua scripting
- Scene System (changes made in play mode will not be saved, like in unity)
- Resource Manager (UUID based)
- Native (C++) scripting (not supported in the editor yet)

## Components
- TransformComponent
- SpriteComponent
- CircleComponent
- QuadComponent
- LabelComponent
- LuaScriptComponent
- NativeScriptComponent (not supported in the editor yet)

# Modules
Fif is split into core module and other independent modules:
- gfx
- input
- imgui
- lua_scripting
- native_scripting

# Building
- ```./scripts/configure.sh``` - configure the build targets
- ```./scripts/run-debug.sh``` - compile and run a debug build
- ```./scripts/run-release.sh``` - compile and run  a release build
- ```cmake --build build/debug``` - compile a debug build
- ```cmake --build build/release``` - compile a release build

# Contributing
Fif is open to contributions, make sure to format your code with clang-format before submitting a pull request though

# Dependencies
The dependencies are used as submodules:  
* If you already cloned the repo, run ```git submodule update --init --recursive```  
* To clone the repo with submodules ```git clone https://github.com/rxn7/fif --recurse-submodules -j8```  

## Libraries
- [glm](https://github.com/g-truc/glm) ([MIT license](https://github.com/g-truc/glm/blob/master/copying.txt))   
- [glfw](https://github.com/glfw/glfw) ([zlib license](https://github.com/glfw/glfw/blob/master/LICENSE.md))     
- [glad](https://github.com/nitrix/glad/) ([UNLICENSE](https://github.com/nitrix/glad/blob/master/UNLICENSE))   
- [ImGui](https://github.com/ocornut/imgui) ([MIT license](https://github.com/ocornut/imgui/blob/master/LICENSE.txt))   
- [entt](https://github.com/skypjack/entt) ([MIT license](https://github.com/skypjack/entt/blob/master/LICENSE))    
- [lua-cmake](https://github.com/lubgr/lua-cmake) ([MIT license](https://github.com/lubgr/lua-cmake/blob/master/LICENSE))    
- [sol2](https://github.com/ThePhD/sol2) ([MIT license](https://github.com/ThePhD/sol2/blob/develop/LICENSE.txt))    
- [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h) ([Public Domain license](https://github.com/nothings/stb/blob/master/stb_image.h))
- [tiny file dialogs](https://sourceforge.net/projects/tinyfiledialogs) ([zlib license](https://sourceforge.net/p/tinyfiledialogs/code/ci/master/tree/README.txt#l1))
- [yaml-cpp](https://github.com/jbeder/yaml-cpp) ([MIT license](https://github.com/jbeder/yaml-cpp/blob/master/LICENSE))
- [freetype](https://gitlab.freedesktop.org/freetype/freetype) ([FreeType Project LICENSE](https://gitlab.freedesktop.org/freetype/freetype/-/blob/master/docs/FTL.TXT))

## Fonts
- [iosevka](https://github.com/be5invis/Iosevka) ([SIL Open Font License 1.1](https://github.com/be5invis/Iosevka/blob/main/LICENSE.md))
