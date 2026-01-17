#ifndef MALLARD_CIPOLLA_H
#define MALLARD_CIPOLLA_H 1

#include <optional>
#include <tuple>
#include <utility>

namespace mld {

template <typename Z>
[[nodiscard]] constexpr std::optional<Z> cipolla(Z a) noexcept {
    if (a.val() < 2) return a;

    auto k = (Z::mod() - 1) >> 1;
    if (a.pow(k) != 1) return std::nullopt;

    Z b = 1, D = 1 - a;
    for (; D.pow(k) == 1; b += 1, D = b * b - a);

    Z u = b, v = 1, w = 1, x = 0;
    for (++k; k != 0; k >>= 1, std::tie(u, v) = std::pair{u * u + D * v * v, 2 * u * v})
        if (k & 1) std::tie(w, x) = std::pair{u * w + D * v * x, u * x + v * w};

    return w;
}
} // namespace mld

#endif // MALLARD_CIPOLLA_H
