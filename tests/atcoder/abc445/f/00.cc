#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/offline/rectangle_union.h"
#include "lib/prelude.h"

void solve() {
    u32 H, W, h, w, n;
    std::cin >> H >> W >> h >> w >> n;

    u64 ans = u64(H - h + 1) * (W - w + 1);
    if (n == 0) {
        std::cout << ans << '\n';
        return;
    }

    mld::offline::rectangle_union<u32> ru;
    ru.reserve(n);

    while (n--) {
        u32 r, c;
        std::cin >> r >> c;

        u32 x = r > h ? r - h : 0;
        u32 y = c > w ? c - w : 0;
        r = std::min(r, H - h + 1);
        c = std::min(c, W - w + 1);

        ru.add_rectangle(x, y, r, c);
    }

    ans -= ru.area();
    std::cout << ans << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
