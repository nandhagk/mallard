#ifndef MALLARD_BINARY_TRIE_H
#define MALLARD_BINARY_TRIE_H 1

#include <concepts>
#include <limits>

#include "lib/internal/binary_trie.h"

namespace mld {
template <std::unsigned_integral WordT, typename Manager, u32 log = 2>
struct binary_trie : private internal::binary_trie<WordT, Manager, log> {
private:
    using base = internal::binary_trie<WordT, Manager, log>;

public:
    using typename base::word_type;

    using typename base::handler;
    using typename base::node;
    using typename base::pointer;

    using typename base::allocator_type;
    using typename base::size_type;

private:
    size_type len = 0;
    word_type xol = 0;
    pointer root = handler::nil;

public:
    constexpr ~binary_trie() noexcept {
        base::dispose(root);
    }

    constexpr binary_trie(const allocator_type &alloc = allocator_type()) noexcept
        : base(alloc) {
        root = base::hlr.create();
    }

    constexpr binary_trie(const binary_trie &other,
                          const allocator_type &alloc) noexcept
        : base(alloc), len(other.len), xol(other.xol), root(other.root) {}

    constexpr binary_trie(binary_trie &&other, const allocator_type &alloc) noexcept
        : base(alloc), len(other.len), xol(other.xol), root(other.root) {}

    [[nodiscard]] constexpr size_type size() const noexcept {
        return len;
    }

    constexpr void insert(word_type v) noexcept {
        ++len;
        base::insert(root, v, xol);
    }

    constexpr void erase(word_type v, size_type n) noexcept {
        len -= n;
        base::erase(root, v, xol, n);
    }

    [[nodiscard]] constexpr size_type count(word_type v) const noexcept {
        return base::count(root, v, xol);
    }

    constexpr void erase(word_type v) noexcept {
        if (size_type n = count(v)) erase(v, n);
    }

    [[nodiscard]] constexpr word_type kth(size_type k) const noexcept {
        assert(k < len);
        return base::kth(root, k, xol);
    }

    [[nodiscard]] constexpr size_type rank(word_type v) const noexcept {
        return base::rank(root, v, xol);
    }

    [[nodiscard]] constexpr word_type min() const noexcept {
        assert(len);
        return base::min(root, xol);
    }

    [[nodiscard]] constexpr word_type max() const noexcept {
        assert(len);
        return base::max(root, xol);
    }

    constexpr void apply(word_type z) noexcept {
        xol ^= z;
    }

    [[nodiscard]] constexpr word_type xor_min(word_type z) const noexcept {
        assert(len);
        return base::min(root, xol ^ z) ^ z;
    }

    [[nodiscard]] constexpr word_type xor_max(word_type z) const noexcept {
        assert(len);
        return base::max(root, xol ^ z) ^ z;
    }

    [[nodiscard]] constexpr std::optional<word_type>
    lower_bound(word_type v) const noexcept {
        size_type rnk = rank(v);
        if (rnk == len) return std::nullopt;
        return kth(rnk);
    }

    [[nodiscard]] constexpr std::optional<word_type>
    less_bound(word_type v) const noexcept {
        size_type rnk = v != std::numeric_limits<word_type>::max() ? rank(v + 1) : len;
        if (rnk == 0) return std::nullopt;
        return kth(rnk - 1);
    }
};
} // namespace mld

#endif // MALLARD_BINARY_TRIE_H
