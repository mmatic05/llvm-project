#!/usr/bin/env python

import argparse
import os
import sys
import platform
import array as arr
from json import loads
from subprocess import Popen, PIPE
from matplotlib.cm import get_cmap

# Holds a file statistics.
class OneFileStats:
  def __init__(self, Name, BlockRThroughput, DispatchWidth,
    IPC, Instructions, Iterations, TotalCycles, TotaluOps, uOpsPerCycle):
    self.Name = Name
    self.BlockRThroughput = BlockRThroughput
    self.DispatchWidth = DispatchWidth
    self.IPC = IPC
    self.Instructions = Instructions
    self.Iterations = Iterations
    self.TotalCycles = TotalCycles
    self.TotaluOps = TotaluOps
    self.uOpsPerCycle = uOpsPerCycle

# Parse the program arguments.
def parse_program_args(parser):
  parser.add_argument('file_names', nargs = '+', type = str, help = 'Names of files which tool process.')
  parser.add_argument('-mtriple', nargs = 1, type = str, action = 'store', default = ['x86_64-unknown-linux-gnu'], help = 'Target triple.')
  parser.add_argument('-mcpu', nargs = 1, type = str, action = 'store', default = ['skylake'], help = 'Target a specific cpu type .')
  parser.add_argument('-diff', action = 'store_true', default = False, help = 'Prints the difference of two input assembler files.')
  parser.add_argument('-iterations', type = int, nargs = 1, action = 'store', default = [100], help = 'Number of iterations to run.')
  return parser.parse_args()

# Verify that the program inputs meet the requirements.
def verify_program_inputs(opts):
  if len(sys.argv) < 2 :
    print ('error: Wrong number of arguments.')
    return False
  if opts.diff and len(opts.file_names) < 2:
    print ('error: Wrong number of input files.')
    return False
  if not opts.diff and len(opts.file_names) > 1:
    print ('error: Wrong number of input files.')
    return False
  return True

# Returns the name of the file to be analyzed from the path it is on.
def getFilenameFromPath (path):
  indexOfSlash = path.rfind("/")
  return path[(indexOfSlash + 1) : len(path)]

# Returns the results of the running llvm-mca tool for the input file.
def llvmMcaStats (opts):

  # Get the directory of the LLVM tools.
  llvm_mca_cmd = os.path.join(os.path.dirname(__file__), \
                                    "llvm-mca")
  
  # The statistics llvm-mca options.
  llvm_mca_stats_opt1 = "-mtriple=" + opts.mtriple[0]
  llvm_mca_stats_opt2 = "-mcpu=" + opts.mcpu[0]
  llvm_mca_stats_opt3 = "-json"
  llvm_mca_stats_opt4 = "-iterations=" + str(opts.iterations[0])

  # Generate the stats with the llvm-mca.
  subproc = Popen([llvm_mca_cmd, llvm_mca_stats_opt1, llvm_mca_stats_opt2, \
                  llvm_mca_stats_opt3, llvm_mca_stats_opt4, opts.file_names[0]], \
                  stdin = PIPE, stdout = PIPE, stderr = PIPE, \
                  universal_newlines = True)
  cmd_stdout, cmd_stderr = subproc.communicate()

  try:
    json_parsed = loads(cmd_stdout)
  except:
    print ('error: No valid llvm-mca statistics found.') 
    print(cmd_stderr)
    sys.exit(1)
  
  FileStats = OneFileStats(opts.file_names[0], \
                           json_parsed["CodeRegions"][0]["SummaryView"]["BlockRThroughput"], \
                           json_parsed["CodeRegions"][0]["SummaryView"]["DispatchWidth"], \
                           json_parsed["CodeRegions"][0]["SummaryView"]["IPC"], \
                           json_parsed["CodeRegions"][0]["SummaryView"]["Instructions"], \
                           json_parsed["CodeRegions"][0]["SummaryView"]["Iterations"], \
                           json_parsed["CodeRegions"][0]["SummaryView"]["TotalCycles"], \
                           json_parsed["CodeRegions"][0]["SummaryView"]["TotaluOps"], \
                           json_parsed["CodeRegions"][0]["SummaryView"]["uOpsPerCycle"])

  return FileStats

# Based on the obtained results llvm-mca tool draws graphs for the input file.
def DrawFileStats (FileStatsResult, opts):
  import matplotlib.pyplot as plt

  names = ['Block RThroughput', \
           'Dispatch Width', \
           'IPC', \
           'uOps Per Cycle', \
           'Instructions', \
           'Iterations', \
           'Total Cycles', \
           'Total uOps']

  values = [FileStatsResult.BlockRThroughput, \
            FileStatsResult.DispatchWidth, \
            FileStatsResult.IPC, \
            FileStatsResult.uOpsPerCycle, \
            FileStatsResult.Instructions, \
            FileStatsResult.Iterations, \
            FileStatsResult.TotalCycles, \
            FileStatsResult.TotaluOps]
  
  fig, axs = plt.subplots(nrows = 4, ncols = 2)
  fig.suptitle('Machine code statistics', fontsize = 20, fontweight = 'bold', color = 'black')  
  i = 0

  for x in range(4):
   for y in range(2):
    axs[x-1][y-1].grid(True, color = 'grey', linestyle = '--')
    if i == 0 :
       axs[x-1][y-1].bar( ' ', values[i], width = 0.05, color = 'red', label = getFilenameFromPath(FileStatsResult.Name))
    else:
       axs[x-1][y-1].bar( ' ', values[i], width = 0.05, color = 'red')
    axs[x-1][y-1].set_axisbelow(True)
    axs[x-1][y-1].set_xlim([-0.5, 0.5])
    axs[x-1][y-1].set_ylim([0, values[i] + (values[i]/2) ])
    axs[x-1][y-1].text(0, values[i] + (values[i]/40), s = float("{0:.2f}".format(values[i])), color = 'black', fontweight = 'bold', fontsize = 10)
    axs[x-1][y-1].set_title(names[i], fontsize = 15, fontweight = 'bold' )
    i = i + 1

  fig.legend(prop = {'size': 15})
  figg = plt.gcf()
  figg.set_size_inches((15, 11), forward = False)
  plt.savefig('llvm-mca.png', dpi = 500)
  print('The plot was saved within "llvm-mca.png".')

# Returns the results of the running llvm-mca tool and diff options for input files.
def llvmMcaDiffStats (opts):

  # Get the directory of the LLVM tools.
  llvm_mca_cmd = os.path.join(os.path.dirname(__file__), \
                                    "llvm-mca")
  
  # The statistics llvm-mca options.
  llvm_mca_stats_opt1 = "-mtriple=" + opts.mtriple[0]
  llvm_mca_stats_opt2 = "-mcpu=" + opts.mcpu[0]
  llvm_mca_stats_opt3 = "-json"
  llvm_mca_stats_opt4 = "-diff"
  llvm_mca_stats_opt5 = "-iterations=" + str(opts.iterations[0])
  llvm_mca_stats_inputFiles = ""

  for i in range(len(opts.file_names)):
    llvm_mca_stats_inputFiles += " " + opts.file_names[i]

  CMD = llvm_mca_cmd + " " + llvm_mca_stats_opt1 + " " + \
        llvm_mca_stats_opt2 + " " + llvm_mca_stats_opt3 + \
        " " + llvm_mca_stats_opt4 + " " + llvm_mca_stats_opt5 + \
        llvm_mca_stats_inputFiles

  # Generate the stats with the llvm-mca.
  subproc = Popen(CMD.split(' '), stdin = PIPE, stdout = PIPE, \
                  stderr = PIPE, universal_newlines = True)
  cmd_stdout, cmd_stderr = subproc.communicate()

  try:
    json_parsed = loads(cmd_stdout)
  except:
    print ('error: No valid llvm-mca statistics found.')
    print(cmd_stderr)
    sys.exit(1)

  arrayOneFileStats = [None] * len(opts.file_names)

  for i in range(len(opts.file_names)):
    ordinalNumberOfFile = "File: " + str(i+1)
    arrayOneFileStats[i] = OneFileStats(opts.file_names[i], \
                               json_parsed[ordinalNumberOfFile]["Block RThroughput"], \
                               json_parsed[ordinalNumberOfFile]["Dispatch Width"],  \
                               json_parsed[ordinalNumberOfFile]["IPC"], \
                               json_parsed[ordinalNumberOfFile]["Instructions"], \
                               json_parsed[ordinalNumberOfFile]["Iterations"], \
                               json_parsed[ordinalNumberOfFile]["Total Cycles"], \
                               json_parsed[ordinalNumberOfFile]["Total uOps"], \
                               json_parsed[ordinalNumberOfFile]["uOps Per Cycle"])

  return arrayOneFileStats

# Based on the obtained results llvm-mca tool and diff options draws graphs for the input files.
def DrawFilesStats(arrayOneFileStats, opts):
  import matplotlib.pyplot as plt
  import numpy as np
  

  names = ['Block RThroughput', \
           'Dispatch Width', \
           'IPC', \
           'uOps Per Cycle', \
           'Instructions', \
           'Total Cycles', \
           'Total uOps']

  rows, cols = (len(opts.file_names), 7)

  values = [[0 for x in range(cols)] for y in range(rows)] 

  for i in range(len(opts.file_names)):
    values[i][0] = arrayOneFileStats[i].BlockRThroughput
    values[i][1] = arrayOneFileStats[i].DispatchWidth
    values[i][2] = arrayOneFileStats[i].IPC
    values[i][3] = arrayOneFileStats[i].uOpsPerCycle
    values[i][4] = arrayOneFileStats[i].Instructions
    values[i][5] = arrayOneFileStats[i].TotalCycles
    values[i][6] = arrayOneFileStats[i].TotaluOps

  fig, axs = plt.subplots(4, 2)
  fig.suptitle('Machine code statistics', fontsize = 20, fontweight = 'bold', color = 'black')
  i = 0

  for x in range(4):
    for y in range(2):
      cmap = get_cmap("tab20")  
      colors = cmap.colors  
      if not (x == 0 and y == 1) and i < 7:
          axs[x][y].grid(True, color = 'grey', linestyle = '--')
          maxValue = 0
          if i == 0:
              for j in range (len(opts.file_names)):
                if maxValue < values[j][i]:
                  maxValue = values[j][i] 
                axs[x][y].bar(0.3 * j, values[j][i], width = 0.1, color = colors[j], label = getFilenameFromPath(opts.file_names[j]))
          else:
              for j in range (len(opts.file_names)):
                if maxValue < values[j][i]:
                  maxValue = values[j][i] 
                axs[x][y].bar(0.3 * j, values[j][i], width = 0.1, color = colors[j])
          axs[x][y].set_axisbelow(True)
          axs[x][y].set_xlim([-0.5, len(opts.file_names)/2 ])
          axs[x][y].set_ylim([0, maxValue + (maxValue/2) ])
          axs[x][y].set_title(names[i], fontsize = 15, fontweight='bold' )
          axs[x][y].axes.xaxis.set_visible(False)
          for j in range (len(opts.file_names)):
            axs[x][y].text(0.3 * j, values[j][i] + (maxValue/40), s = float("{0:.2f}".format(values[j][i])), color = 'black', fontweight = 'bold', fontsize = 7)
          i = i + 1
   
  axs[0][1].set_visible(False)
  fig.legend(prop = {'size': 15})
  figg = plt.gcf()
  figg.set_size_inches((15, 11), forward = False)
  plt.savefig('llvm-mca-diff.png', dpi = 500)
  print('The plot was saved within "llvm-mca-diff.png".')

def Main():
  parser = argparse.ArgumentParser()
  opts = parse_program_args(parser)

  if not verify_program_inputs(opts):
    parser.print_help()
    sys.exit(1)

  if opts.diff:
    arrayOneFileStats = llvmMcaDiffStats(opts)
    DrawFilesStats(arrayOneFileStats, opts)
  else:
    FileStatsResult = llvmMcaStats(opts)
    DrawFileStats(FileStatsResult, opts)
 
if __name__ == '__main__':
  Main()
  sys.exit(0)
