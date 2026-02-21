#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"

template <typename T>
using min_heap = std::priority_queue<T, std::vector<T>, std::greater<T>>;

void solve() {
    u32 n, x;
    std::cin >> n >> x;

    std::vector<u32> a(x);
    for (auto &&y : a) std::cin >> y;

    min_heap<u32> q(std::greater<u32>{}, std::move(a));

    u64 ans = 0;
    while (q.size() > 1) {
        u32 u = q.top();
        q.pop();

        u32 v = q.top();
        q.pop();

        u32 w = u + v;
        q.push(w);
        ans += w;
    }

    std::cout << ans << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
