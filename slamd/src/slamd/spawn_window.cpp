#include <fmt/format.h>
#include <sstream>
#ifdef _WIN32
#include <process.h>
#else
#include <spawn.h>
extern char** environ;
#endif
#include <spdlog/spdlog.h>
#include <filesystem>
#include <slamd/spawn_window.hpp>
namespace slamd {

std::string shell_escape(
    const std::string& input
) {
    std::stringstream ss;
    ss << '\'';
    for (char c : input) {
        if (c == '\'') {
            ss << "'\\''";  // end, escape, reopen
        } else {
            ss << c;
        }
    }
    ss << '\'';
    return ss.str();
}

void spawn_window(
    uint16_t port,
    std::optional<std::string> exe_path
) {
    std::filesystem::path executable_path(exe_path.value_or(EXEC_PATH));

    if (!std::filesystem::exists(executable_path)) {
        throw std::runtime_error(
            fmt::format("Executable {} not found", executable_path.string())
        );
    }

    std::string port_arg = fmt::format("{}", port);

    char* const argv[] =
        {(char*)"slamd_window", (char*)"--port", (char*)port_arg.c_str(), NULL};

#ifdef _WIN32
    intptr_t result = _spawnv(_P_NOWAIT, executable_path.string().c_str(), argv);
    if (result == -1) {
        throw std::runtime_error(fmt::format("Failed to spawn process {}", executable_path.string()));
    }
#else
    pid_t pid;
    posix_spawn(&pid, executable_path.c_str(), NULL, NULL, argv, environ);
#endif
}

}  // namespace slamd