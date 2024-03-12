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

    // ���ļ�
    std::ifstream ifs(filePath, std::ios::binary);
    if (!ifs.is_open()) {
        callback(drogon::HttpResponse::newNotFoundResponse());
        return;
    }

    // ��ȡ����
    std::string data((std::istreambuf_iterator<char>(ifs)),
        std::istreambuf_iterator<char>());

    // ��ȡ�ļ���׺��
    std::string extension{ filePath.extension().string() };
    extension = extension.substr(1);

    // ȫתСд
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

    if (global::g_content_type_dict.contains(extension)) {
        resp->addHeader("Content-Type", global::g_content_type_dict.at(extension));
    }
    resp->addHeader("Cache-Control", std::format("public, max-age={0}, s-maxage={0}", expires_i));
    resp->addHeader("Expires", expires);
    // Set response content
    resp->setBody(std::move(data));
    resp->addHeader("Content-Disposition", "inline");

    callback(resp);
}