import argparse
import shutil
import subprocess
from pathlib import Path

# > python3 init.py
#   by default, specify `my_clang` toolchain
# > python3 init.py my_gcc
#   specify `my_gcc` toolchain
parser = argparse.ArgumentParser(add_help=False)
parser.add_argument("toolchain", type=str, nargs="?", default="my_clang")
argv = parser.parse_args()


def remove(path: Path):
  if not path.exists():
    return
  if path.is_dir():
    shutil.rmtree(path)
  else:
    path.unlink()


def main():
  remove(Path(".cache"))
  remove(Path(".xmake"))
  remove(Path("build"))

  xmake_toolchain = argv.toolchain
  subprocess.run(["xmake", "f", "-v", "--toolchain=" + xmake_toolchain])
  subprocess.run(
    ["xmake", "project", "-k", "compile_commands", "--outputdir=build"],
    text=True,
    stdout=subprocess.PIPE,
  )


if __name__ == "__main__":
  main()
