import os
import glob
from jinja2 import Environment, FileSystemLoader

TEMPLATE_DIR = os.path.join(os.path.dirname(__file__), 'templates')
OUTPUT_DIR = os.path.join(os.path.dirname(__file__), 'build', 'prysma_code_generation')

def generate_tests():
    os.makedirs(OUTPUT_DIR, exist_ok=True)
    env = Environment(loader=FileSystemLoader(TEMPLATE_DIR))
    
    for template_path in glob.glob(os.path.join(TEMPLATE_DIR, "*.jinja")):
        template_name = os.path.basename(template_path)
        base_name = template_name.replace('.p.jinja', '')
        template = env.get_template(template_name)
        
        for t in ["bool", "int32", "int64", "float"]:
            if t == 'bool' and (base_name.startswith('fv_') or base_name in ['mem_negative', 'mem_extreme', 'mem_pass_two_ptr']):
                continue
                
            def cast_val(v):
                if t == "bool": return "true" if float(v) != 0 else "false"
                if t == "float": return str(float(v))
                return str(int(v))
            
            context = {
                "type_name": t,
                "val1": cast_val(1),
                "val2": cast_val(0),
                "val_extreme": "2147483" if t in ["int32", "int64", "bool"] else "0.001"
            }
            
            for v in [0, 1, 2, 3, 4, 5, 10, 11, 15, 20, 25.5, 26, 30, 42, 50, 70, 99, 100, 999]:
                key = f"val_{str(v).replace('.', '_')}" if isinstance(v, float) else f"val_{v}"
                context[key] = cast_val(v)
            
            with open(os.path.join(OUTPUT_DIR, f"{base_name}_{t}.p"), 'w') as f:
                f.write(template.render(**context))

if __name__ == "__main__":
    generate_tests()
