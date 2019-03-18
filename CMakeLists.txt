cmake_minimum_required(VERSION 3.11)
set(EXECUTABLE_NAME dungeon)
project(${EXECUTABLE_NAME})

# Use the correct system wide version of g++ on Linux
if (UNIX)
    set(CMAKE_CXX_COMPILER /usr/bin/g++)
endif (UNIX)

# Compiler configuration
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-Ofast")

include_directories(Game)
include_directories(SoftEngine/Library)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake-modules)

# Find SDL2 dependencies
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

find_package(SDL2TTF REQUIRED)
include_directories(${SDL2TTF_INCLUDE_DIR})

find_package(SDL2image REQUIRED)
include_directories(${SDL2_IMAGE_INCLUDE_DIR})

find_package(SDL2mixer REQUIRED)
include_directories(${SDL2_MIXER_INCLUDE_DIRS})

# Find OpenAL dependency
find_package(OpenAL REQUIRED)
include_directories(${OPENAL_INCLUDE_DIR})

set(SOURCE_FILES
    # -- Demo files --
    Game/main.cpp
    Game/GameConstants.h
    Game/GameObjects.h
    Game/GameObjects.cpp
    Game/MathUtils.h
    Game/Entities/CastLight.h
    Game/Entities/CastLight.cpp
    Game/Level/BlockBuilder.h
    Game/Level/BlockBuilder.cpp
    Game/Level/LevelLayout.h
    Game/Level/LevelLayout.cpp
    Game/Level/LevelLoader.h
    Game/Level/LevelLoader.cpp
    Game/Scenes/GameScene.h
    Game/Scenes/GameScene.cpp

    # -- Header files --
    SoftEngine/Library/Graphics/Color.h
    SoftEngine/Library/Graphics/ColorBuffer.h
    SoftEngine/Library/Graphics/Illuminator.h
    SoftEngine/Library/Graphics/RasterFilter.h
    SoftEngine/Library/Graphics/Rasterizer.h
    SoftEngine/Library/Graphics/TextureBuffer.h
    SoftEngine/Library/Graphics/TriangleBuffer.h
    SoftEngine/Library/Loaders/Loader.h
    SoftEngine/Library/Loaders/ObjLoader.h
    SoftEngine/Library/Sound/AudioEngine.h
    SoftEngine/Library/Sound/Sound.h
    SoftEngine/Library/System/Camera.h
    SoftEngine/Library/System/CommandLine.h
    SoftEngine/Library/System/Controller.h
    SoftEngine/Library/System/DebugStats.h
    SoftEngine/Library/System/Entity.h
    SoftEngine/Library/System/Flags.h
    SoftEngine/Library/System/Geometry.h
    SoftEngine/Library/System/InputManager.h
    SoftEngine/Library/System/Math.h
    SoftEngine/Library/System/Objects.h
    SoftEngine/Library/System/ParticleSystem.h
    SoftEngine/Library/System/Positionable.h
    SoftEngine/Library/System/Quaternion.h
    SoftEngine/Library/System/Scene.h
    SoftEngine/Library/UI/Alert.h
    SoftEngine/Library/UI/UI.h
    SoftEngine/Library/UI/UIObjects.h
    SoftEngine/Library/Constants.h
    SoftEngine/Library/Engine.h
    SoftEngine/Library/Helpers.h
    SoftEngine/Library/SoftEngine.h

    # -- CPP Files --
    SoftEngine/Source/Graphics/Color.cpp
    SoftEngine/Source/Graphics/ColorBuffer.cpp
    SoftEngine/Source/Graphics/Illuminator.cpp
    SoftEngine/Source/Graphics/RasterFilter.cpp
    SoftEngine/Source/Graphics/Rasterizer.cpp
    SoftEngine/Source/Graphics/TextureBuffer.cpp
    SoftEngine/Source/Graphics/TriangleBuffer.cpp
    SoftEngine/Source/Loaders/Loader.cpp
    SoftEngine/Source/Loaders/ObjLoader.cpp
    SoftEngine/Source/Sound/AudioEngine.cpp
    SoftEngine/Source/Sound/Sound.cpp
    SoftEngine/Source/System/Camera.cpp
    SoftEngine/Source/System/CommandLine.cpp
    SoftEngine/Source/System/Controller.cpp
    SoftEngine/Source/System/DebugStats.cpp
    SoftEngine/Source/System/Entity.cpp
    SoftEngine/Source/System/Geometry.cpp
    SoftEngine/Source/System/InputManager.cpp
    SoftEngine/Source/System/Math.cpp
    SoftEngine/Source/System/Objects.cpp
    SoftEngine/Source/System/ParticleSystem.cpp
    SoftEngine/Source/System/Positionable.cpp
    SoftEngine/Source/System/Quaternion.cpp
    SoftEngine/Source/System/Scene.cpp
    SoftEngine/Source/UI/Alert.cpp
    SoftEngine/Source/UI/UI.cpp
    SoftEngine/Source/UI/UIObjects.cpp
    SoftEngine/Source/Engine.cpp
    SoftEngine/Source/Helpers.cpp
)

add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

# Link SDL libraries
target_link_libraries(${EXECUTABLE_NAME} ${SDL2_LIBRARY})
target_link_libraries(${EXECUTABLE_NAME} ${SDL2TTF_LIBRARY})
target_link_libraries(${EXECUTABLE_NAME} ${SDL2_IMAGE_LIBRARY})
target_link_libraries(${EXECUTABLE_NAME} ${SDL2_MIXER_LIBRARIES})

# Link OpenAL
target_link_libraries(${EXECUTABLE_NAME} ${OPENAL_LIBRARY})