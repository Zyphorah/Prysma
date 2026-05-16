import sys
import os
import subprocess
import shutil
import glob

def find_perf():
    candidates = sorted(glob.glob("/usr/lib/linux-tools/*/perf"), reverse=True)
    for c in candidates:
        if os.access(c, os.X_OK):
            return c
    if shutil.which("perf"):
        return "perf"
    return None

def try_perf(perf_bin, events, exe, cwd):
    ret = subprocess.run([perf_bin, "stat", "-e", events, exe], cwd=cwd)
    if ret.returncode == 0:
        return True
    print("perf without sudo failed, trying with sudo...")
    ret = subprocess.run(["sudo", perf_bin, "stat", "-e", events, exe], cwd=cwd)
    return ret.returncode == 0

def main():
    root_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    perf_dir = os.path.join(root_dir, "tests", "perf_tests")
    build_dir = os.path.join(perf_dir, "build")

    print("[1/4] Building Prysma compiler (python3 build.py -O3)...")
    subprocess.run(["python3", "build.py"], cwd=root_dir, check=True)

    print("[2/4] Configuring perf tests (CMake Release)...")
    subprocess.run([
        "cmake", "-S", ".", "-B", "build",
        "-DCMAKE_BUILD_TYPE=Release",
    ], cwd=perf_dir, check=True)

    print("[3/4] Building perf tests...")
    cores = str(max(1, os.cpu_count() or 1))
    subprocess.run([
        "cmake", "--build", "build", "-j", cores
    ], cwd=perf_dir, check=True)

    exe = os.path.join(build_dir, "PrysmaPerfTests")
    if not os.path.isfile(exe):
        print(f"Error: executable not found at {exe}")
        sys.exit(1)

    print("[4/4] Running benchmark with perf stat (real CPU cycles via PMU)...")
    perf_bin = find_perf()
    perf_events = "cycles,instructions,cache-misses,cache-references,branches,branch-misses"

    if perf_bin:
        print(f"Using perf: {perf_bin}")
        if try_perf(perf_bin, perf_events, exe, root_dir):
            return

    print("(perf unavailable — running Catch2 benchmark only)")
    subprocess.run([exe], cwd=root_dir, check=True)

if __name__ == "__main__":
    main()
