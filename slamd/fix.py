import os, re

for d in ['src', 'include']:
    for root, _, files in os.walk(d):
        for f in files:
            if f.endswith(('.cpp', '.hpp', '.h')):
                p = os.path.join(root, f)
                with open(p, 'r', encoding='utf-8') as file:
                    c = file.read()
                if 'uint' in c or 'ushort' in c:
                    new_c = re.sub(r'\buint\b', 'unsigned int', c)
                    new_c = re.sub(r'\bushort\b', 'unsigned short', new_c)
                    with open(p, 'w', encoding='utf-8') as file:
                        file.write(new_c)
