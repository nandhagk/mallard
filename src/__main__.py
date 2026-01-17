import subprocess
from pathlib import Path
from string import ascii_lowercase

import click

from src.expand import expand

CONTEST_DIR = Path.cwd() / "contest"
BUILD_DIR = Path.cwd() / "build"
TEST_DIR = Path.cwd() / "tests"

CXX_FLAGS = (
    "-std=c++20",
    "-O2",
    "-g",
    "-DNANDHAGK_LOCAL",
    "-fsanitize=address",
    "-fsanitize=undefined",
    "-Wall",
    "-Wextra",
    "-Wshadow",
    "-Wconversion",
    "-fconcepts-diagnostics-depth=10",
)


@click.group()
def main():
    """Entrypoint."""


@main.command()
@click.argument("test")
@click.argument("num", default=0)
@click.option("-d", "--debug", default=False, is_flag=True)
def test(test: str, num: int, debug: bool) -> None:
    """Test Runner."""
    Path.mkdir(BUILD_DIR, exist_ok=True)
    test_dir = TEST_DIR / test

    src_path = (test_dir / f"{num:02}").with_suffix(".cc")
    dst_code = expand(src_path, add_lineno=debug)

    dst_path = (test_dir / "out").with_suffix(".cc")
    dst_path.write_text(dst_code)

    subprocess.run(["clang-format", "-i", dst_path])
    print("Expanded", flush=True)

    exec_path = BUILD_DIR / f"{test.replace('/', '_')}_{num:02}"
    if exec_path.exists():
        exec_path.unlink()

    input_path = test_dir / "input.txt"
    subprocess.run(["g++", *CXX_FLAGS, dst_path, "-o", exec_path])

    print("Compiled", flush=True)
    with input_path.open("r") as f:
        subprocess.run([exec_path], stdin=f, check=False)


@main.command()
@click.argument("q")
@click.option("-d", "--debug", default=False, is_flag=True)
def run(q: str, debug: bool) -> None:
    """Runner."""
    Path.mkdir(BUILD_DIR, exist_ok=True)
    q_dir = CONTEST_DIR / q

    src_path = (q_dir / q).with_suffix(".cc")
    dst_code = expand(src_path, add_lineno=debug)

    dst_path = (CONTEST_DIR / "out").with_suffix(".cc")
    dst_path.write_text(dst_code)

    subprocess.run(["clang-format", "-i", dst_path])
    print("Expanded")

    exec_path = BUILD_DIR / q
    if exec_path.exists():
        exec_path.unlink()

    input_path = q_dir / "input.txt"

    subprocess.run(["g++", *CXX_FLAGS, dst_path, "-o", exec_path])

    print("Compiled")
    with input_path.open("r") as f:
        subprocess.run([exec_path], stdin=f, check=False)


@main.command()
def generate() -> None:
    """Generate template."""
    Path.mkdir(CONTEST_DIR, exist_ok=True)

    template_path = Path.cwd() / "src" / "template.cc"
    template = template_path.read_text()

    for q in ascii_lowercase:
        q_dir = CONTEST_DIR / q
        Path.mkdir(q_dir, exist_ok=True)

        src_path = (q_dir / q).with_suffix(".cc")
        src_path.write_text(template)

        input_path = q_dir / "input.txt"
        input_path.write_text("")


main()
