# -*- coding: utf-8 -*-

# Imports

import sys
import os
import glob

# create_ui_resource_header

def create_ui_resource_header(directory, file_name):

    offset_table= []
    byte_count=   0

    # Create header file.

    out_file= open(("%s/%s" % (directory, file_name)), "w+")

    base_name=  file_name.split('.')[0]

    out_file.write("/* AUTOMATICALLY GENERATED FILE - DO NOT MODIFY */\n\n")

    # Write include guard.

    out_file.write(("#ifndef %s_h\n"   % (base_name)))
    out_file.write(("#define %s_h\n\n" % (base_name)))

    # Include *.h headers.

    out_file.write("// Includes\n\n")

    os.chdir(directory)
    for h_file in glob.glob("*.h"):
        if h_file != file_name:
            out_file.write("#include \"%s\"\n" % (h_file))

    out_file.write("\n")
    out_file.write("// Resource access macros.\n\n")

    # Write macros.

    out_file.write("#define READ_FONT_RESOURCE(FONT_NAME, SIZE) \\\n")
    out_file.write("    &(font_##FONT_NAME##_##SIZE)\n\n")

    out_file.write("#define READ_BITMAP_RESOURCE(BITMAP) \\\n")
    out_file.write("    &(BITMAP##_bmp)\n\n")

    # Close inclusion guard and the output file.

    out_file.write("#endif\n")
    out_file.close()

    print("UI Resource header created %s.h" % (base_name))


def main():

    # Check arguments.

    if len(sys.argv) < 2:
        print("Usage: <directory> <file name>")
        return

    # Get file path, name and size.

    directory= sys.argv[1]
    filename=  sys.argv[2]

    if not directory:
        print("Error: Invalid directory")
        return

    if not filename:
        print("Error: Invalid file name")
        return

    create_ui_resource_header(directory, filename)

if __name__ == '__main__':
    main()