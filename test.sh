#!/bin/sh
cd cmake-build-debug
ctest .
cd ..
python3 reporter.py -i ./reports -o ./latex-report/data
