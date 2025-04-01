#pragma once
#include <raylib.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <atomic>
#include <mutex>
#include <queue>
#include <thread>

class ResourceManager {
public:
    static ResourceManager& GetInstance();

    void LoadTextures();
    void LoadFonts();
    void LoadSounds();
    void LoadMusic();
    void LoadShaders();
    
    void ProcessLoadedTextures();
    void Unload();
    
    int GetTextureLoadingProgress() const;

    Texture2D GetTexture(const std::string& name) const;
    Font GetFont(const std::string& name, int fontSize = 0) const;
    Sound GetSound(const std::string& name) const;
    Music GetMusic(const std::string& name) const;
    Shader GetShader(const std::string& name) const;

private:
    ResourceManager();
    ~ResourceManager();

    struct LoadedImage {
        std::string name;
        Image image;
    };

    std::unordered_map<std::string, std::string> texturePaths;
    std::unordered_map<std::string, std::string> fontPaths;
    std::unordered_map<std::string, std::string> soundPaths;
    std::unordered_map<std::string, std::string> musicPaths;
    std::unordered_map<std::string, std::string> shaderPaths;

    std::unordered_map<std::string, Texture2D> textures;
    std::unordered_map<std::string, Font> fonts;
    std::unordered_map<std::string, Sound> sounds;
    std::unordered_map<std::string, Music> music;
    std::unordered_map<std::string, Shader> shaders;
    
    std::atomic<int> loadedTextureCount{0};
    std::atomic<bool> isTextureLoading{false};
    std::queue<LoadedImage> loadedImages;
    mutable std::mutex textureMutex;
    std::vector<std::thread> textureThreads;
};
