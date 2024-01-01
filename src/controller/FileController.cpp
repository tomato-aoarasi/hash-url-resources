#include "global.hpp"
#include "controller/FileController.hpp"

void controller::FileController::getResource(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback, const std::string& hash){
    // Construct the full path to the image file
    bool is_exist{ static_cast<bool>(global::g_file_hash.count(hash)) };

    if (not is_exist) {
        callback(drogon::HttpResponse::newNotFoundResponse());
        return;
    }

    fs::path filePath{ global::server::g_resources + "/" + global::g_file_hash.at(hash) };

    // Open the image file
    std::ifstream imageFile(filePath, std::ios::binary);
    if (!imageFile.is_open()) {
        callback(drogon::HttpResponse::newNotFoundResponse());
        return;
    }

    // Read the image content
    std::string imageData((std::istreambuf_iterator<char>(imageFile)),
        std::istreambuf_iterator<char>());

    // ��ȡ�ļ���׺��
    std::string extension{ filePath.extension().string() };
    extension = extension.substr(1);

    std::transform(extension.begin(), extension.end(), extension.begin(), [](unsigned char c) {
        return std::tolower(c);
    });

    // ���õ���ʱ��
    constexpr int expires_i{ 3600 };

    // ��ȡ��ǰʱ��
    auto now{ std::chrono::system_clock::now() };

    auto seven_days_later{ now + std::chrono::seconds(expires_i) };

    // ��ʱ��ת��Ϊʱ�����������
    auto timestamp{ std::chrono::system_clock::to_time_t(seven_days_later) };
    std::stringstream ss;
    std::string expires{ ss.str() };

    // Set response headers
    auto resp{ drogon::HttpResponse::newHttpResponse() };
    // resp->setStatusCode(drogon::k304NotModified);
    
    resp->addHeader("Accept-Range", "bytes");
    resp->addHeader("Content-Type", "image/" + extension);
    resp->addHeader("Cache-Control", std::format("public, max-age={0}, s-maxage={0}", expires_i));
    resp->addHeader("Expires", expires);
    // Set response content
    resp->setBody(std::move(imageData));
    resp->addHeader("Content-Disposition", "inline");

    callback(resp);
}