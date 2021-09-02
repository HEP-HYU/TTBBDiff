#!/usr/bin/env python
import os
import re
import sys
import time
import commands


def print_numfile( dirpath, dosum=False):

  pre = " "
  sum = 0
  dataset = " "
  for (root,dirs,files) in os.walk( dirpath):
    if files:
      dataset = root.split("/")
      if not dosum:
        print (root + " --> " + str(len(files)) + " files")
      sum += len(files)
  
  if dosum:
    print ( dataset[7] + "/" + dataset[6] + " --> " + str(sum) + " files")

if __name__ == '__main__':

    from optparse import OptionParser

    parser = OptionParser()
    parser.usage = """
    %prog [options] <sample_dir>
    
    Print the numnber of processed files.
    """

    parser.add_option("-r", "--report", dest="report",
                      action = 'store_true',
                      default=False,
                      help='Print report for bad jobs.')

    parser.add_option("-s", "--submit", dest="submit",
                      action = 'store_true',
                      default=False,
                      help='Submission command')

    parser.add_option("-m", "--merge", dest="merge",
                      action = 'store_true',
                      default=False,
                      help='hadd -f merged.root *.root')


    (options,args) = parser.parse_args()

    fpath = args[0]
    filelist = args[1]
    f = open(filelist,'r')
    lines = f.read().splitlines()

    if options.report:
        print_numfile( fpath )
        for line in lines :
          print_numfile( line, True  )
    if options.submit:
        for line in lines :
          dirnames = line.split("/")
          yearname = dirnames[6]
          year = ''
          if '16' in yearname: 
            year = "2016"
          if '17' in yearname:
            year = "2017"
          if '18' in yearname:
            year = "2018"    

          logfilename = dirnames[7] + '_' + dirnames[6] + '.txt'
          cmd = './skimnanoaod.py -F --split 25 -Y ' + year + ' ' + line + ' ' + fpath + '/' + dirnames[7] +  '/' + dirnames[6] + ' > ' + logfilename 
          #print cmd
          os.system(cmd)
    if options.merge:
        print ("merge")

