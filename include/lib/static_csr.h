#ifndef MALLARD_STATIC_CSR_H
#define MALLARD_STATIC_CSR_H 1

#include <numeric>
#include <ranges>
#include <span>
#include <vector>

#include "lib/prelude.h"

namespace mld {

template <typename R>
concept static_csr_input_range =
    std::ranges::input_range<R> && std::ranges::sized_range<R> &&
    std::unsigned_integral<std::remove_cvref_t<std::ranges::range_value_t<R>>>;

template <typename T>
struct static_csr {
    using value_type = T;

    using size_type = u32;
    using container_type = std::vector<value_type>;

protected:
    size_type len;
    container_type buf;
    std::vector<size_type> fst;

public:
    constexpr static_csr() noexcept = default;

    template <static_csr_input_range R>
    explicit constexpr static_csr(R &&r) noexcept {
        build(std::forward<R>(r));
    }

    constexpr static_csr(
        size_type n, const std::vector<std::pair<size_type, value_type>> &e) noexcept {
        build(n, e);
    }

    template <static_csr_input_range R>
    constexpr void build(R &&r) noexcept {
        len = static_cast<size_type>(std::ranges::size(r));
        fst.resize(len + 1);

        std::inclusive_scan(
            std::ranges::begin(r), std::ranges::end(r), fst.begin() + 1,
            [](size_type a, size_type b) { return a + b; }, size_type{0});

        buf.clear(), buf.resize(fst.back());
    }

    // TODO: Make this better
    constexpr void
    build(size_type n,
          const std::vector<std::pair<size_type, value_type>> &e) noexcept {
        len = n;
        fst.assign(len + 1, 0);

        for (auto &&[k, _] : e) ++fst[k];
        std::inclusive_scan(fst.begin(), fst.end(), fst.begin());
        fst.back() = static_cast<size_type>(e.size());

        buf.resize(fst.back());
        for (auto &&[k, d] : e) buf[--fst[k]] = d;
    }

    [[nodiscard]] constexpr size_type size() const noexcept {
        return len;
    }

    [[nodiscard]] constexpr auto data() noexcept {
        return buf;
    }

    [[nodiscard]] constexpr auto data() const noexcept {
        return buf;
    }

    [[nodiscard]] constexpr std::span<value_type> operator[](size_type k) noexcept {
        return {buf.data() + fst[k], buf.data() + fst[k + 1]};
    }

    [[nodiscard]] constexpr std::span<const value_type>
    operator[](size_type k) const noexcept {
        return {buf.data() + fst[k], buf.data() + fst[k + 1]};
    }

    struct iterator {
    private:
        size_type k;
        const static_csr &g;

    public:
        constexpr iterator(size_type j, const static_csr &h)
            : k(j), g(h) {}

        [[nodiscard]] constexpr auto operator*() const noexcept {
            return g.operator[](k);
        }

        constexpr iterator &operator++() & noexcept {
            return ++k, *this;
        }

        [[nodiscard]] friend constexpr bool operator==(const iterator &lhs,
                                                       const iterator &rhs) noexcept {
            return lhs.k == rhs.k;
        }
    };

    [[nodiscard]] constexpr iterator begin() const noexcept {
        return iterator(0, *this);
    }

    [[nodiscard]] constexpr iterator end() const noexcept {
        return iterator(len, *this);
    }
};
} // namespace mld

#endif // MALLARD_STATIC_CSR_H
