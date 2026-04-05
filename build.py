import os
import shutil
import subprocess
from generation.generator_ast import GeneratorAST
from generation.generator_interface_visitor import GeneratorInterfaceVisitor
from generation.generator_visitor_base_general import GeneratorVisitorBaseGeneral
from generation.generator_graphe_viz import GeneratorGraphViz
from generation.generator_expression import GeneratorExpression
from generation.generator_parser import GeneratorParser

def main():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    os.chdir(script_dir)

    GeneratorAST(script_dir).generate()
    GeneratorInterfaceVisitor(script_dir).generate()
    GeneratorVisitorBaseGeneral(script_dir).generate()
    GeneratorGraphViz(script_dir).generate()
    GeneratorExpression(script_dir).generate()
    GeneratorParser(script_dir).generate()

    cxxflags_list = [
        "-O3",                  # Maximum optimization for speed
        "-march=native",        # Fully utilize your CPU's instruction set
        "-fno-rtti",            # Disable runtime type information (RTTI)
        "-fomit-frame-pointer", # Free up a CPU register
        "-flto",                # Link Time Optimization (LTO)
        "-DNDEBUG",             # Completely disable assertions
        "-std=c++26",           # C++26 standard
    ]
    
    ldflags_list = [
        "-flto",
        "-Wl,--gc-sections",
        "-Wl,-s"
    ]

    cxxflags = " ".join(cxxflags_list)
    ldflags = " ".join(ldflags_list)

    subprocess.run([
        "cmake", "-S", ".", "-B", "build", 
        "-DCMAKE_BUILD_TYPE=Release",
        f"-DCMAKE_CXX_FLAGS={cxxflags}",
        f"-DCMAKE_EXE_LINKER_FLAGS={ldflags}", 
        f"-DCMAKE_MODULE_LINKER_FLAGS={ldflags}",
        f"-DCMAKE_SHARED_LINKER_FLAGS={ldflags}",
        "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON",
        "-DZLIB_LIBRARY=/usr/lib/x86_64-linux-gnu/libz.so",
        "-DZLIB_INCLUDE_DIR=/usr/include",
    ], check=True)

    os.makedirs("build", exist_ok=True)
    shutil.rmtree(os.path.join("build", "obj"), ignore_errors=True)

    num_cores = str(os.cpu_count() or 1)

    subprocess.run([
        "cmake", "--build", "build", "--parallel", num_cores
    ], check=True)
    
if __name__ == "__main__":
    main()