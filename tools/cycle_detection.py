import matplotlib.pyplot as plt
import numpy as np
import json
import sys


def setup(title, xlabel, ylabel):
    """
    MatPlotLib configuration, such as title, axis labels, grid size, etc
    """
    fig, ax = plt.subplots()

    plt.title(
        title,
        fontsize=10,
        fontweight='ultralight'
    )
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.tick_params(
        axis='both',
        which='major',
        labelsize=8
    )
    plt.grid(color='#efefff')
    ax.set_axisbelow(True)
    # ax.set_yscale('log')
    ax.spines[['left', 'top', 'right', 'bottom']].set_visible(False)

    return fig, ax


def save(ax, path):
    """
    Save plot to a image file
    """

    box = ax.get_position()

    ax.set_position([
        box.x0, box.y0 + box.height * 0.2,
        box.width, box.height * 0.8
    ])

    ax.legend(
        loc='upper center',
        bbox_to_anchor=(0.5, -0.2),
        ncol=4, fontsize=8
    )

    plt.savefig(path)


fig, ax = setup(
    title='Tempo de execução dos algoritmos de detecção de ciclos',
    xlabel='Número de vértices do grafo',
    ylabel='Tempo decorrido (us)'
)

data = {}

with open(sys.argv[1], 'r') as file:
    for line in file:
        length, sample, dfs, djs = map(float, line.split())

        if length not in data:
            data[length] = [[], []]

        data[length][0].append(dfs)
        data[length][1].append(djs)

x = []
y = []
e = []

for length in data:
    x.append(length)
    y.append(np.average(data[length][0]))
    e.append(np.std(data[length][0]))

ax.errorbar(x, y, e, linewidth=1.0, marker='.',
            color='red', label='Depth-first Search')


x = []
y = []
e = []

for length in data:
    x.append(length)
    y.append(np.average(data[length][1]))
    e.append(np.std(data[length][1]))

ax.errorbar(x, y, e, linewidth=1.0,
            marker='.', color='blue', label='Disjoint Sets')

save(ax, 'cycle_detection.png')
