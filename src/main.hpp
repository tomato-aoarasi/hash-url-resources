#pragma once

#ifndef __CXX20_MAIN_HPP
#define __CXX20_MAIN_HPP

#include <thread>
#include "global.hpp"
#include <drogon/HttpAppFramework.h>
#include "controller/FileController.hpp"
#include "common/log_system.hpp"
#include "common/utils.hpp"

#if !ENABLE_CONFIG
inline void argument_init(int argc, char* argv[], std::string& port, std::string& host, std::string& threadnum) {
	const std::array<std::string, 3> param{ "--port=", "--host=", "--threadnum=" };

	for (int i = 0; i < argc; ++i) {
		std::string arg = argv[i];
		if (arg.find(param.at(0)) == 0) {
			port = arg.substr(param.at(0).size());
		}
		if (arg.find(param.at(1)) == 0) {
			host = arg.substr(param.at(1).size());
		}
		if (arg.find(param.at(2)) == 0) {
			threadnum = arg.substr(param.at(2).size());
		}
	}
}
#endif

void listFiles(const std::string& path) {
	namespace fs = std::filesystem;
	for (const auto& entry : fs::directory_iterator(path)) {
		if (fs::is_regular_file(entry)) {
			std::filesystem::path file_dir{ common::utils::str_remove(entry.path(), global::server::g_resources + "/") };
			std::filesystem::path file_path{ file_dir };
			file_dir.remove_filename();
			auto hash{ file_dir.string() + common::utils::encrypt(file_path)};

			if (global::g_file_hash.count(hash) == 0){
				global::g_file_hash[hash] = file_path;
				std::string p{ global::server::g_resources / file_path };
				LogSystem::logInfo(std::format("[+] {} => {}", global::server::g_resources + "/" + hash, p));
			}

		} else if (fs::is_directory(entry)) {
			listFiles(entry.path());
		}
	}
}

// init
int _ { []() {
	LogSystem::initialized();
	namespace fs = std::filesystem;
	std::jthread t([&] {
		while (true) {
			std::string directoryPath{ global::server::g_resources };  // 替换为实际的目录路径
			if (fs::exists(directoryPath) && fs::is_directory(directoryPath)) {
				listFiles(directoryPath);
			}
			else {
				std::runtime_error("Invalid directory path.");
			};

			std::this_thread::sleep_for(std::chrono::milliseconds(global::server::g_refresh_time));

			std::vector<std::string> keysToDelete;

			for (const auto& [hash, path] : global::g_file_hash) {
				std::string p{ global::server::g_resources + "/" + path };
				if (not fs::exists(p)) {
					LogSystem::logInfo(std::format("[-] {} <= {}", hash, p));
					keysToDelete.push_back(hash);
				}
			}
			for (const auto& key : keysToDelete) {
				global::g_file_hash.erase(key);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(global::server::g_refresh_time));
		}
		});
	t.detach();
	
	return 0;
}() };

#if ENABLE_CONFIG
inline void start(void) {
	using drogon::app;

	LogSystem::logInfo(std::format("host: {}   port: {}   thread num: {}", global::server::g_host, global::server::g_port, global::server::g_thread_num));

	app().addListener(global::server::g_host, global::server::g_port);
	app().setThreadNum(global::server::g_thread_num);
	app().setDocumentRoot(global::server::g_document_root);
#else
inline void start(std::string && p, std::string && host, std::string && threadnumsv) {
	using drogon::app;

	std::size_t thread_num{ threadnumsv.empty() ? app().getThreadNum() : std::stoi(threadnumsv) };

	common::utils::createDirector("logs");

	const uint16_t port{ static_cast<uint16_t>(std::stoul(p.data())) };

	LogSystem::logInfo(std::format("host: {}   port: {}   thread num: {}", host, port, thread_num));

	app().addListener(host.data(), port);
	app().setThreadNum(thread_num);
	app().setDocumentRoot("static");
#endif // ENABLE_CONFIG
	app().setLogLevel(trantor::Logger::kInfo);
	// app().setFileTypes({ "html" });
	app().run();
}

#endif // !__CXX20_MAIN_HPP
