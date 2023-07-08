import os
import sys
import getopt
import xml.etree.ElementTree as ET
from typing import List

def get_description(filename: str) -> str:
    tree = ET.parse(filename)
    descs = []
    for elem in tree.iter():
        if (elem.tag == "property") and (elem.attrib["name"] =="description"):
            descs.append(elem.attrib["value"])
    if len(descs) != 1:
        raise RuntimeError("More than 1 description found")
    return descs[0]

def itemize(values: List[str]) -> str:
    block = r"\begin{itemize}"
    block += "\n"
    for item in values:
        block += "\t"
        block += r"\item "
        block += item
        block += "\n"
    block += r"\end{itemize}"
    block += "\n"
    return block

def main(argv):
    input_dir = ""
    output_dir = ""
    opts, args = getopt.getopt(argv,"hi:o:",["idir=","odir="])
    for opt, arg in opts:
        if opt == "-h":
            print ("test.py -i <inputDir> -o <outputDir>")
            sys.exit()
        elif opt in ("-i", "--ifile"):
            input_dir = arg
        elif opt in ("-o", "--ofile"):
            output_dir = arg
    print("Input file is", input_dir)
    print("Output file is", output_dir)

    descs = []
    for filename in os.listdir(input_dir):
        f = os.path.join(input_dir, filename)
        if os.path.isfile(f):
            if os.path.splitext(f)[1] != ".xml":
                continue
            desc = get_description(f)
            desc = desc.replace("_", r"\_")
            descs.append(desc)

    latex = itemize(descs)
    with open(os.path.join(output_dir, "tests.tex"), "w") as f:
        print(latex, file=f)
    
if __name__ == "__main__":
    main(sys.argv[1:])
