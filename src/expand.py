from pathlib import Path
import re

SYSTEM_HEADERS = frozenset({
    # C++ headers
    "algorithm", "any", "array", "atomic", "barrier", "bit", "bitset", "cassert",
    "cctype", "cerrno", "cfenv", "cfloat", "charconv", "chrono", "cinttypes", "climits",
    "clocale", "cmath", "codecvt", "compare", "complex", "concepts",
    "condition_variable", "coroutine", "csetjmp", "csignal", "cstdarg", "cstddef",
    "cstdint", "cstdio", "cstdlib", "cstring", "ctime", "cuchar", "cwchar", "cwctype",
    "deque", "exception", "execution", "expected", "filesystem", "flat_map", "flat_set",
    "format", "forward_list", "fstream", "functional", "future", "generator",
    "initializer_list", "iomanip", "ios", "iosfwd", "iostream", "istream", "iterator",
    "latch", "limits", "list", "locale", "map", "mdspan", "memory", "memory_resource",
    "mutex", "new", "numbers", "numeric", "optional", "ostream", "print", "queue",
    "random", "ranges", "ratio", "regex", "scoped_allocator", "semaphore", "set",
    "shared_mutex", "source_location", "span", "spanstream", "sstream", "stack",
    "stacktrace", "stdexcept", "stdfloat", "stop_token", "streambuf", "string",
    "string_view", "strstream", "syncstream", "system_error", "thread", "tuple",
    "type_traits", "typeindex", "typeinfo", "unordered_map", "unordered_set", "utility",
    "valarray", "variant", "vector", "version",
    # C headers
    "assert.h", "complex.h", "ctype.h", "errno.h", "fenv.h", "float.h", "inttypes.h",
    "iso646.h", "limits.h", "locale.h", "math.h", "setjmp.h", "signal.h", "stdalign.h",
    "stdarg.h", "stdatomic.h", "stdbit.h", "stdbool.h", "stdckdint.h", "stddef.h",
    "stdint.h", "stdio.h", "stdlib.h", "stdnoreturn.h", "string.h", "tgmath.h",
    "threads.h", "time.h", "uchar.h", "wchar.h", "wctype.h"
})

EXTENDED_HEADERS = frozenset({
    "bits/stdc++.h",
    "ext/pb_ds/assoc_container.hpp",
    "ext/pb_ds/tree_policy.hpp",
    "tr2/dynamic_bitset",
})

INCLUDE_LEN = len("#include")
LIB_DIR = Path.cwd() / "include"

DEBUG_PATH = LIB_DIR / "lib" / "debug.h"
DEBUG_EXPANSION = (
    '#ifdef NANDHAGK_LOCAL',
    '#include "/home/nandhagk/Projects/mallard/include/lib/debug.h"',
    '#else',
    '#define debug(...)',
    '#endif // NANDHAGK_LOCAL',
)

NEWLINE_RE = re.compile(r"\n\s*\n")

def __line_directive(lineno: int, src_path: Path) -> str:
    return f'#line {lineno} "{src_path}"'


def __expand(src_path: Path, included_paths: set[Path], *, add_lineno: bool = False) -> list[str]:
    src_code = src_path.read_text()

    result = []
    if add_lineno:
        result.append(__line_directive(1, src_path))

    for lineno, line in enumerate(src_code.splitlines(), 1):
        if "MALLARD" in line:
            continue

        stripped_line = line.strip()
        if not stripped_line.startswith("#include"):
            result.append(line)
            continue

        included_path = stripped_line[INCLUDE_LEN:].strip()[1:-1]
        if included_path in SYSTEM_HEADERS:
            continue

        if included_path in EXTENDED_HEADERS:
            result.append(line)
            continue

        included_path = LIB_DIR / included_path
        if not included_path.exists():
            print(f"Failed to expand: {included_path}")
            continue

        if included_path in included_paths:
            continue

        included_paths.add(included_path)
        if included_path == DEBUG_PATH:
            result.extend(DEBUG_EXPANSION)
        else:
            result.extend(__expand(included_path, included_paths, add_lineno=add_lineno))

        if add_lineno:
            result.append(__line_directive(lineno + 1, src_path))

    return result


def expand(src_path: Path, *, add_lineno: bool = False) -> str:
    """Inline `#include` in src file."""
    result = "\n".join(__expand(src_path, {src_path}, add_lineno=add_lineno))
    return NEWLINE_RE.sub("\n\n", result)
