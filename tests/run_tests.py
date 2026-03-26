import sys
import os
import subprocess
from pathlib import Path

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

from generation.generator_ast import GenerateurAST
from generation.generator_interface_visitor import GenerateurInterfaceVisitor
from generation.generator_visitor_base_general import GenerateurVisitorBaseGenerale
from generation.generator_graphe_viz import GenerateurGraphViz

def main():
    root_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

    GenerateurAST(root_dir).generatedr()
    GenerateurInterfaceVisitor(root_dir).generatedr()
    GenerateurVisitorBaseGenerale(root_dir).generatedr()
    GenerateurGraphViz(root_dir).generatedr()

    subprocess.run(["python3", "debug.py"], cwd=root_dir, check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    subprocess.run(["cmake", "-S", ".", "-B", "build"], cwd=root_dir, check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    subprocess.run(["cmake", "--build", "build", "-j", str(max(1, os.cpu_count() or 1))], cwd=root_dir, check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

    test_exe = Path(root_dir) / "tests" / "build" / "PrysmaTests"
    if test_exe.exists():
        subprocess.run([str(test_exe)], cwd=root_dir, check=True)

    subprocess.run(["python3", "tests/generate_tests.py"], cwd=root_dir, check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    
    # We let the orchestrator print its output so we see the failing tests.
    try:
        subprocess.run(["python3", "tests/orchestrator_test.py"], cwd=root_dir, check=True)
    except subprocess.CalledProcessError:
        sys.exit(1)

if __name__ == "__main__":
    main()