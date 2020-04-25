#include "../graph/graph.h"
#include <stdbool.h> 
#include "../timer/cycletimer.h"

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

int get_min_color(graph_t *g, int vid) {
    //printf("Get min color for %d!\n", vid);
    int count = 0, i;
    vertex_t v = g -> vertex[vid];
    int nb_color_list[v.degree];
    for (i = 0; i < v.degree; i++) {
        int nb_id = v.neighbor[i];
        vertex_t nb_v = g -> vertex[nb_id];
        if (nb_v.is_colored){
        	nb_color_list[count] = nb_v.color;
        	count ++;
        }
    }
    qsort(nb_color_list, count, sizeof(int), &cmpfunc);

    bool color_valid = false;
    int min_color = 1;

    int j = 0;
    // if no neighbor colored 1
    if (nb_color_list[0] != 1){
    	return 1;
    } 
    // chech least available color
    // case 1: available color < max(nb_colors)
    for (i = 1; i < count; i ++){
    	if (nb_color_list[i] - nb_color_list[i - 1] >= 2){
    		return nb_color_list[i-1] + 1;
    	}
    }
    // case 2: availble color = max(nb_colors + 1)
    return nb_color_list[count - 1] + 1; 

}

void assign_color(graph_t *g){
    int i;
	for (i = 0; i < g -> nvertex; i ++){
		g -> vertex[i].color = get_min_color(g, i);
		g -> vertex[i].is_colored = true;
	}
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        outmsg("Usage: please input graph file!\n");
        exit(0);
    }
    
    double start, end;

    char *fpath = argv[1];

    graph_t *g = read_graph(fpath);
    if (g == NULL) {
        outmsg("Invalid graph!\n");
        exit(0);
    }
    print_graph_info(g);
    
    double duration;

    start = currentSeconds();

    assign_color(g);

    end = currentSeconds();

    duration = end - start;

    printf("=============== Total running time is %f =============\n\n", duration);
    

    check_color(g);

    return EXIT_SUCCESS;
}














