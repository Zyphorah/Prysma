from encodings import utf_8
import os
from pathlib import Path
from jinja2 import Environment, FileSystemLoader

template_path = Path(__file__).parent 
template_name = "license.txt.jinja"
base_path = Path(__file__).parents[2]
extensions = {'.h','.cpp'}


def check_license_file(path: Path):

    lignes = path.read_text(encoding="utf-8").splitlines()
    if not lignes:
        return False
    return " - Brief description of the file -------*- C++ -*-===//" in lignes[0]


def update_license(path : Path, lignes):

    env = Environment(loader=FileSystemLoader(str(template_path)))
    template = env.get_template(template_name)

    context = {
        "path_file": path.name
    }

    if check_license_file(path) is not True:      
        with open(path, 'w') as f:
            header = template.render(**context)
            f.write(header + lignes)
    
def main():

    for path in base_path.rglob('*'):
        if path.is_file() and path.suffix in extensions:
            lignes = path.read_text("utf-8")
            update_license(path, lignes)
          
if __name__ == "__main__":
    main()