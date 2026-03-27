import networkx as nx
import matplotlib.pyplot as plt

G = nx.Graph()

G.add_node(1, x=0.0, y=1.0)
G.add_node(2, x=2.0, y=3.0)

# доступ
print(G.nodes[1]['x'], G.nodes[1]['y'])
nx.draw_shell(G, nlist=[range(5, 10), range(5)], with_labels=True, font_weight='bold')
plt.show()