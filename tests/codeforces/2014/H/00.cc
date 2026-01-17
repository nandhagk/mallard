#include <bits/stdc++.h>

#include "lib/offline/mo.h"
#include "lib/prelude.h"

static constexpr u32 N = 1'000'000;
std::vector<u32> cnt;

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x, --x;

    mld::offline::mo m(n);
    m.reserve(q);

    std::vector<bool> ans(q);
    while (q--) {
        u32 l, r;
        std::cin >> l >> r;

        m.query(--l, r);
    }

    u32 o = 0;
    m.solve(
        [&](u32 i) {
            if (++cnt[a[i]] & 1)
                ++o;
            else
                --o;
        },
        [&](u32 i) {
            if (cnt[a[i]]-- & 1)
                --o;
            else
                ++o;
        },
        [&](u32 i) { ans[i] = o == 0; });

    for (auto &&x : ans) std::cout << (x ? "YES\n" : "NO\n");
    for (auto &&x : a) cnt[x] = 0;
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    cnt.resize(N);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
