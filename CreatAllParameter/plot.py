# -*- coding:utf-8 -*-
#!D:\ProgramData\Anaconda3\Scripts\python3
import pandas as pd
import numpy as np
from pandas import Series, DataFrame
import matplotlib.pyplot as plt
import matplotlib
import os
import time
matplotlib.style.use('ggplot')

def Read_Csv_File(mFilename):
    filedata = pd.read_csv(mFilename).T
    filedata.columns = filedata.ix[0]
    filedata.index.name = 'date'
    filedata.columns.name = None
    filedata = filedata.ix[filedata.index[1:-1]]
    filedata = filedata.drop_duplicates()
    return filedata


FilePath = "D:\\StockFile\\PlotFile"
CsvFile="D:\\StockFile\\PlotFile\\sh1.csv"
indexfile = '1.txt'

def Plot_test():
    files = os.listdir(FilePath)
    if len(files) < 2 :
        return
    if indexfile not in  files:
        return
    time.sleep(0.05)
    for onefile in files:
        if onefile.find('csv') != -1:
            a = Read_Csv_File(FilePath+"\\"+onefile)
            a.plot()
            plt.show()
            os.remove(FilePath+"\\"+onefile)
        elif onefile.find('1.txt') != -1:
            os.remove(FilePath + "\\" + onefile)

if __name__ == "__main__":
    while 1:
        Plot_test()
