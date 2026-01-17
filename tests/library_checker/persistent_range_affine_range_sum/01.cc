#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/actions/affine_sum.h"
#include "lib/dynamic_lazy_segment_tree.h"
#include "lib/managers/clonable.h"
#include "lib/prelude.h"
#include "lib/static_modint.h"

using Z = mld::modint998244353;

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    using action = mld::actions::affine_sum<Z>;
    using tree = mld::dynamic_lazy_segment_tree<action, mld::pmr::managers::clonable<>>;

    static std::array<std::byte, 1 << 28> buf;
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
        i32 k;
        std::cin >> t >> k;

        if (t == 0) {
            u32 l, r, a, b;
            std::cin >> l >> r >> a >> b;

            st.emplace_back(st[k + 1]);
            st.back().apply(l, r, {a, b});
        } else if (t == 1) {
            i32 s;
            u32 l, r;
            std::cin >> s >> l >> r;

            st.emplace_back(st[k + 1]);
            st.back().copy(st[s + 1], l, r);
        } else {
            u32 l, r;
            std::cin >> l >> r;

            st.emplace_back(st.back());
            std::cout << st[k + 1].fold(l, r).val().val() << '\n';
        }
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
