import matplotlib.pyplot as plt
import numpy as np

# Data from the results
data = {
    "Vanilla": {
        "TPS": 543556,
        "Net_rate": 608.2,  # in MB/s
        "Ops": 5435914
    },
    "BPF Attached": {
        "TPS": 538265,
        "Net_rate": 602.2,  # in MB/s
        "Ops": 5383365
    }
}


# Function to generate bar graphs
def plot_and_save_graph(metric, filename):
    labels = list(data.keys())
    values = np.array([data[label][metric] for label in labels])

    # Bar positions
    x = np.arange(len(labels))  # the label locations
    width = 0.35  # the width of the bars

    fig, ax = plt.subplots()

    # Create bars for the metric
    bars = ax.bar(x, values, width, label=metric, color='blue')

    # Add some text for labels, title and custom x-axis tick labels, etc.
    ax.set_ylabel(metric)
    ax.set_title(f'Memcached {metric} Comparison')
    ax.set_xticks(x)
    ax.set_xticklabels(labels)
    ax.legend()

    # Add a grid for better readability
    ax.yaxis.grid(True)

    # Save the figure
    plt.tight_layout()
    plt.savefig(filename)
    plt.close()


# Generate graphs for TPS and Net_rate
plot_and_save_graph('TPS', 'tps.png')
plot_and_save_graph('Ops', 'ops.png')
plot_and_save_graph('Net_rate', 'net_rate.png')
