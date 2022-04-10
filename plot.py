import matplotlib.pyplot as plt
import numpy as np

# make data
readData = np.loadtxt(r'output.dat', delimiter='\n', dtype=str)
readX = np.fromstring(readData[0], sep=' ', dtype=float)
readY = np.fromstring(readData[1], sep=' ', dtype=float)
readZ = np.fromstring(readData[2], sep=' ', dtype=float)

X, Y = np.meshgrid(readX, readY)
Z = np.zeros((readX.size, readY.size))

for i in range(0, readX.size-1):
    for j in range(0, readY.size-1):
        Z[i][j] = readZ[i*readY.size + j]

# levels = np.linspace(Z.min(), Z.max(), 20)


# plot
fig, ax = plt.subplots()

co = ax.contourf(X, Y, Z, levels=np.linspace(Z.min(), Z.max(), 100))
fig.colorbar(co, ax=ax)

plt.autoscale()
plt.xlabel("x")
plt.ylabel("y")
plt.title("z(x,y)")

plt.show()