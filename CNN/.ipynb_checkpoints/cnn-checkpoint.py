import h5py as h
import numpy as np

from sklearn.model_selection import train_test_split
from tensorflow.keras.utils import to_categorical

f = h.File("new.h5", 'r')
images = np.vstack([f["tt/images"][:], f["wprime/images"][:]])
labels = np.hstack([f["tt/labels"][:], f["wprime/labels"][:]])
labels = to_categorical(labels)

x_train, x_test, y_train, y_test = train_test_split(images, labels,
                                                    test_size=0.2,
                                                    shuffle=True)


