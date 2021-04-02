#pragma once

namespace renderer {
struct ColorRGBA32 {
    char val[4];
    [[nodiscard]] inline const char &r() const noexcept { return val[0]; }
    [[nodiscard]] inline const char &g() const noexcept { return val[1]; }
    [[nodiscard]] inline const char &b() const noexcept { return val[2]; }
    [[nodiscard]] inline const char &a() const noexcept { return val[3]; }
    [[nodiscard]] inline char &r() noexcept { return val[0]; }
    [[nodiscard]] inline char &g() noexcept { return val[1]; }
    [[nodiscard]] inline char &b() noexcept { return val[2]; }
    [[nodiscard]] inline char &a() noexcept { return val[3]; }
};
}  // namespace renderer