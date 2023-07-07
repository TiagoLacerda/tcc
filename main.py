import matplotlib.pyplot as plt
import numpy as np
import random

# Load data from file

file = open("data/data.txt", "r")
data = [list(map(int, line.split(" "))) for line in file.read().splitlines()]

n = [item[0] for item in data]
dfs = [item[1] for item in data]
djs = [item[2] for item in data]

# Setup matplotlib

fig, ax = plt.subplots()

plt.title("Tempo de execução dos algoritmos de decisão de conexidade", fontsize=10, fontweight='ultralight')
plt.xlabel("Número de vértices do grafo")
# plt.xlabel("Número de arestas que induzem ciclo no grafo")
plt.ylabel("Tempo decorrido (us)")
plt.tick_params(axis='both', which='major', labelsize=8)
plt.grid(color='#efefff')
# plt.rc('axes', axisbelow=True)
ax.set_axisbelow(True)

ax.spines[['left', 'top', 'right', 'bottom']].set_visible(False)

# Plot data

ax.scatter(n, dfs, color='red', linewidth=1.0, label='Depth-First Search')
ax.scatter(n, djs, color='blue', linewidth=1.0, label='Disjoint Sets')
ax.legend()

# plt.show()
plt.savefig('data/data.png')