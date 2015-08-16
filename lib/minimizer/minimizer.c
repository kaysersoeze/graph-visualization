/***************************************************************************** 
 * Author: Ludvig Sundström

 * File Name: minimizer.c
 
 * Description: Defines an object able to 'minimize' the energy of the graph
 * G(V, B) created by the set of Vertices V and Bonds B defined in JSON format.
 
 * Creation Date: 24-06-2015

 *****************************************************************************/

#include <unistd.h>

#include "emscripten.h"
#include "util.h"
#include "constants.h"
#include "graph.h"
#include "process_input.h"
#include "energy.h"
#include "gradient.h"
#include "local_minimizer.h"
#include "global_minimizer.h"

#ifndef EMSCRIPT
#define EMSCRIPT 0
#endif

static void js_interact(GraphPointer graph)
{
    assert(graph);
    float *varr;
    int *barr, *zarr;
    varr = NULL;
    barr = NULL; zarr = NULL;
    if (EMSCRIPT) {
        varr = VertexSet_to_array(graph->vs);
        barr = Bondset_to_array(graph->bs);
        zarr = Grid_to_array(graph->grid);
        EM_ASM_({
            window.EXPORTS.processCdata($0, $1, $2, $3, $4, $5);
        }, varr, barr, zarr, 
                graph->vs.n * 2, graph->bs.n * 2, graph->grid->nz * 3);
    }
    free(varr); free(barr); free(zarr);
    varr = NULL; barr = NULL; zarr = NULL;
}

void Minimizer_run(const char *fname) 
{
    assert(fname);

    if (access(fname, R_OK) != -1) {

        GraphPointer graph;
        graph = Graph_create(fname);

        LocalMinimizer_run(graph, Energy_calculate, Gradient_calculate, FTOL);
        js_interact(graph);
        GlobalMinimizer_run(graph, Energy_calculate, Gradient_calculate);
        js_interact(graph);
        
        Graph_free(graph);
        graph = NULL;
        
        assert(!graph);

    } else {
        Util_runtime_error("Minimizer_run(): Can't read file");
    }
}


