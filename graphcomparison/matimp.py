#!/usr/bin/env python3
import re
from decimal import Decimal

row = 0
mat = ""
pat = re.compile(r'(\D*)[\d.]*(\D*)')
f = open('mat.tex', 'r')
f.readline()
for l in f:
    row_str = ""
    col = 0
    for n in l.split(" & "):
        ns = n.strip()
        nss = ns.rstrip("\\")
        try:
            float(nss)
        except:
            res = n
        else:
            d = Decimal(nss)
            rd = round(d, 2)
            if row % 2 == 1:
                if col == (row - 1) / 2:
                    rd = r"\diagval{%s}" % str(rd)
            #print("[%d, %d]: %s" % (row, col, rd))
            m = pat.search(n)
            prefix = m.group(1)
            suffix = m.group(2)
            res = "{0}{1}{2}".format(prefix, rd, suffix)
        if row_str == "":
            row_str = res
        else:
            row_str = row_str + " & " + res
        col = col + 1
    if row % 2 == 0:
        row_str = "\t\lr{P%d'} &\n" % (row / 2)
    mat = mat + row_str
    row = row + 1

print(mat)

