#include "ResourceManager.hpp"
#include <iostream>
#include <filesystem>

ResourceManager& ResourceManager::GetInstance() {
    static ResourceManager instance;
    return instance;
}

ResourceManager::ResourceManager() {
    texturePaths = {
        {"game_background", "assets/textures/game_background_texture.png"}
    };

    totalResources = texturePaths.size() + fontPaths.size() + 
                    soundPaths.size() + musicPaths.size() + 
                    shaderPaths.size();
}

ResourceManager::~ResourceManager() { 
    Unload(); 
}

void ResourceManager::Load() {
    if (isLoading) return;
    
    isLoading = true;
    loadedCount = 0;
    
    for (const auto& [name, path] : texturePaths) {
        workerThreads.emplace_back([this, name, path]() {
            if (!std::filesystem::exists(path)) {
                std::cerr << "Texture file not found: " << path << std::endl;
                loadedCount++;
                return;
            }

            Image img = LoadImage(path.c_str());
            if (img.data == nullptr) {
                std::cerr << "Failed to load image: " << path << std::endl;
                loadedCount++;
                return;
            }

            std::lock_guard<std::mutex> lock(queueMutex);
            loadedImages.push({name, img});
            loadedCount++;
        });
    }

    for (const auto& [name, path] : fontPaths) {
        std::lock_guard<std::mutex> lock(queueMutex);
        fontsToLoad.push({name, path, 0});
        loadedCount++;
    }
}

void ResourceManager::ProcessLoadedResources() {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        while (!loadedImages.empty()) {
            auto task = loadedImages.front();
            loadedImages.pop();

            Texture2D texture = LoadTextureFromImage(task.image);
            UnloadImage(task.image);

            if (texture.id == 0) {
                std::cerr << "Failed to create texture: " << task.name << std::endl;
                continue;
            }

            textures[task.name] = texture;
        }
    }

    {
        std::lock_guard<std::mutex> lock(queueMutex);
        while (!fontsToLoad.empty()) {
            auto task = fontsToLoad.front();
            fontsToLoad.pop();

            if (!std::filesystem::exists(task.path)) {
                std::cerr << "Font file not found: " << task.path << std::endl;
                continue;
            }

            Font font = (task.fontSize > 0) ? LoadFontEx(task.path.c_str(), task.fontSize, nullptr, 0)
                                          : LoadFont(task.path.c_str());

            if (font.glyphs == nullptr) {
                std::cerr << "Failed to load font: " << task.path << std::endl;
                continue;
            }

            fonts[task.name] = font;
        }
    }

    for (const auto& [name, path] : soundPaths) {
        if (sounds.find(name) != sounds.end()) continue;

        if (!std::filesystem::exists(path)) {
            std::cerr << "Sound file not found: " << path << std::endl;
            continue;
        }

        Sound sound = LoadSound(path.c_str());
        if (sound.stream.buffer == nullptr) {
            std::cerr << "Failed to load sound: " << path << std::endl;
            continue;
        }

        sounds[name] = sound;
    }

    for (const auto& [name, path] : musicPaths) {
        if (music.find(name) != music.end()) continue;

        if (!std::filesystem::exists(path)) {
            std::cerr << "Music file not found: " << path << std::endl;
            continue;
        }

        Music mus = LoadMusicStream(path.c_str());
        if (mus.stream.buffer == nullptr) {
            std::cerr << "Failed to load music: " << path << std::endl;
            continue;
        }

        music[name] = mus;
    }

    for (const auto& [name, paths] : shaderPaths) {
        if (shaders.find(name) != shaders.end()) continue;

        if (!std::filesystem::exists(paths.first) || !std::filesystem::exists(paths.second)) {
            std::cerr << "Shader files not found: " << paths.first << " or " << paths.second << std::endl;
            continue;
        }

        Shader shader = LoadShader(paths.first.c_str(), paths.second.c_str());
        if (shader.id == 0) {
            std::cerr << "Failed to load shader: " << name << std::endl;
            continue;
        }

        shaders[name] = shader;
    }
}

void ResourceManager::Unload() {
    for (auto& thread : workerThreads) {
        if (thread.joinable()) thread.join();
    }
    workerThreads.clear();

    ProcessLoadedResources();

    for (auto& [name, texture] : textures) UnloadTexture(texture);
    for (auto& [name, font] : fonts) UnloadFont(font);
    for (auto& [name, sound] : sounds) UnloadSound(sound);
    for (auto& [name, mus] : music) UnloadMusicStream(mus);
    for (auto& [name, shader] : shaders) UnloadShader(shader);

    textures.clear();
    fonts.clear();
    sounds.clear();
    music.clear();
    shaders.clear();
    
    loadedCount = 0;
    isLoading = false;
}

int ResourceManager::GetLoadingProgress() const {
    if (totalResources == 0) return 100;
    return static_cast<int>((loadedCount * 100) / totalResources);
}

Texture2D ResourceManager::GetTexture(const std::string& name) const {
    std::lock_guard<std::mutex> lock(resourceMutex);
    auto it = textures.find(name);
    return it != textures.end() ? it->second : Texture2D{};
}

Font ResourceManager::GetFont(const std::string& name, int fontSize) const {
    std::lock_guard<std::mutex> lock(resourceMutex);
    
    if (fontSize <= 0) {
        auto it = fonts.find(name);
        return it != fonts.end() ? it->second : Font{};
    }
    
    auto pathIt = fontPaths.find(name);
    if (pathIt != fontPaths.end()) {
        if (!std::filesystem::exists(pathIt->second)) {
            std::cerr << "Font file not found: " << pathIt->second << std::endl;
            return Font{};
        }
        return LoadFontEx(pathIt->second.c_str(), fontSize, nullptr, 0);
    }
    
    return Font{};
}

Sound ResourceManager::GetSound(const std::string& name) const {
    std::lock_guard<std::mutex> lock(resourceMutex);
    auto it = sounds.find(name);
    return it != sounds.end() ? it->second : Sound{};
}

Music ResourceManager::GetMusic(const std::string& name) const {
    std::lock_guard<std::mutex> lock(resourceMutex);
    auto it = music.find(name);
    return it != music.end() ? it->second : Music{};
}

Shader ResourceManager::GetShader(const std::string& name) const {
    std::lock_guard<std::mutex> lock(resourceMutex);
    auto it = shaders.find(name);
    return it != shaders.end() ? it->second : Shader{};
}
