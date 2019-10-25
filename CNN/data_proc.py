import h5py as h
import numpy as np


with h.File("tt.h5", 'r') as tt:
    with h.File("wprime.h5", 'r') as w:
        with h.File("new.h5", 'w') as new:
            xt = tt["1"][:]
            xw = w["1"][:]
            xt /= np.max(xt)
            xw /= np.max(xw)
            yt = np.zeros(tt['1'].shape[0])
            yw = np.ones(w['1'].shape[0])
            new.create_dataset("tt/images", data=xt) 
            new.create_dataset("tt/labels", data=yt, dtype='i')
            new.create_dataset("wprime/images", data=xw) 
            new.create_dataset("wprime/labels", data=yw, dtype='i') 
