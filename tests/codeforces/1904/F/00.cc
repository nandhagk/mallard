#include <bits/stdc++.h>

#include "lib/hld.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"
#include "lib/topological_sort.h"

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
        m = 3 * n;

        for (u32 i = 2; i < n + n; ++i) f(uid(i / 2), uid(i));
        for (u32 i = 2; i < n + n; ++i) f(lid(i), lid(i / 2));
    }

    template <std::invocable<u32, u32> F>
    static void add(u32 u, u32 v, F f) noexcept {
        f(u, v);
    }

    template <std::invocable<u32, u32> F>
    void add_from(u32 l, u32 r, u32 v, F f) const noexcept {
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) add(lid(l++), v, std::forward<F>(f));
            if (r & 1) add(lid(--r), v, std::forward<F>(f));
        }
    }

    template <std::invocable<u32, u32> F>
    void add_to(u32 u, u32 l, u32 r, F f) const noexcept {
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) add(u, uid(l++), std::forward<F>(f));
            if (r & 1) add(u, uid(--r), std::forward<F>(f));
        }
    }

    template <std::invocable<u32, u32> F>
    void add_range(u32 ul, u32 ur, u32 vl, u32 vr, F f) noexcept {
        u32 z = m++;

        add_from(ul, ur, z, std::forward<F>(f));
        add_to(z, vl, vr, std::forward<F>(f));
    }

private:
    u32 n, m;

    [[nodiscard]] constexpr u32 uid(u32 i) const noexcept {
        return i >= n ? i - n : n + i;
    }

    [[nodiscard]] constexpr u32 lid(u32 i) const noexcept {
        return i >= n ? i - n : n + n + i;
    }
};

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * n);

    for (u32 i = 1; i < n; ++i) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        e.emplace_back(u, v);
        e.emplace_back(v, u);
    }

    mld::static_csr g(n, e);
    mld::hld h(g);

    std::vector<std::pair<u32, u32>> f;
    f.reserve(m * std::bit_width(n));

    auto add = [&](u32 u, u32 v) {
        if (u != v) f.emplace_back(u, v);
    };

    range_graph rg(n, add);
    while (m--) {
        u32 t, a, b, c;
        std::cin >> t >> a >> b >> c;

        --a, --b, --c;
        for (auto &&[u, v] : h.decompose(a, b)) {
            u = h.tin[u], v = h.tin[v];
            if (u > v) std::swap(u, v);

            if (t == 1) {
                if (u <= h.tin[c] && h.tin[c] <= v) {
                    if (u != h.tin[c]) rg.add_to(h.tin[c], u, h.tin[c], add);
                    if (v != h.tin[c]) rg.add_to(h.tin[c], h.tin[c] + 1, v + 1, add);
                } else {
                    rg.add_to(h.tin[c], u, v + 1, add);
                }
            } else { //
                if (u <= h.tin[c] && h.tin[c] <= v) {
                    if (u != h.tin[c]) rg.add_from(u, h.tin[c], h.tin[c], add);
                    if (v != h.tin[c]) rg.add_from(h.tin[c] + 1, v + 1, h.tin[c], add);
                } else {
                    rg.add_from(u, v + 1, h.tin[c], add);
                }
            }
        }
    }

    mld::static_csr z(3 * n, f);
    auto res = mld::topological_sort(z);

    if (!res) {
        std::cout << -1 << '\n';
        return;
    }

    auto ord = *res;
    ord.erase(std::remove_if(ord.begin(), ord.end(), [&](u32 u) { return u >= n; }),
              ord.end());

    std::vector<u32> ans(n);

    u32 k = 0;
    for (u32 u : ord) ans[h.tor[u]] = k++;

    for (auto &&x : ans) std::cout << x + 1 << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
