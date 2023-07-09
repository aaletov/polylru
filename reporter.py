import os
import sys
import getopt
import xml.etree.ElementTree as ET
import datetime as dt
from typing import List, Tuple

def parse_xml(filename: str) -> Tuple[dt.datetime, str]:
    tree = ET.parse(filename)
    descs = []
    tss = []
    for elem in tree.iter():
        if (elem.tag == "property") and (elem.attrib["name"] =="description"):
            descs.append(elem.attrib["value"])
        if (elem.tag == "testcase"):
            sts = elem.attrib["timestamp"]
            tts = dt.datetime.fromisoformat(sts)
            tss.append(tts)
    if not((len(descs) == 1) and (len(tss) == 1)):
        raise RuntimeError("More than 1 description and tss found")
    return (tss[0], descs[0])

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

    items = []
    for filename in os.listdir(input_dir):
        f = os.path.join(input_dir, filename)
        if os.path.isfile(f):
            if os.path.splitext(f)[1] != ".xml":
                continue
            ts, desc = parse_xml(f)
            desc = desc.replace("_", r"\_")
            items.append((ts, desc))

    items.sort(key=lambda item: item[0])
    descs = list([item[1] for item in items])
    latex = itemize(descs)
    with open(os.path.join(output_dir, "tests.tex"), "w") as f:
        print(latex, file=f)
    
if __name__ == "__main__":
    main(sys.argv[1:])
