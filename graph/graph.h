#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAXLINE 200000
#define MAX_COLOR 2000

typedef struct {
    int id;
    int *neighbor; //Id of its neighbors
    bool is_colored;
    int color;
    int degree;  
} vertex_t;


typedef struct {
    int nvertex;
    vertex_t *vertex;
} graph_t;

graph_t *read_graph(char *fpath);
void outmsg(char *fmt, ...);
void print_graph_info(graph_t *g);
bool check_color(graph_t *g);

