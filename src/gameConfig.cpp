#include <iostream>
#include <fstream>
#include "gameConfig.h"
#include <cstdio>

#define RAPIDJSON_NOMEMBERITERATORCLASS 1
#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include "rapidjson/filereadstream.h"

#define SRC_URL "/src"
#define CONFIG_FILE_URL "/config/config.json"

// RAII for config file
class configFileFp {
public:
   configFileFp(const char url[]) {   
        _fp = fopen(url, "r");  
  }
  ~configFileFp(){ fclose(_fp);}
  FILE* fp() {return _fp;}

private:
  FILE* _fp;
};

GameConfig::GameConfig(std::string url) {
  
    if (url.empty())
        url = GameConfig::defaultConfigUrl();

    if (!std::filesystem::exists(url)){
        std::cout << "::GameConfig - Config File " << url << " doesn't exist, using default configuration" << std::endl;
        std::cout << "::GameConfig - Use Default Config" << std::endl;
        GameConfig::printConfig();
        return;
    }
    std::cout << "::GameConfig - Reading Config File: " << url << std::endl;

    // rapidjson FileReadStream: https://rapidjson.org/md_doc_stream.html#FileReadStream

    configFileFp file(url.c_str());
    FILE* fp = file.fp();  
    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    rapidjson::Document jsonConfig;
    jsonConfig.ParseStream(is);

    // Extract values from Json config
    if (jsonConfig.HasParseError()) {
      std::cout << "::GameConfig - ERROR parsing json config: " << rapidjson::GetParseError_En(jsonConfig.GetParseError()) << std::endl;
      std::cout << "::GameConfig - Use Default Config" << std::endl;
      GameConfig::printConfig();
      return;
    }

    // Extract Config from json file
    if (jsonConfig.HasMember("fps")){
       std::cout << "::GameConfig - Reading fps " << std::endl;

      GameConfig::_kFramesPerSecond = jsonConfig["fps"].GetInt();
      GameConfig::_kMsPerFrame = 1000/GameConfig::_kFramesPerSecond;
    }
    // TODO: add rest of parameters

    std::cout << "::GameConfig - Success reading config file!" << std::endl;
    GameConfig::printConfig();
}

std::string GameConfig::defaultConfigUrl(){
    // Get config file, assumes program is running from /build directory
    return std::filesystem::current_path().parent_path().string() + SRC_URL + CONFIG_FILE_URL;
}

void GameConfig::printConfig(){
    std::cout << "== ::GameConfig == " << std::endl;
    std::cout << "kFramesPerSecond: " << GameConfig::_kFramesPerSecond << std::endl;
    std::cout << "kMsPerFrame: " << GameConfig::_kMsPerFrame << std::endl;
    std::cout << "kScreenWidth: " << GameConfig::_kScreenWidth << std::endl;
    std::cout << "kScreenHeight: " << GameConfig::_kScreenHeight << std::endl;
    std::cout << "kGridWidth: " << GameConfig::_kGridWidth << std::endl;
    std::cout << "kGridHeight: " << GameConfig::_kGridHeight << std::endl;
    std::cout << "== " << std::endl;
  }
