#!/usr/bin/env python

from glob import glob
import json

files = glob('log/*/*.json')

for file in files:
    with open(file,'r') as f:
        print(json.load(f)[-1])
