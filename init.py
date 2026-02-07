import shutil
import subprocess
from pathlib import Path


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
    subprocess.run(["xmake", "f"])
    subprocess.run(
        ["xmake", "project", "-k", "compile_commands", "--outputdir=build"],
        text=True,
        stdout=subprocess.PIPE,
    )


if __name__ == "__main__":
    main()
