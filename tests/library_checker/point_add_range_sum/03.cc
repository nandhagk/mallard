#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/sum.h"
#include "lib/dynamic_segment_tree.h"
#include "lib/managers/reusing.h"
#include "lib/prelude.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    static std::array<std::byte, 1 << 26> buf;
    std::pmr::monotonic_buffer_resource mbr{buf.data(), buf.size(),
                                            std::pmr::null_memory_resource()};
    std::pmr::polymorphic_allocator pa{&mbr};

    mld::dynamic_segment_tree<mld::algebra::sum<u64>, mld::pmr::managers::reusing<>> st(
        std::views::iota(u32{0}, n) | std::views::transform([](auto) {
            u32 a;
            std::cin >> a;
            return a;
        }),
        pa);

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 0) {
            u32 p, x;
            std::cin >> p >> x;

            st.add(p, x);
        } else {
            u32 l, r;
            std::cin >> l >> r;

            std::cout << st.fold(l, r).val() << '\n';
        }
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
