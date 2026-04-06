import networkx as nx
import matplotlib.pyplot as mpl


g = nx.Graph()
with open('output.txt', 'r') as f:
    nodes = []
    str_solid_nodes = f.readline()
    str_nodes = str_solid_nodes.split()
    for str_node in str_nodes:
        str_x, str_y, str_demand, type = str_node.split(';')
        nodes.append(tuple(int(str_demand), {
            'type': type,
            'x': int(str_x),
            'y': int(str_y),
            'demand': float(str_demand),
            'name': str_node
            }))
    
    g.add_nodes_from(nodes)
    for i in range(len(nodes)):
        str_connected_nodes = f.readline().split()
        

