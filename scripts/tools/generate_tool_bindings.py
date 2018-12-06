import sys
import re
from pathlib import Path
from license_generator import get_license_str


def generate_bindings(tools: list):
    include_dir = Path("include")
    tool_include_file = open(include_dir / "tools" / "tools_pch.h", "w")
    tool_include_file.write("/*\n" + get_license_str() + "\n*/")
    tool_include_file.write("""
#ifndef SFGE_TOOL_PCH_H
#define SFGE_TOOL_PCH_H

#include <utility/python_utility.h>

namespace sfge::tools
{
void ExtendPythonTools(py::module& m);
}

#endif"""
                            )
    tool_include_file.close()

    src_dir = Path("src")
    tool_src_file = open(src_dir / "tools" / "tools_pch.cpp", "w")
    tool_src_file.write("/*\n" + get_license_str() + "\n*/")
    for tool_name in tools:
        tool_name_lowercase = tool_name[0].lower() + re.sub(r"([A-Z])", r"_\1", tool_name[1:]).lower()
        tool_src_file.write("#include <"+tool_name_lowercase+".h>\n")
    tool_src_file.close()

if __name__ == "__main__":
    generate_bindings(sys.argv[1:])
