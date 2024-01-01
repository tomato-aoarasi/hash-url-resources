#include "demo_v1_User.hpp"

void HttpTestController::setInfo(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, User&& user) {
	auto resp = HttpResponse::newHttpResponse();
	resp->setStatusCode(k200OK);
	resp->setContentTypeCode(ContentType::CT_TEXT_HTML);
	if (user.userId.empty() || user.account.empty() || user.passwd.empty()) {
		resp->setBody("Error!");
	}
	else {
		resp->setBody("User " + user.userId + " saved!");
	}
	callback(resp);
}

void HttpTestController::getInfo(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, std::string userId, const std::string& token) const {
	auto resp = HttpResponse::newHttpResponse();
	resp->setStatusCode(k200OK);
	resp->setContentTypeCode(ContentType::CT_TEXT_HTML);
	resp->setBody("id: " + userId + "<br/>account: " + "123456" + "<br/>passwd: " + "******" + "<br/>token: " + token);
	callback(resp);
}