#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/booth.h"
#include "lib/prelude.h"

void solve() {
    std::string s;
    std::cin >> s;

    std::rotate(s.begin(), s.begin() + mld::booth(s), s.end());
    std::cout << s << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
