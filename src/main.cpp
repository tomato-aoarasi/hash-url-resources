#include "main.hpp"

int main(int argc, char* argv[]) {
#if !ENABLE_CONFIG
    std::string port{ "80" }, host{ "0.0.0.0" }, threadnum{};
    argument_init(argc, std::move(argv), port, host, threadnum);
    start(std::move(port), std::move(host), std::move(threadnum));
#else
    start();
#endif
    return 0;
}