#pragma once

#include <drogon/HttpController.h>
#include "User.hpp"

using namespace drogon;

class HttpTestController : public drogon::HttpController<HttpTestController>
{
public:
    METHOD_LIST_BEGIN
        // 添加两个路径映射
        ADD_METHOD_TO(HttpTestController::setInfo, "/user/set", Post);
        ADD_METHOD_TO(HttpTestController::getInfo, "/{1:userId}/info?token={2}", Get);
    METHOD_LIST_END
    void setInfo(const HttpRequestPtr& req,
        std::function<void(const HttpResponsePtr&)>&& callback,
        User&& user);
    void getInfo(const HttpRequestPtr& req,
        std::function<void(const HttpResponsePtr&)>&& callback,
        std::string userId,
        const std::string& token) const;
};