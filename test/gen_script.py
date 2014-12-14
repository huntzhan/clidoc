
def gen_macro(text, prefix):
    template = "#define {0} {1}::{0}"
    for item in text.split():
        print template.format(item, prefix)
