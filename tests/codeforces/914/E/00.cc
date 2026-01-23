#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/ctd.h"
#include "lib/prelude.h"
#include "lib/rooted_tree.h"
#include "lib/rooted_tree_lca.h"
#include "lib/static_csr.h"

static constexpr u32 N = 20;

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * n);

    for (u32 i = 1; i < n; ++i) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        e.emplace_back(u, v);
        e.emplace_back(v, u);
    }

    std::string s;
    std::cin >> s;

    mld::static_csr g(n, e);
    mld::rooted_tree_lca h(g);

    std::vector<u32> dp(n);
    for (u32 u = 0; u < n; ++u) dp[u] = 1 << (s[u] - 'a');

    auto kp = dp;
    for (u32 u : h.tor | std::views::drop(1)) dp[u] ^= dp[h.par[u]];

    auto pxor = [&](u32 u, u32 v) { return dp[u] ^ dp[v] ^ kp[h.lca(u, v)]; };

    std::vector<u64> tp(n);
    auto padd = [&](u32 u, u32 v, u64 a) {
        u32 w = h.lca(u, v);
        tp[u] += a, tp[v] += a, tp[w] -= a;
        if (w != 0) tp[h.par[w]] -= a;
    };

    auto [r, par] = mld::ctd(g);

    std::vector<std::pair<u32, u32>> f;
    f.reserve(n);
    for (u32 u = 0; u < n; ++u)
        if (u != r) f.emplace_back(par[u], u);

    mld::static_csr z(n, f);
    mld::rooted_tree y(z, r);

    std::vector<u64> cnt(1 << N);
    std::vector<u64> ans(n);
    for (u32 u : y.tor) {
        for (u32 i = 0; i < y.siz[u]; ++i) {
            u32 v = y.tor[y.tin[u] + i];
            u32 w = pxor(u, v);
            if (w == 0 || std::has_single_bit(w)) padd(u, v, 1);
            ++cnt[w];
        }
        --cnt[kp[u]];

        for (u32 k : z[u]) {
            if (k == y.par[u]) continue;

            for (u32 i = 0; i < y.siz[k]; ++i) {
                u32 v = y.tor[y.tin[k] + i];
                u32 w = pxor(u, v);
                --cnt[w];
            }

            for (u32 i = 0; i < y.siz[k]; ++i) {
                u32 v = y.tor[y.tin[k] + i];
                u32 w = pxor(u, v);
                u64 a = cnt[w ^ kp[u]];
                for (u32 j = 0; j < N; ++j) a += cnt[w ^ kp[u] ^ (1 << j)];
                padd(u, v, a), ans[u] += a;
            }

            for (u32 i = 0; i < y.siz[k]; ++i) {
                u32 v = y.tor[y.tin[k] + i];
                u32 w = pxor(u, v);
                ++cnt[w];
            }
        }

        for (u32 i = 1; i < y.siz[u]; ++i) {
            u32 v = y.tor[y.tin[u] + i];
            u32 w = pxor(u, v);
            --cnt[w];
        }
    }

    for (u32 u : h.tor | std::views::drop(1) | std::views::reverse)
        tp[h.par[u]] += tp[u];

    for (u32 u = 0; u < n; ++u) std::cout << tp[u] - (ans[u] >> 1) << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
