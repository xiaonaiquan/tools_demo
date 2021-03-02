#include <iostream>
#include <functional>
#include <initializer_list>

namespace go {
    using std::string;
    namespace time {
        using Duration = int64_t;
        constexpr auto Second = 1LL;
    } // namespace time
    namespace tls {
        using Config = string;
    }

    struct Server {
        string addr;
        int port;
        string protocol;
        time::Duration timeout;
        int max_conns;
        tls::Config *tls_;
    };

    using Option = std::function<void(Server &s)>;

    auto Protocol(string p) -> Option
    {
        return [p](Server &s) { s.protocol = p; };
    }
    auto Timeout(time::Duration timeout) -> Option
    {
        return [timeout](Server &s) { s.timeout = timeout; };
    }
    auto MaxConns(int max_conns) -> Option
    {
        return [max_conns](Server &s) { s.max_conns = max_conns; };
    }
    auto TLS(tls::Config *tls_) -> Option
    {
        return [tls_](Server &s) { s.tls_ = tls_; };
    }

// 忘记默认值了
    Server NewServer(string addr,
                     int port,
                     std::initializer_list<Option> options = {})
    {
        Server s{ addr, port, "tcp", 30 * time::Second, 1000, nullptr };
        for (auto &option : options) {
            option(s);
        }
        return s;
    }

    void UsageServer()
    {
        auto s1 = NewServer("localhost", 1024);
        auto s2 = NewServer("localhost", 2048, { Protocol("udp") });
        auto s3 = NewServer("0.0.0.0", 8080,
                            { Timeout(300 * time::Second), MaxConns(1000) });
    }
} // namespace go

int main() {
    using namespace go;
    auto s1 = NewServer("localhost", 1024);
    auto s2 = NewServer("localhost", 2048, {Protocol("udp")});
    auto s3 = NewServer("0.0.0.0", 8080,{ Timeout(300 * time::Second), MaxConns(1000) });
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
