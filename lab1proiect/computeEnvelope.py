import numpy as np
from scipy.signal import hilbert

data = np.loadtxt("waveData.txt")
raw_points = len(data)

if raw_points > sample_rate * 10:
    data = data[:sample_rate * 6]

# envelope pe data (care e deja tăiată în funcție de regulă)
analytic = hilbert(data)
envelope = np.abs(analytic)

np.savetxt("envelope.txt", envelope)
s

