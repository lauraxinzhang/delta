#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Jun 28 12:00:01 2020

@author: xinzhang
"""

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches
from matplotlib.collections import PatchCollection

print('hello world')
triang = np.loadtxt('triangles.txt', delimiter=',')
#print(triang)
triang = np.reshape(triang, (-1, 3, 2))
#print(triang)

search = np.loadtxt('search.txt')
# print(search)


fig, ax = plt.subplots()
col = []
for i in range(triang.shape[0]):
    triangle = triang[i, :, :]
    poly = patches.Polygon(triangle)
    col.append(poly)
    

hist = []    
for j in range(search.shape[0]):
    index = np.floor(search[j]/3);
    triag = triang[index, :, :]
    poly = patches.Polygon(triag)
    hist.append(poly)


collection = PatchCollection(col, edgecolor = 'k', facecolor = 'white') 
collection2 = PatchCollection(hist, edgecolor = 'k', facecolor = 'green') 


ax.add_collection(collection)
# ax.add_collection(collection2)
# plt.scatter([0.5], [0])


ax.autoscale_view()
# ax.set_ylim([0, 4]);
# ax.set_xlim([0, 4]);
# plt.savefig('path.pdf')
plt.show()

