import pandas as pd

import deepdish.io as io 

df = pd.read_hdf("array/array_dnn_TTToSemiLeptonic_ttbb.h5")
#df = pd.read_hdf("rdf_array/pt20/forTrain/array_dnn_TTLJ_ttbb_powheg_openloops_CP5.h5")

#idx_only0 = df.groupby(['event'])['signal'].transform(max) == 0
#d0 = df[idx_only0]
#d_remove0 = pd.concat([df,d0]).drop_duplicates(keep=False).reset_index()
#print d_remove0

#io.save("rdf_array/ttbb_if_signal.h5",d_remove0)

idx_only1 = df.groupby(['event'])['signal'].transform(max) == 1
d01 = df[idx_only1]
d01 = d01.sort_values(by=['event']).reset_index()

print d01
io.save("array/ttbb_if_signal.h5",d01)
