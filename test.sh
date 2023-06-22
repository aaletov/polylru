#!/bin/sh
ctest .
python3 reporter.py -i ./reports -o ./latex-report/data
