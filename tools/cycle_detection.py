import math
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


def std_of_ratio(a, b, a_std, b_std):
    return math.sqrt((a_std / b) ** 2 + (a * b_std / b ** 2) ** 2)


fig, ax = setup(
    title='Relação do tempo de execução dos algoritmos de detecção\nde ciclos (Disjoint Sets / Depth-first Search)',
    xlabel='Número de vértices do grafo',
    ylabel=''
)

data = {}

with open(sys.argv[1], 'r') as file:
    for line in file:
        length, sample, density, dfs, djs = map(float, line.split())

        if density not in data:
            data[density] = {}

        if length not in data[density]:
            data[density][length] = [[], []]

        data[density][length][0].append(dfs)
        data[density][length][1].append(djs)


for density in data:
    x = []
    y = []
    e = []

    for length in data[density]:
        dfs = np.average(data[density][length][0])
        dfs_std = np.std(data[density][length][0])

        djs = np.average(data[density][length][1])
        djs_std = np.std(data[density][length][1])

        std = std_of_ratio(djs, dfs, djs_std, dfs_std)

        x.append(length)
        y.append(djs / dfs)
        e.append(std)

    ax.errorbar(x, y, e, linewidth=1.0, marker='.', label=f'density={int(density)}')


save(ax, 'cycle_detection.png')
