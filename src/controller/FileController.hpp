#pragma once

#ifndef __CXX20_FILE_CONTROLLER
#define __CXX20_FILE_CONTROLLER

#include <filesystem>
#include <fstream>
#include <cctype>
#include <cstdlib>
#include <algorithm>
#include <drogon/HttpController.h>

namespace controller {
	namespace fs = std::filesystem;
	class FileController : public drogon::HttpController<FileController> {
	public:
		METHOD_LIST_BEGIN
			ADD_METHOD_TO(FileController::getResource, "/resources/{hash}", drogon::Get);
		METHOD_LIST_END
		void getResource(const drogon::HttpRequestPtr& req,
			std::function<void(const drogon::HttpResponsePtr&)>&& callback,
			const std::string& hash);
	};
}

#endif // __CXX20_FILE_CONTROLLER