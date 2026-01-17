#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/managers/clonable.h"
#include "lib/prelude.h"
#include "lib/static_mex.h"

void solve() {
    u32 n, k;
    std::cin >> n >> k;

    static std::array<std::byte, 1 << 29> buf;
    std::pmr::monotonic_buffer_resource mbr{buf.data(), buf.size(),
                                            std::pmr::null_memory_resource()};

    std::pmr::polymorphic_allocator pa{&mbr};

    mld::static_mex<mld::pmr::managers::clonable<>> sm(
        std::views::iota(u32{0}, n) | std::views::transform([](auto) {
            u32 a;
            std::cin >> a;
            return a;
        }),
        pa);

    for (u32 i = k; i <= n; ++i) std::cout << sm.mex(i - k, i) << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
