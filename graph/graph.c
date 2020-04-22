#include "graph.h"

graph_t *read_graph(char *fpath) {
    FILE *gfile = NULL;
    graph_t *g = NULL;

    char linebuf[MAXLINE];
    int nvertex, i = 0;
    char *str = NULL;

    memset(linebuf, 0, MAXLINE);
    gfile = fopen(fpath, "r");
    if (gfile == NULL) {
        outmsg("Couldn't open graph files %s\n", fpath);
        exit(1);
    }

    if (fgets(linebuf, MAXLINE, gfile) == NULL) {
        outmsg("Couldn't find number of vertexes\n");
        return NULL;
    }

    if (sscanf(linebuf, "%d", &nvertex) != 1) {
	    outmsg("ERROR. Malformed graph file header (line 1)\n");
	    return NULL;
    }

    g = (graph_t *)malloc(sizeof(graph_t));
    g->nvertex = nvertex;
    g->vertex = (vertex_t *)malloc(sizeof(vertex_t) * nvertex);

    while (fgets(linebuf, MAXLINE, gfile) != NULL) {

        str = strtok(linebuf, ",");
        int index = 0, num = 0;
        while(str != NULL) {
            sscanf(str, "%d", &num);
            if (index == 0) {
                i = num;
                g->vertex[i].id = i;
                g->vertex[i].is_colored = false;
                g->vertex[i].color = 0;
            } else if (index == 1) {
                g->vertex[i].degree = num;
                g->vertex[i].neighbor = (int *)malloc(sizeof(int) * num);
            } else {
                g->vertex[i].neighbor[index - 2] = num;
            }
            str = strtok(NULL, ",");
            index++;
        }
    }

    return g;

    
}

void outmsg(char *fmt, ...) {
    va_list ap;
    bool got_newline = fmt[strlen(fmt)-1] == '\n';
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    if (!got_newline)
	fprintf(stderr, "\n");
}

void print_graph(graph_t *g) {
    int i, j;
    /*
    for (i = 0; i < g->nvertex; i++) {
        for (j = 0; j < g->vertex[i].degree; j++) {
            printf("%d ", g->vertex[i].neighbor[j]);
        }
        printf("\n");
    }*/
    for (i = 0; i < g->nvertex; i++) {
        printf("%d has color %d\n", i, g->vertex[i].color);
    }

}


bool check_color(graph_t *g) {
    int i, j;
    for (i = 0; i < g->nvertex; i++) {
        if (g->vertex[i].color <= 0 || g->vertex[i].is_colored == false)
            return false;
        
        for (j = 0; j < g->vertex[i].degree; j++) {
            int neightbor = g->vertex[i].neighbor[j];
            if (g->vertex[i].color == g->vertex[neightbor].color)
                return false;
        }
    }
    return true;
}