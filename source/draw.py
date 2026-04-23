import networkx as nx
import matplotlib.pyplot as plt

OUTPUT_DIR = 'output/'

fig, axes = plt.subplots(1, 2, figsize=(12, 6))

g = nx.Graph()
dg = nx.DiGraph()
pos = {}
node_labels = {}
edge_labels_undir = {}
edge_labels_dir = {}
node_colors = []
with open(OUTPUT_DIR + 'output.txt', 'r') as f:
    nodes = []
    str_solid_nodes = f.readline()
    str_nodes = str_solid_nodes.split()
    for i, str_node in enumerate(str_nodes):
        str_x, str_y, str_demand, type = str_node.split(';')
        nodes.append((i, {
            'type': type,
            'demand': float(str_demand),
            }))
        pos[i] = (int(str_x), int(str_y))
        node_labels[i] = f'{str_demand}:{i + 1}'
        if type == 'Station':
            node_colors.append('orange')
        else:
            node_colors.append('blue')

    g.add_nodes_from(nodes)
    dg.add_nodes_from(nodes)

    for i in range(len(nodes)):
        str_connected_nodes = f.readline().split()

        for str_to in str_connected_nodes:
            to, value, flow = str_to.split(';')
            e = (i, int(to), {
                'value': float(value),
                'flow': float(flow),
            })
            g.add_edge(i, int(to),
                       value=float(value),
                       flow=float(flow),
                       name=str_to)

            edge_labels_undir[(i, int(to))] = f'val:{value}'

            # for directed graph
            if float(flow) >= 1.e-8:
                dg.add_edge(i, int(to),
                       value=float(value),
                       flow=float(flow),
                       name=str_to)
                
                edge_labels_dir[(i, int(to))] = f'flw:{flow}'


nx.draw_networkx_edge_labels(
    g,
    pos,
    edge_labels=edge_labels_undir,
    ax=axes[0],
    rotate=False
)
nx.draw_networkx_edge_labels(
    dg, pos,
    edge_labels=edge_labels_dir,
    label_pos=0.6,
    rotate=False,
    font_size=8,
    bbox=dict(facecolor='white', alpha=0.7, pad=0.2),
    ax=axes[1]
)

nx.draw(
    g,
    pos=pos,
    labels=node_labels,
    node_color=node_colors,
    with_labels=True,
    ax=axes[0]
)
nx.draw(dg, pos=pos, labels=node_labels, node_color=node_colors, with_labels=True, ax=axes[1])
plt.savefig(OUTPUT_DIR + 'graph.svg')
plt.show()
