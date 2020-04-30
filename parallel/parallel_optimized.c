// Block partition based coloring
#include <omp.h>
#include "../graph/graph.h"
#include <stdbool.h> 
#include <stdio.h>
#include <time.h>
#include "../timer/cycletimer.h"

#define INIT_SIZE 100

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


int get_min_color(graph_t *g, int vid) {
    
    bool color_valid = false;
    int min_color = 0;
    

    while (!color_valid) {
        min_color++;
        color_valid = true;
        int i;
        for (i = 0; i < g->vertex[vid].degree; i++) {
            int neighbor = g->vertex[vid].neighbor[i];

            if (min_color == g->vertex[neighbor].color) {
                color_valid = false;
                break;
            }
        }
    }

    return min_color;
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

    double duration;

    start = currentSeconds();
    assign_color(g);

    int *color_class_num;
    int **color_class_vertices;
    int color_max;

    color_class_num = (int *)calloc(2 * MAX_COLOR, sizeof(int));
    color_class_vertices = (int **)malloc(MAX_COLOR * sizeof(int *));

    color_max = split_color_class(g, color_class_num, color_class_vertices);
    #pragma omp barrier
    re_assign_color(g, color_class_num, color_class_vertices, color_max);
    unsigned char conflicts[g->nvertex];
    
    #pragma omp barrier
    detect_conflicts(g, conflicts);

    #pragma omp barrier
    solve_conflicts(g, conflicts);

    stop = currentSeconds();

    duration = stop - start;

    printf("=============== Total running time is %f =============\n\n", duration);
    
    check_color(g);

    return EXIT_SUCCESS;
}
