#pragma once
#include <iostream>
#include <drogon/HttpRequest.h>

struct User {
    std::string userId;
    std::string account;
    std::string passwd;
};

namespace drogon {
    template <>
    inline User fromRequest(const HttpRequest& req) {
        auto json = req.getJsonObject();
        User user;
        if (json) {
            user.userId = (*json)["userId"].asString();
            user.account = (*json)["account"].asString();
            user.passwd = (*json)["passwd"].asString();
        }
        return user;
    }
}