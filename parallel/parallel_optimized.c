// Block partition based coloring
#include <omp.h>
#include "../graph/graph.h"
#include <stdbool.h> 
#include <stdio.h>
#include <time.h>
#include "../timer/cycletimer.h"

#define INIT_SIZE 30

void assign_color(graph_t *g);
int get_min_color(graph_t *g, int vid);
void detect_conflicts(graph_t *g, unsigned char *conflicts);
void mark_conflict(graph_t *g, int vid, unsigned char *conflicts);
void solve_conflicts(graph_t *g, unsigned char *conflicts);

void assign_color(graph_t *g) {
    int i;
    #pragma omp parallel for
    for (i = 0; i < g->nvertex; i++) {
        g->vertex[i].color = get_min_color(g, i);
        g->vertex[i].is_colored = true;
    }
}


int split_color_class(graph_t *g, int *color_class_num, int **color_class_vertices) {
    int color_max = 0, i;
    for (i = 0; i < g->nvertex; i++) {
        int color = g->vertex[i].color;
        if (color > color_max) {
            color_max = color;
        }

        if (color_class_num[color * 2] == 0) {
            color_class_vertices[color] = (int *)malloc(sizeof(int) * INIT_SIZE);
            color_class_num[color * 2] = INIT_SIZE;
        } else if (color_class_num[color * 2] <= color_class_num[color * 2 + 1]) {
            color_class_vertices[color] = (int *)realloc(color_class_vertices[color], sizeof(int) * 2 * color_class_num[color * 2]);
            color_class_num[color * 2] = color_class_num[color * 2] * 2;
        }

        int num = color_class_num[color * 2 + 1];
        color_class_vertices[color][num] = i;
        color_class_num[color * 2 + 1] = num + 1;
    }

    return color_max;
}


void re_assign_color(graph_t *g, int *color_class_num, int **color_class_vertices, int color_max) {
    int color;
    #pragma omp parallel for
    for (color = color_max; color >= 1; color--) {
        int i;
        #pragma omp parallel for
        for (i = 0; i < color_class_num[color*2+1]; i++) {
            int vid = color_class_vertices[color][i];
            g->vertex[vid].color = get_min_color(g, vid);
        }
        //free(color_class_vertices[color]);
    }
    //free(color_class_vertices);
    //free(color_class_num);
}


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


void detect_conflicts(graph_t *g, unsigned char *conflicts) {
    int i;
    #pragma omp parallel for
    for (i = 0; i < g->nvertex; i++) {
        mark_conflict(g, i, conflicts);
    }
}

void mark_conflict(graph_t *g, int vid, unsigned char *conflicts) {
    int i;
    for (i = 0; i < g->vertex[vid].degree; i++) {
        if (g->vertex[vid].is_colored == false) continue;

        int neighbor = g->vertex[vid].neighbor[i];
        if (g->vertex[vid].color == g->vertex[neighbor].color) {
            int min = vid > neighbor ? neighbor : vid;
            g->vertex[min].is_colored = false;
            conflicts[min] = 1;
            if (min == vid) break;
        }
    }
}


void solve_conflicts(graph_t *g, unsigned char *conflicts) {
    int max = 0, i;
    for (i = 0; i < g->nvertex; i++) {
        if (g->vertex[i].color > max)
            max = g->vertex[i].color;
    }

    for (i = 0; i < g->nvertex; i++) {
        if (conflicts[i] == 1) {
            g->vertex[i].color = ++max;
            g->vertex[i].is_colored = true;
        }
    }
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        outmsg("Usage: please input number of threads and graph file!\n");
        exit(0);
    }
    
    double start, stop;

    int thread_count = atoi(argv[1]);
    omp_set_num_threads(thread_count);

    char *fpath = argv[2];

    graph_t *g = read_graph(fpath);
    if (g == NULL) {
        outmsg("Invalid graph!\n");
        exit(0);
    }

    print_graph_info(g);

    int *color_class_num;
    int **color_class_vertices;
    int color_max;

    color_class_num = (int *)calloc(2 * MAX_COLOR, sizeof(int));
    color_class_vertices = (int **)malloc(MAX_COLOR * sizeof(int *));

    double duration;

    start = currentSeconds();
    assign_color(g);
    double assign_color_end = currentSeconds();

    color_max = split_color_class(g, color_class_num, color_class_vertices);
    #pragma omp barrier
    re_assign_color(g, color_class_num, color_class_vertices, color_max);
    double re_assign_color_end = currentSeconds();

    unsigned char conflicts[g->nvertex];
    
    #pragma omp barrier
    detect_conflicts(g, conflicts);
    double detect_conflicts_end = currentSeconds();

    #pragma omp barrier
    solve_conflicts(g, conflicts);
    stop = currentSeconds();

    duration = stop - start;

    printf("=============== Total running time is %f =============\n\n", duration);
    printf("=============== assign_color running time is %f =============\n\n", assign_color_end - start);
    printf("=============== re_assign_color running time is %f =============\n\n", re_assign_color_end - assign_color_end);
    printf("=============== detect_conflicts running time is %f =============\n\n", detect_conflicts_end - re_assign_color_end);
    printf("=============== solve_conflicts running time is %f =============\n\n", stop - detect_conflicts_end);

    check_color(g);

    return EXIT_SUCCESS;
}
