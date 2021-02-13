#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Nov 30 18:17:42 2020

@author: willpope
"""

# INIT --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

import time
from datetime import datetime
import csv 
import os


# LIST FLATTENING TOOL
from collections.abc import Iterable                            
def flatFuncAux(items):
    for x in items:
        if isinstance(x, Iterable) and not isinstance(x, (str, bytes)):
            yield from flatFuncAux(x)
        else:
            yield x

def flatList(inpArr1):
    return list(flatFuncAux(inpArr1))


# CSV INITIALIZATION - creates .csv file, saves
def csvStart(subfolder, file_prefix, header):
    time_start = datetime.now()

    file = file_prefix + '_' + time_start.strftime('d%Y%m%dt%H%M%S')
    rel_path = subfolder + "/" + file + '.csv'
    script_directory = os.path.dirname(__file__)
    
    abs_path = os.path.join(script_directory, rel_path)

    file_create = open(abs_path, 'w', newline ='')         # open(..'w'..) creates new CSV file
    with file_create:   
        write = csv.writer(file_create) 
        write.writerow(header) 
        
    return time_start, abs_path


# CSV FUNCTION - opens .csv file, writes row, saves
def csvAdd(output_array, time_start, abs_path):
    global entry

    entry = entry + 1
    timeGMT = time.strftime("%H:%M:%S %Z", time.gmtime())
    timeELA = time.time() - time_start
    timeELA = round(timeELA, 3)

    row_entry = flatList([entry, timeGMT, timeELA, output_array, time_start.strftime("%Y%m%d%H%M%S")])  # output_array is all your data entries   

    file_append = open(abs_path, 'a', newline ='')      # open(..'a'..) appends existing CSV file
    with file_append:   
        write = csv.writer(file_append) 
        write.writerow(row_entry)  