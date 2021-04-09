from sklearn.preprocessing import StandardScaler
import pandas as pd
import DataCollection as dc
import numpy as np

X = dc.X

X_norm = StandardScaler().fit_transform(X)

