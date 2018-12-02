import sys
import re
from license_generator import get_license_str


def generate_bindings(tools : list):
    for tool_name in tools:
        tool_name_lowercase = tool_name[0].lower() + re.sub(r"([A-Z])", r"_\1", tool_name[1:]).lower()


if __name__ == "__main__":
    print("[Python] Generate bindings for tools: "+str(sys.argv))
    generate_bindings(sys.argv[1:])
