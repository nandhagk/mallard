#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/sum.h"
#include "lib/dynamic_sequence.h"
#include "lib/managers/reusing.h"
#include "lib/prelude.h"
#include "lib/rbst.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    static std::array<std::byte, 1 << 25> buf;
    std::pmr::monotonic_buffer_resource mbr{buf.data(), buf.size(),
                                            std::pmr::null_memory_resource()};

    std::pmr::polymorphic_allocator pa{&mbr};

    mld::dynamic_sequence<mld::algebra::sum<u64>, mld::rbst,
                          mld::pmr::managers::reusing<>>
        ds(std::views::iota(u32{0}, n) | std::views::transform([](auto) {
               u32 a;
               std::cin >> a;
               return a;
           }),
           pa);

    while (q--) {
        u32 t, l, r;
        std::cin >> t >> l >> r;

        if (t == 0) {
            if (l < r) ds.reverse(l, r);
        } else {
            if (l == r)
                std::cout << u32{0} << '\n';
            else
                std::cout << ds.fold(l, r).val() << '\n';
        }
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
