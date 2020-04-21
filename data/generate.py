"""
    A naive version, need to be modified
"""

total_vertex = 100
f = open("./test.txt", "w")
f.write("100\n")

for i in range(100):
    line = "{},{},{}\n"
    f.write(line.format(i, 1, (i+1)%100))

f.close()
