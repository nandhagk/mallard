#include <bits/stdc++.h>

#include "lib/pbds.h"
#include "lib/prelude.h"

void solve() {
    u32 n;
    std::cin >> n;

    mld::pbds::hash_map<u32, u32> cnt;
    cnt.resize(n);

    for (u32 i = 0; i < n; ++i) {
        u32 a;
        std::cin >> a;
        ++cnt[a];
    }

    std::vector<u32> f;
    f.reserve(n);

    u64 x = 0, y = 0;
    for (auto &&[p, c] : cnt) {
        u64 z = u64(p / 2) * c;
        x += z, y += z;
        if (p % 2) f.push_back(c);
    }

    std::ranges::sort(f, std::greater{});
    for (u32 i = 0; i < f.size(); ++i)
        if (i % 2)
            y += f[i];
        else
            x += f[i];

    std::cout << x << ' ' << y << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
