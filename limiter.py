#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Jun 28 12:00:01 2020

@author: xinzhang
"""

import numpy as np
import matplotlib.pyplot as plt
# import matplotlib.patches as patches
# from matplotlib.collections import PatchCollection

lm = np.loadtxt('limiter.txt', delimiter=' ')
print(lm.shape)
lm = lm.reshape((-1, 1))
print(lm.shape)

num  = lm.shape[0];
nrz = num/2
rr = lm[:nrz];
zz = lm[nrz:];
print(rr.shape, zz.shape)

# plt.show()

rz = np.hstack((rr, zz))
print(rz.shape)
print(rz.flatten())
np.savetxt('limit_flat', rz.flatten().T, delimiter = ',')

import matplotlib.patches as patches
from matplotlib.collections import PatchCollection

print('hello world')
triang = np.loadtxt('triangles.txt', delimiter=',')
#print(triang)
triang = np.reshape(triang, (-1, 3, 2))
#print(triang)

fig, ax = plt.subplots()
col = []
for i in range(triang.shape[0]):
    triangle = triang[i, :, :]
    poly = patches.Polygon(triangle)
    col.append(poly)


collection = PatchCollection(col, edgecolor = 'k', facecolor = 'white') 


ax.add_collection(collection)
plt.plot(rr, zz)
# ax.add_collection(collection2)
# plt.scatter([0.5], [0])


ax.autoscale_view()
plt.show()