#!/bin/bash
# Usage
# Instead of the directory of input samples, file lists for samples will replace it.
# Please set output directory of your cms scratch.

# Just added --batch option from original job script.

./skimnanoaod.py --batch -Y 2016 -C ttbb samples/mc/18/TTToSemiLeptonic.txt /cms/scratch/jolim/ttbb_batchtest/18/mc/TTToSemiLeptonic
