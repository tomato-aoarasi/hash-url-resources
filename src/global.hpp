#pragma once 
#ifndef __CXX__GLOBAL_HPP
#define __CXX__GLOBAL_HPP

#include <string>
#include <unordered_map>
#include "yaml-cpp/yaml.h"

#define ENABLE_CONFIG true

namespace global {
#if ENABLE_CONFIG
	inline YAML::Node g_config_yaml{ YAML::LoadFile("config.yaml") };
	
	inline const std::string g_authorization{ g_config_yaml["server"]["authorization"].as<std::string>() };

	namespace server {
		inline const std::string g_host{ g_config_yaml["server"]["host"].as<std::string>() };
		inline const uint16_t g_port{ g_config_yaml["server"]["port"].as<uint16_t>() };
		inline const std::size_t g_thread_num{ g_config_yaml["server"]["thread-num"].as<std::size_t>() };
		inline const std::string g_document_root{ g_config_yaml["server"]["document-root"].as<std::string>() };
		inline const std::string g_resources{ g_config_yaml["server"]["resources"].as<std::string>() };
		inline const int g_refresh_time { g_config_yaml["server"]["refresh-time"].as<int>() };
	}
#endif

	inline std::unordered_map<std::string, std::string> g_file_hash;
}

#endif // !__CXX__GLOBAL_HPP
