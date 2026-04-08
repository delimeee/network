import networkx as nx
import matplotlib.pyplot as plt

OUTPUT_DIR = 'output/'

g = nx.Graph()
pos = {}
node_labels = {}
edge_labels = {}
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
        node_labels[i] = f'{type}:{str_demand}'

    g.add_nodes_from(nodes)
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

            edge_labels[(i, int(to))] = f'val:{value}\nflw:{flow}'


nx.draw_networkx_edge_labels(g, pos, edge_labels=edge_labels)
nx.draw(g, pos=pos, labels=node_labels, with_labels=True)
plt.savefig(OUTPUT_DIR + 'graph.svg')
plt.show()
