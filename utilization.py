#!/usr/bin/env python

from glob import glob
import json
import matplotlib.pyplot as plt

files=[]
for i in range(1,5):
    files.append(glob(f'log/{2**i}/*.res.json'))

res=[]
for cores in files:
    arr=[]
    for fil in cores:
        with open(fil,'r') as f:
            arr.append(json.load(f)["analysis"])
    res.append(arr)

for r in res:
    x=[]
    y=[]
    for i in r:
        x.append(i["inter_arrival_mean"])
        y.append(i["utilization"])
    plt.plot(x,y,label=f"{r[0]['cores']} cores")

plt.xlabel("inter arrival mean")
plt.ylabel("utilization")
plt.axhspan(.5, .9, facecolor='green', alpha=0.05)
plt.axvspan(.1, .6, facecolor='red', alpha=0.05)
plt.axvspan(.5, 1.2, facecolor='orange', alpha=0.05)
plt.axvspan(1.2, 1.9, facecolor='green', alpha=0.05)
plt.title("cpu utilization vs inter arrival mean of jobs for multicore cpus")
plt.legend()
plt.show()
