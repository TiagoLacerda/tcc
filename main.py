import matplotlib.pyplot as plt
import numpy as np
import json

with open("results.json", "r") as file:
    data = json.load(file)

# Setup

fig, ax = plt.subplots()

plt.title("Tempo de execução dos algoritmos de geração de árvores-geradoras", fontsize=10, fontweight='ultralight')
plt.xlabel("Número de threads")
plt.ylabel("Tempo decorrido (us)")
plt.tick_params(axis='both', which='major', labelsize=8)
plt.grid(color='#efefff')
ax.set_axisbelow(True)
ax.spines[['left', 'top', 'right', 'bottom']].set_visible(False)

# Plot data

for graph in data:
    foo = dict()
    
    for execution in graph["executions"]:
        threads = execution["threads"]
        elapsed = execution["elapsed"]
        if threads not in foo:
            foo[threads] = []
        foo[threads].append(elapsed)

    x = [k for k in foo]
    y = [np.average(foo[k]) for k in foo]
    e = [np.std(foo[k]) for k in foo]
    ax.errorbar(x, y, linewidth=1.0, label=f"N={graph["n"]}, M={graph["m"]}", yerr=e, linestyle='None', marker='.')

# Shift plot up to make room for legend
box = ax.get_position()
ax.set_position([box.x0, box.y0 + box.height * 0.2,
                 box.width, box.height * 0.8])

# Legend
ax.legend(loc='upper center', bbox_to_anchor=(0.5, -0.2),
           ncol=4, fontsize=8)


plt.savefig('data.png')