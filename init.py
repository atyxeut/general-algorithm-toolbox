# Copyright 2026 atyxeut
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import argparse
import shutil
import subprocess
from pathlib import Path

# > python3 init.py llvm
#   by default, specify `llvm` toolchain
# > python3 init.py llvm release
#   specify `llvm` toolchain and build in release mode
# > python3 init.py gcc
#   specify `gcc` toolchain
# > python3 init.py gcc release
#   specify `gcc` toolchain and build in release mode
parser = argparse.ArgumentParser(add_help=False)
parser.add_argument("toolchain", type=str)
parser.add_argument("build_mode", type=str, nargs="?", default="debug")
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

  subprocess.run(["xmake", "f", "-v", "--toolchain=" + argv.toolchain, "-m", argv.build_mode])
  subprocess.run(["xmake", "project", "-k", "compile_commands", "--outputdir=build"], text=True, stdout=subprocess.PIPE)


if __name__ == "__main__":
  main()
