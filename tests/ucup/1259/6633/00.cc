#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/two_sat.h"

// TODO: Add this to template
struct range_graph {
    range_graph() noexcept = default;

    template <std::invocable<u32, u32> F>
    range_graph(u32 p, F f) noexcept {
        build(p, f);
    }

    template <std::invocable<u32, u32> F>
    void build(u32 p, F f) noexcept {
        n = p;
        m = 2 * n;

        for (u32 i = 2; i < n + n; ++i) f(uid(i / 2), uid(i));
    }

    template <std::invocable<u32, u32> F>
    static void add(u32 u, u32 v, F f) noexcept {
        f(u, v);
    }

    template <std::invocable<u32, u32> F>
    void add_to(u32 u, u32 l, u32 r, F f) const noexcept {
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) add(u, uid(l++), std::forward<F>(f));
            if (r & 1) add(u, uid(--r), std::forward<F>(f));
        }
    }

private:
    u32 n, m;

    [[nodiscard]] constexpr u32 uid(u32 i) const noexcept {
        return i >= n ? i - n : n + i;
    }
};

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<u32> p(n), q(n);
    for (u32 i = 0; i < n; ++i) std::cin >> p[i] >> q[i];

    std::vector<u32> l(n);
    std::iota(l.begin(), l.end(), 0);
    std::ranges::sort(l, {}, [&](u32 i) { return p[i]; });

    std::vector<u32> r(n);
    std::iota(r.begin(), r.end(), 0);
    std::ranges::sort(r, {}, [&](u32 i) { return q[i]; });

    std::vector<u32> a(n), b(n);
    for (u32 i = 0; i < n; ++i) a[l[i]] = i;
    for (u32 i = 0; i < n; ++i) b[r[i]] = i;

    mld::two_sat ts(4 * n);
    for (u32 i = 0; i < n; ++i) ts.add_clause(a[i], 0, b[i] + 2 * n, 1);
    for (u32 i = 0; i < n; ++i) ts.add_clause(b[i] + 2 * n, 0, a[i], 1);

    range_graph gl(n, [&](u32 u, u32 v) { ts.add_clause(u, 1, v, 0); });
    range_graph gr(n, [&](u32 u, u32 v) { ts.add_clause(u + 2 * n, 1, v + 2 * n, 0); });

    for (u32 i = 0; i < n; ++i) {
        u32 x = static_cast<u32>(
            std::ranges::lower_bound(l, p[i], {}, [&](u32 j) { return p[j]; }) -
            l.begin());
        u32 y = static_cast<u32>(
            std::ranges::upper_bound(l, q[i], {}, [&](u32 j) { return p[j]; }) -
            l.begin());

        assert(x <= a[i] && a[i] < y);

        auto f = [&](u32 u, u32 v) { ts.add_clause(u, 0, v, 0); };
        if (a[i] > x) gl.add_to(a[i], x, a[i], f);
        if (y > a[i] + 1) gl.add_to(a[i], a[i] + 1, y, f);
    }

    for (u32 i = 0; i < n; ++i) {
        u32 x = static_cast<u32>(
            std::ranges::lower_bound(r, p[i], {}, [&](u32 j) { return q[j]; }) -
            r.begin());
        u32 y = static_cast<u32>(
            std::ranges::upper_bound(r, q[i], {}, [&](u32 j) { return q[j]; }) -
            r.begin());

        assert(x <= b[i] && b[i] < y);

        auto f = [&](u32 u, u32 v) { ts.add_clause(u + 2 * n, 0, v + 2 * n, 0); };
        if (b[i] > x) gr.add_to(b[i], x, b[i], f);
        if (y > b[i] + 1) gr.add_to(b[i], b[i] + 1, y, f);
    }

    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        ts.add_clause(a[u], 1, a[v], 1);
        ts.add_clause(b[u] + 2 * n, 1, b[v] + 2 * n, 1);
    }

    std::cout << (ts.satisfiable() ? "YES\n" : "NO\n");
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
