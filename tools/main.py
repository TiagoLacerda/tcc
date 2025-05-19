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

        executions = {}

        for graph in graphs:
            n = graph['n']

            if n not in executions:
                executions[n] = {}

            for execution in graph['executions']:
                t = execution['threads']

                if t not in executions[n]:
                    executions[n][t] = []

                executions[n][t].append(execution)

        for n in executions:
            if n not in data:
                data[n] = {}

            for t in executions[n]:
                if t not in data[n]:
                    data[n][t] = []

                elapsed = [execution['elapsed'] *
                           multiplier for execution in executions[n][t]]

                if average:
                    avg = np.average(elapsed)
                    std = np.std(elapsed)
                else:
                    avg = executions[n][t][0]['elapsed']
                    std = executions[n][t][0]['standard_deviation']

                data[n][t].append({'avg': avg, 'std': std})


def plot(data, ax):
    cmap = plt.get_cmap("rainbow")
    cind = np.linspace(0, 1, len({n for n in data}))
    i = 0

    for n in data:
        x = []
        y = []
        e = []

        # if n > 15:
        #     continue

        for t in data[n]:
            # if t > 12:
            #     continue

            if len(sys.argv) < 4:
                # Compare within file (speedup)

                if t == 1:
                    continue

                avg_a = data[n][1][0]['avg']
                std_a = data[n][1][0]['std']
                avg_b = data[n][t][0]['avg']
                std_b = data[n][t][0]['std']

            else:
                # Compare two files

                if len(data[n][t]) < 2:
                    continue

                avg_a = data[n][t][0]['avg']
                std_a = data[n][t][0]['std']
                avg_b = data[n][t][-1]['avg']
                std_b = data[n][t][-1]['std']

            x.append(t)
            y.append(avg_a/avg_b)
            e.append(std_a_over_b(avg_a, avg_b, std_a, std_b))

        if len(x) == 0:
            continue

        ax.errorbar(x, y, e, linewidth=1.0, marker='.', label=f'N = {n}', color=cmap(cind[i]))
        i += 1


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
    path = sys.argv[i]

    if path == "result_summary.json":
        average = False
        multiplier = 1.0
    else:
        average = True
        multiplier = 10 ** -6

    load(sys.argv[i], data, multiplier, average)

print(json.dumps(data))

plot(data, ax)

save(ax, sys.argv[len(sys.argv) - 1])
