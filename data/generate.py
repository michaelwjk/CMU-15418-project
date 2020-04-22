import random


total_vertex = 20000
f = open("./test.txt", "w")
f.write("{}\n".format(total_vertex))

total_map = {}

def save_vertex(total_map, vid):
    f.write("{},{}".format(vid, len(total_map[vid])))

    for neighbor in total_map[vid]:
        f.write(",{}".format(neighbor))
    
    f.write("\n")

    del total_map[vid]


for i in range(total_vertex):
    total_map[i] = set()

for i in range(total_vertex):
    if i == (int)(total_vertex * 0.25):
        print("25% finished!")
    elif i == (int)(total_vertex * 0.5):
        print("50% finished!")
    elif i == (int)(total_vertex * 0.75):
        print("75% finished!")
    
    """num = random.randint(1, total_vertex / 2) - len(total_map[i])"""
    num = int(total_vertex / 2) - len(total_map[i])
    if num <= 0:
        save_vertex(total_map, i)
        continue
    
    num = min(num, total_vertex-i-1)

    while num > 0:
        neighbor = random.randint(i+1, total_vertex-1)
        if neighbor != i:
            total_map[i].add(neighbor)
            total_map[neighbor].add(i)

            num -= 1

    save_vertex(total_map, i)

f.close()
