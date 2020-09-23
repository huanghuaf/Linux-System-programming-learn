#!/usr/bin/env python
 
#
# This is a simple script to plot memory usage/allocation trends based on the periodical snapshots of
# /proc/meminfo and /proc/slabinfo.
#
# Usage:
#   memleak_plot.py -f <memory_monitor_output_file>
#
 
import os
import sys
import os.path
import errno
import re
import glob
import time
import argparse
import subprocess
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.legend_handler import HandlerLine2D
 
 
def run_system(cmd):
    """ Run bash commands and return the output """
    print(cmd)
    p = subprocess.Popen(cmd,shell=True,stdout=subprocess.PIPE,stderr=subprocess.PIPE,stdin=subprocess.PIPE)
    out,err = p.communicate()
    if len(out) > 0:
        return out.splitlines()
    else:
        return None
 
def draw_unreclaim(f):
    print(f)
    
    cmd = r"grep MemFree {} | awk '{{print $2}}'".format(f)
    MemFree = run_system(cmd)
    MemFree = np.asarray(MemFree, dtype=int) / 1024 # MB
 
 
    cmd = "grep SUnreclaim {} | awk '{{print $2}}'".format(f)
    SUnreclaim = run_system(cmd)
    SUnreclaim = np.asarray(SUnreclaim, dtype=int) / 1024 # MB
    x = np.arange(1, 2*len(SUnreclaim)+1, 2)
 
    # creat figure and ax
    fig, ax1 = plt.subplots()
 
    # draw ax1
    ax1.plot(x, SUnreclaim, 'r', label='SUnreclaim')
    ax1.set_xlabel('uptime(hours)')
    # Make the y-axis label and tick labels match the line color.
    ax1.set_ylabel('SUnreclaim(MB)', color='r')
    for tl in ax1.get_yticklabels():
        tl.set_color('r')
    plt.legend(loc='upper left')
    ax1.grid(True)
 
    # draw ax2
    # draw two scales in one plot
    ax2 = ax1.twinx()
    ax2.plot(x, MemFree, 'b', label='MemFree')
    ax2.set_ylabel('MemFree(MB)', color='b')
    for tl in ax2.get_yticklabels():
        tl.set_color('b')
    plt.legend(loc='upper right')
    #plt.show()
 
def draw_kmalloc(f):
    print(f)
 
    kmalloc_4k = run_system("grep kmalloc-4k {} | grep -v dma | awk '{{print $2}}'".format(f))
    kmalloc_4k = np.asarray(kmalloc_4k, dtype=int)
    
    kmalloc_2k = run_system("grep kmalloc-2k {} | grep -v dma | awk '{{print $2}}'".format(f))
    kmalloc_2k = np.asarray(kmalloc_2k, dtype=int)
    
    kmalloc_1k = run_system("grep kmalloc-1k {} | grep -v dma | awk '{{print $2}}'".format(f))
    kmalloc_1k = np.asarray(kmalloc_1k, dtype=int)
    
    kmalloc_512 = run_system("grep kmalloc-512 {} | grep -v dma | awk '{{print $2}}'".format(f))
    kmalloc_512 = np.asarray(kmalloc_512, dtype=int)
    
    kmalloc_256 = run_system("grep kmalloc-256 {} | grep -v dma | awk '{{print $2}}'".format(f))
    kmalloc_256 = np.asarray(kmalloc_256, dtype=int)
    
    kmalloc_192 = run_system("grep kmalloc-192 {} | grep -v dma | awk '{{print $2}}'".format(f))
    kmalloc_192 = np.asarray(kmalloc_192, dtype=int)
    
    kmalloc_128 = run_system("grep kmalloc-128 {} | grep -v dma | awk '{{print $2}}'".format(f))
    kmalloc_128 = np.asarray(kmalloc_128, dtype=int)
   
    kmalloc_64 = run_system("grep kmalloc-64 {} | grep -v dma | awk '{{print $2}}'".format(f))
    kmalloc_64 = np.asarray(kmalloc_64, dtype=int)
    
    x = np.arange(1, 2 * len(kmalloc_4k) + 1, 2)
 
    fig, ax1 = plt.subplots()
    ax1.plot(x, kmalloc_4k, label="kmalloc-4k")
    ax1.plot(x, kmalloc_256, label="kmalloc-256")
    ax1.plot(x, kmalloc_128, label="kmalloc-128")
    ax1.plot(x, kmalloc_64, label="kmalloc-64")
    
    ax1.set_xlabel('uptime')
    ax1.grid(True)
    plt.legend(loc='upper left')
 
    #plt.show()
 
def main():
    parser = argparse.ArgumentParser(description="This is a memleak analyse and graphic program")
    parser.add_argument('-f', '--file', action='store', required=True, help='the memory information file')
 
    args = parser.parse_args()
    mon_file = args.file
 
    draw_unreclaim(mon_file)
    draw_kmalloc(mon_file)
 
    plt.show()
 
if __name__ == '__main__':
    main()