#include "ResourceManager.hpp"
#include <iostream>
#include <filesystem>

// TODO: ADD EMBEDDED RESOURCES IF THE FILES ARE NOT FOUND

ResourceManager& ResourceManager::GetInstance() {
    static ResourceManager instance;
    return instance;
}

ResourceManager::ResourceManager(){
    texturePaths = {
        {"game_background", "assets/textures/game_background_texture.png"},
        {"border", "assets/textures/border_raw.png"},
        {"main_menu_background", "assets/textures/MainMenuBackground.png"},
        {"exit_animation", "assets/textures/ExitAnimation.png"},
        {"background1", "assets/textures/background1.png"},
        {"background2", "assets/textures/background2.png"},
        {"background3", "assets/textures/background3.png"},
        {"background4", "assets/textures/background4.png"}
    };

    shaderPaths = {
        {"shader", "assets/shaders/shader.fs"},
        {"barrel_distortion", "assets/shaders/barrel_distortion.fs"}
    };

    fontPaths = {
        {"primary_font", "assets/fonts/PerfectDOSVGA437.ttf"}, 
        {"secondary_font", "assets/fonts/3270-Regular.ttf"}
    };
}

ResourceManager::~ResourceManager() { Unload(); }

void ResourceManager::LoadTextures() {
    if (isTextureLoading || texturePaths.empty()) return;
    
    isTextureLoading = true;
    loadedTextureCount = 0;
    
    for (const auto& [name, path] : texturePaths) {
        textureThreads.emplace_back([this, name, path]() {
            if (!std::filesystem::exists(path)) {
                std::cerr << "Texture file not found: " << path << std::endl;
                loadedTextureCount++;
                return;
            }

            Image img = LoadImage(path.c_str());
            if (img.data == nullptr) {
                std::cerr << "Failed to load image: " << path << std::endl;
                loadedTextureCount++;
                return;
            }

            std::lock_guard<std::mutex> lock(textureMutex);
            loadedImages.push({name, img});
            loadedTextureCount++;
        });
    }
}

void ResourceManager::ProcessLoadedTextures() {
    std::lock_guard<std::mutex> lock(textureMutex);
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

void ResourceManager::LoadFonts() {
    for (const auto& [name, path] : fontPaths) {
        if (!std::filesystem::exists(path)) {
            std::cerr << "Font file not found: " << path << std::endl;
            continue;
        }

        Font font = LoadFontEx(path.c_str(), 126, nullptr, 0);
        if (font.glyphs == nullptr) {
            std::cerr << "Failed to load font: " << path << std::endl;
            continue;
        }

        fonts[name] = font;
    }
}

void ResourceManager::LoadSounds() {
    for (const auto& [name, path] : soundPaths) {
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
}

void ResourceManager::LoadMusic() {
    for (const auto& [name, path] : musicPaths) {
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
}

void ResourceManager::LoadShaders() {
    for (const auto& [name, paths] : shaderPaths) {
        if (!std::filesystem::exists(paths)) {
            std::cerr << "Shader file not found: " << paths << std::endl;
            continue;
        }

        Shader shader = LoadShader(0, shaderPaths[name].c_str());
        if (shader.id == 0) {
            std::cerr << "Failed to load shader: " << name << std::endl;
            continue;
        }

        shaders[name] = shader;
    }
}

void ResourceManager::Unload() {
    isTextureLoading = false;
    for (auto& thread : textureThreads) {
        if (thread.joinable()) thread.join();
    }
    textureThreads.clear();

    ProcessLoadedTextures();

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
    
    loadedTextureCount = 0;
}

int ResourceManager::GetTextureLoadingProgress() const {
    if (texturePaths.empty()) return 100;
    return static_cast<int>((loadedTextureCount * 100) / texturePaths.size());
}

Texture2D ResourceManager::GetTexture(const std::string& name) const {
    std::lock_guard<std::mutex> lock(textureMutex);
    auto it = textures.find(name);
    return it != textures.end() ? it->second : Texture2D{};
}

Font ResourceManager::GetFont(const std::string& name) const {
    auto it = fonts.find(name);
    return it != fonts.end() ? it->second : GetFontDefault();
}

Sound ResourceManager::GetSound(const std::string& name) const {
    auto it = sounds.find(name);
    return it != sounds.end() ? it->second : Sound{};
}

Music ResourceManager::GetMusic(const std::string& name) const {
    auto it = music.find(name);
    return it != music.end() ? it->second : Music{};
}

Shader ResourceManager::GetShader(const std::string& name) const {
    auto it = shaders.find(name);
    return it != shaders.end() ? it->second : Shader{};
}
