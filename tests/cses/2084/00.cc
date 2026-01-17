#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/lichao_tree.h"
#include "lib/managers/reusing.h"
#include "lib/prelude.h"

void solve() {
    u32 n, x;
    std::cin >> n >> x;

    std::vector<u32> s(n), f(n);
    for (auto &&z : s) std::cin >> z;
    for (auto &&z : f) std::cin >> z;

    static std::array<std::byte, 1 << 29> buf;
    std::pmr::monotonic_buffer_resource mbr{buf.data(), buf.size(),
                                            std::pmr::null_memory_resource()};

    std::pmr::polymorphic_allocator pa{&mbr};

    using line = mld::internal::line<u64>;
    mld::lichao_tree<u32, line, mld::pmr::managers::reusing<>> lch(0, 1'000'001, pa);

    lch.add(line(x, 0));
    for (u32 i = 0; i < n - 1; ++i) lch.add(line(f[i], lch.query(s[i])(s[i])));

    std::cout << lch.query(s.back())(s.back()) << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
