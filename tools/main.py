import matplotlib.pyplot as plt
import numpy as np
import json
import sys


def std_a_over_b(a, b, a_std, b_std):
    c = a / b
    return c * np.sqrt((a_std / a) ** 2 + (b_std / b) ** 2)


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


def load(path, data, multiplier, average):
    """
    Args:
        path (Any): Path to file.
        data (Any): Parsed data collection.
        multiplier (Any): Constant to multiply elapsed values by (to compensate different units of measure).
        average (Any): Whether average (and stdev) should be calculated or is already pre-calculated.
    """
    with open(path, "r") as file:
        graphs = json.load(file)

        for graph in graphs:
            n = graph['n']

            if n not in data:
                data[n] = {}

            for t in {execution['threads'] for execution in graph['executions']}:
                if t not in data[n]:
                    data[n][t] = []

                executions = [execution for execution in graph['executions'] if execution['threads'] == t]

                elapsed = [execution['elapsed'] *
                           multiplier for execution in executions]

                if average:
                    avg = np.average(elapsed)
                    std = np.std(elapsed)
                else:
                    avg = executions[0]['elapsed']
                    std = executions[0]['standard_deviation']

                data[n][t].append({'avg': avg, 'std': std})


def plot(data, ax):
    for n in data:
        x = []
        y = []
        e = []

        for t in data[n]:
            if (t == 1):
                continue

            avg_1 = data[n][1][0]['avg']
            std_1 = data[n][1][0]['std']
            avg_t = data[n][t][0]['avg']
            std_t = data[n][t][0]['std']

            x.append(t)
            y.append(avg_1/avg_t)
            e.append(std_a_over_b(avg_1, avg_t, std_1, std_t))

        ax.errorbar(x, y, linewidth=1.0, marker='.', label=f'N = {n}')


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
    title='Speedup do Algoritmo Paralelizado de Obtenção de Árvores-Geradoras',
    xlabel='Número de threads',
    ylabel=''
)

data = {}

for i in range(1, len(sys.argv) - 1):
    load(sys.argv[i], data, 10 ** -6, True)

print(json.dumps(data))

plot(data, ax)

save(ax, sys.argv[len(sys.argv) - 1])
