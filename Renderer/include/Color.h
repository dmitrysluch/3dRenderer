#pragma once

namespace renderer {
struct ColorRGBA32 {
    unsigned char val[4];
    [[nodiscard]] inline const unsigned char &r() const noexcept { return val[0]; }
    [[nodiscard]] inline const unsigned char &g() const noexcept { return val[1]; }
    [[nodiscard]] inline const unsigned char &b() const noexcept { return val[2]; }
    [[nodiscard]] inline const unsigned char &a() const noexcept { return val[3]; }
    [[nodiscard]] inline unsigned char &r() noexcept { return val[0]; }
    [[nodiscard]] inline unsigned char &g() noexcept { return val[1]; }
    [[nodiscard]] inline unsigned char &b() noexcept { return val[2]; }
    [[nodiscard]] inline unsigned char &a() noexcept { return val[3]; }
};
}  // namespace renderer