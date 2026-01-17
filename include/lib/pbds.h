#ifndef MALLARD_PBDS_H
#define MALLARD_PBDS_H 1

#include <ext/pb_ds/assoc_container.hpp>

#include "lib/hash.h"

namespace mld::pbds {
using namespace __gnu_pbds;

template <typename Key, typename Value, typename Hash = mld::hash<Key>>
using hash_map =
    gp_hash_table<Key, Value, Hash, std::equal_to<Key>, direct_mask_range_hashing<>,
                  linear_probe_fn<>,
                  hash_standard_resize_policy<hash_exponential_size_policy<>,
                                              hash_load_check_resize_trigger<>, true>>;

template <typename Key, typename Hash = mld::hash<Key>>
using hash_set = hash_map<Key, null_type, Hash>;

} // namespace mld::pbds

#endif // MALLARD_PBDS_H
