import networkx as nx

def generate_regular(n,d):
	G = nx.random_regular_graph(d,n)
	# modify the file name
	f = open("./test.txt", "w")
	f.write("{}\n".format(n))
	
	for i in range(n):
		l = list(G.neighbors(i))
		print(l)
		f.write("{},{},".format(i, len(l)))
		s = ",".join(map(str,l))
		f.write("{}\n".format(s))
	
	# f.write("{}\n".format(n))
	# nx.write_multiline_adjlist(G,"test.txt",delimiter=",")




# require d*n even
if __name__ == "__main__":
	# name = "test.txt"
	total_verts = 6
	deg = 3
	generate_regular(total_verts,deg)

