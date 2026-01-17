#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/lichao_tree.h"
#include "lib/manacher.h"
#include "lib/managers/reusing.h"
#include "lib/prelude.h"

void solve() {
    std::string s;
    std::cin >> s;

    u32 n = static_cast<u32>(s.size());
    auto [odd, even] = mld::manacher(s);

    static std::array<std::byte, 1 << 29> buf;
    std::pmr::monotonic_buffer_resource mbr{buf.data(), buf.size(),
                                            std::pmr::null_memory_resource()};

    std::pmr::polymorphic_allocator pa{&mbr};

    using line = mld::internal::line<i32>;
    mld::lichao_tree<u32, line, mld::pmr::managers::reusing<>> lch(0, n + 1, pa);

    for (u32 i = 0; i < n; ++i) lch.add(i, i + odd[i], line(-2, 2 * i32(i) - 1));
    for (u32 i = 1; i < n; ++i) lch.add(i, i + even[i], line(-2, 2 * i32(i) - 2));

    for (u32 i = 0; i < n; ++i) std::cout << u32(-lch.query(i)(i)) << ' ';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
