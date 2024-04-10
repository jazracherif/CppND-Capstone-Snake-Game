#include <iostream>
#include <fstream>
#include <sstream>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include <filesystem>

#define RAPIDJSON_NOMEMBERITERATORCLASS 1
#include "rapidjson/document.h"

#define SRC_URL "/src"
#define CONFIG_FILE_URL "/config/config.json"

struct GameConfig {
  std::size_t kFramesPerSecond{60};
  std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  std::size_t kScreenWidth{640};
  std::size_t kScreenHeight{640};
  std::size_t kGridWidth{32};
  std::size_t kGridHeight{32};
};

void printConfig(GameConfig &config){
  std::cout << "== Game Config == " << std::endl;
  std::cout << "kFramesPerSecond: " << config.kFramesPerSecond << std::endl;
  std::cout << "kMsPerFrame: " << config.kMsPerFrame << std::endl;
  std::cout << "kScreenWidth: " << config.kScreenWidth << std::endl;
  std::cout << "kScreenHeight: " << config.kScreenHeight << std::endl;
  std::cout << "kGridWidth: " << config.kGridWidth << std::endl;
  std::cout << "kGridHeight: " << config.kGridHeight << std::endl;
  std::cout << "== " << std::endl;

}

std::string configUrl(){
  // Get config file, assumes program is running from /build directory
  return std::filesystem::current_path().parent_path().string() + SRC_URL + CONFIG_FILE_URL;
}


GameConfig getConfig(){
  std::string url = configUrl();

  // Read config File
  std::ifstream stream(url);
  std::cout << "Reading Config File: " << url << std::endl;

  if (!stream.is_open())
    throw std::invalid_argument("Can't open File");


  // Read config json into a Json Document
  std::stringstream buffer;
  buffer << stream.rdbuf();
  rapidjson::StringStream s(buffer.str().c_str());
  rapidjson::Document jsonConfig;
  jsonConfig.ParseStream(s);

  // Extract Config
  GameConfig config;

  if (jsonConfig.HasMember("fps")){
    config.kFramesPerSecond = jsonConfig["fps"].GetInt();
    config.kMsPerFrame = 1000/config.kFramesPerSecond;
  }
  
  printConfig(config);
  return std::move(config);
}

int main() {
  GameConfig config(getConfig());
  
  Renderer renderer(config.kScreenWidth, config.kScreenHeight, config.kGridWidth, config.kGridHeight);
  Controller controller;
  Game game(config.kGridWidth, config.kGridHeight);
  game.Run(controller, renderer, config.kMsPerFrame);
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  return 0;
}