from pathlib import Path
from jinja2 import Environment, FileSystemLoader

template_path = Path(__file__).parent 
template_name = "license.txt.jinja"
base_path = Path(__file__).parents[2]
extensions = {'.h','.cpp', '.jinja'}


def check_license_file(path: Path):

    lignes = path.read_text(encoding="utf-8").splitlines()
    if not lignes:
        return False
    return " - Brief description of the file -------*- C++ -*-===//" in lignes[0]


def update_license(path : Path, current_content):

    env = Environment(loader=FileSystemLoader(str(template_path)))
    template = env.get_template(template_name)
    file_lines = current_content.splitlines()

    context = {
        "path_file": path.name
    }
    header = template.render(**context)
    new_header_lines = header.splitlines()

    if check_license_file(path) is not True:   
        if path.resolve() != (template_path / template_name).resolve():
            with open(path, 'w') as f:
                f.write(header + "\n\n" + current_content)
    else:
        if path.resolve() != (template_path / template_name).resolve():
            nb_lines_to_replace = len(new_header_lines)
            file_lines[:nb_lines_to_replace] = new_header_lines
            path.write_text("\n".join(file_lines) + "\n", encoding="utf-8")
            print(f"[UPDATED] {path.name}")

def main():

    for path in base_path.rglob('*'):
        if path.is_file() and path.suffix in extensions:
            current_content = path.read_text("utf-8")
            update_license(path, current_content)
          
if __name__ == "__main__":
    main()