#ifndef MALLARD_DYNAMIC_CSR_H
#define MALLARD_DYNAMIC_CSR_H 1

#include <cassert>
#include <concepts>
#include <vector>

#include "lib/prelude.h"

namespace mld {
template <typename T>
struct dynamic_csr {
    using size_type = u32;
    using value_type = T;

private:
    size_type len;
    std::vector<size_type> cur, siz, nxt;
    std::vector<value_type> buf;

    static constexpr size_type UNSET = -1;

public:
    constexpr dynamic_csr() noexcept = default;

    explicit constexpr dynamic_csr(size_type n) noexcept {
        build(n);
    }

    constexpr void build(size_type n) noexcept {
        len = n;
        cur.assign(len, UNSET);
        siz.assign(len, 0);
        nxt.clear();
        buf.clear();
    }

    constexpr void reserve(size_type n) noexcept {
        nxt.reserve(n);
        buf.reserve(n);
    }

    struct span_iterator {
    private:
        size_type k;
        const dynamic_csr &g;

    public:
        constexpr span_iterator(size_type l, const dynamic_csr &h) noexcept
            : k(l), g(h) {}

        [[nodiscard]] constexpr auto operator*() const noexcept {
            return g.buf[k];
        }

        constexpr span_iterator &operator++() & noexcept {
            k = g.nxt[k];
            return *this;
        }

        [[nodiscard]] friend constexpr bool
        operator==(const span_iterator &lhs, const span_iterator &rhs) noexcept {
            return lhs.k == rhs.k;
        }
    };

    struct const_span {
    private:
        size_type u;
        const dynamic_csr &g;

    public:
        constexpr const_span(size_type v, const dynamic_csr &h) noexcept
            : u(v), g(h) {}

        [[nodiscard]] constexpr span_iterator begin() const noexcept {
            return span_iterator(g.cur[u], g);
        }

        [[nodiscard]] constexpr span_iterator end() const noexcept {
            return span_iterator(UNSET, g);
        }

        [[nodiscard]] constexpr size_type size() const noexcept {
            return g.siz[u];
        }

        [[nodiscard]] constexpr bool empty() const noexcept {
            return g.siz[u] == 0;
        }
    };

    struct span {
    private:
        size_type u;
        dynamic_csr &g;

    public:
        constexpr span(size_type v, dynamic_csr &h) noexcept
            : u(v), g(h) {}

        template <typename... Args>
            requires std::constructible_from<value_type, Args...>
        constexpr void emplace_back(Args... args) noexcept {
            g.buf.emplace_back(std::forward<Args>(args)...);
            g.nxt.emplace_back(g.cur[u]);
            g.cur[u] = static_cast<size_type>(g.buf.size()) - 1;
            ++g.siz[u];
        }

        [[nodiscard]] constexpr span_iterator begin() const noexcept {
            return span_iterator(g.cur[u], g);
        }

        [[nodiscard]] constexpr span_iterator end() const noexcept {
            return span_iterator(UNSET, g);
        }

        [[nodiscard]] constexpr size_type size() const noexcept {
            return g.siz[u];
        }

        [[nodiscard]] constexpr bool empty() const noexcept {
            return g.siz[u] == 0;
        }

        constexpr void clear() noexcept {
            g.cur[u] = UNSET;
            g.siz[u] = 0;
        }
    };

    [[nodiscard]] constexpr span operator[](size_type u) noexcept {
        return span(u, *this);
    }

    [[nodiscard]] constexpr const_span operator[](size_type u) const noexcept {
        return const_span(u, *this);
    }

    [[nodiscard]] constexpr size_type size() const noexcept {
        return len;
    }

    struct iterator {
    private:
        size_type k;
        const dynamic_csr &g;

    public:
        constexpr iterator(size_type j, const dynamic_csr &h)
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

#endif // MALLARD_DYNAMIC_CSR_H
