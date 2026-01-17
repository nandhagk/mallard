#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/basic_dynamic_sequence.h"
#include "lib/managers/reusing.h"
#include "lib/prelude.h"
#include "lib/splay_tree.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::string s;
    std::cin >> s;

    static std::array<std::byte, 1 << 26> buf;
    std::pmr::monotonic_buffer_resource mbr{buf.data(), buf.size(),
                                            std::pmr::null_memory_resource()};

    std::pmr::polymorphic_allocator pa{&mbr};

    mld::basic_dynamic_sequence<char, mld::splay_tree, mld::pmr::managers::reusing<>>
        ds(s, pa);

    std::string t;
    t.reserve(n + 1);

    while (q--) {
        u32 l, r;
        std::cin >> l >> r;

        ds.reverse(--l, r);
    }

    ds.walk([&](char c) { t += c; });
    std::cout << t;
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
