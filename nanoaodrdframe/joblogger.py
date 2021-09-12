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


def merge( dirpath ):
  print dirpath
  for (root,dirs,files) in os.walk( dirpath):
    if files:
      dataset = root.split("/")
      pardir = root[:-len(dataset[-1])] 
      fnames = ' '
      for filename in files:
        fnames = fnames + ' ' + root+"/"+filename
      cmd = 'hadd -f ' + pardir+"/"+dataset[-1] +'.root' + fnames
      #print cmd
      os.system(cmd)

if __name__ == '__main__':

    from optparse import OptionParser

    parser = OptionParser()
    parser.usage = """
    %prog -r <output_dir> <sample_list>
    %prog -s <output_dir> <sample_list>
    %prog -m <output_dir>
    """

    parser.add_option("-r", "--report", dest="report",
                      action = 'store_true',
                      default=False,
                      help='Print number of files')

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

    if options.report:
        filelist = args[1]
        f = open(filelist,'r')
        lines = f.read().splitlines()

        print_numfile( fpath )
        for line in lines :
          print_numfile( line, True  )
    if options.submit:
        filelist = args[1]
        f = open(filelist,'r')
        lines = f.read().splitlines() 

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
          print cmd
          #os.system(cmd)
    if options.merge:
        print ("merge")
        merge( fpath ) 

