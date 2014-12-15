
def gen_macro(text, prefix):
    template = "#define {0} {1}::{0}"
    for item in text.split():
        print template.format(item, prefix)

def gen_non_terminal_class(text, is_terminal=True):
    def change_name_form(name):
        total = []
        for word in name.split('_'):
            total.append(word.lower().capitalize())
        return ''.join(total)
    template = ("using {0} = Terminal<Token::TypeID::{1}>;" if is_terminal
               else "using {0} = NonTerminal<NonTerminalType::{1}>;")
    for name in text:
        r = template.format(
            change_name_form(name),
            name,
        )
        print r
