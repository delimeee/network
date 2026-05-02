from random import randint, choice

INPUT_DIR = 'input/'

n = int(input())
m = int(input())

a = 1000
b = 1000

nodes = {}
demand = {100, 250, 300}
data = []

sum_demand = 0
for _ in range(n - m):
    x = randint(0, a)
    y = randint(0, b)
    while((x, y) in nodes):
        x = randint(0, a)
        y = randint(0, b)

    d = choice(list(demand))
    sum_demand += d
    data.append((x, y, d, 1))

for _ in range(m):
    x = randint(0, a)
    y = randint(0, b)
    while((x, y) in nodes):
        x = randint(0, a)
        y = randint(0, b)

    cut = randint(0, sum_demand)
    production = cut
    sum_demand -= cut
    data.append((x, y, production, 0))

with open(INPUT_DIR + f'{n}_nodes.txt', 'w') as f:
    f.write(f'{n}\n')
    for node in data:
        x, y, d, t = node
        f.write(f'{x} {y} {d} {t}\n')
    