#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/sum.h"
#include "lib/dynamic_segment_tree.h"
#include "lib/managers/clonable.h"
#include "lib/prelude.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    using tree = mld::dynamic_segment_tree<mld::algebra::sum<u64>,
                                           mld::pmr::managers::clonable<>>;

    static std::array<std::byte, 1 << 27> buf;
    std::pmr::monotonic_buffer_resource mbr{buf.data(), buf.size(),
                                            std::pmr::null_memory_resource()};
    std::pmr::polymorphic_allocator pa{&mbr};

    std::vector<tree> st;
    st.reserve(q + 1);

    st.emplace_back(std::views::iota(u32{0}, n) | std::views::transform([](auto) {
                        u32 a;
                        std::cin >> a;
                        return a;
                    }),
                    pa);

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 1) {
            u32 k, a, x;
            std::cin >> k >> a >> x;

            st[--k].set(--a, x);
        } else if (t == 2) {
            u32 k, a, b;
            std::cin >> k >> a >> b;

            std::cout << (st[--k].fold(--a, b).val()) << '\n';
        } else {
            u32 k;
            std::cin >> k;

            st.emplace_back(st[--k]);
        }
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
