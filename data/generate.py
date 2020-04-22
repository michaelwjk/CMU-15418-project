import random

total_vertex = 10000
f = open("./test.txt", "w")
f.write("{}\n".format(total_vertex))

total_map = {}

for i in range(total_vertex):
    total_map[i] = set();

for i in range(total_vertex):
    num = random.randint(1, total_vertex / 10)
    if num <= len(total_map[i]):
        continue
    
    for n in range(num):
        neighbor = random.randint(0, total_vertex-1)
        if neighbor != i:
            total_map[i].add(neighbor)
            total_map[neighbor].add(i)


for i in range(total_vertex):
    f.write("{},{}".format(i, len(total_map[i])))

    for neighbor in total_map[i]:
        f.write(",{}".format(neighbor))
    
    f.write("\n")

f.close()
