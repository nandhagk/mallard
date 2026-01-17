#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<i32> x(n), y(n);
    for (u32 i = 0; i < n; ++i) std::cin >> x[i] >> y[i];

    std::ranges::sort(x);
    std::ranges::sort(y);

    i128 a = 0;

    i64 p = 0;
    for (u32 i = 0; i < n; p += x[i] + y[i], ++i)
        a += (static_cast<i64>(x[i]) + y[i]) * i - p;

    std::string s;
    for (; a; a /= 10) s += static_cast<char>(a % 10 + '0');

    std::ranges::reverse(s);
    if (s.empty()) s += '0';

    std::cout << s << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
