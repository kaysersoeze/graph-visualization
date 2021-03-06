/*****************************************************************************

* File Name: placement.c

* Author: Ludvig Sundström

* Description: Assign positions (x, y) to a set of vertices

* Creation Date: 13-07-2015

*****************************************************************************/

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

#include "placement.h"
#include "vertex.h"
#include "constants.h"
#include "util.h"

/* Private *******************************************************************/

static int comp_by_mass(const void *elem1, const void *elem2) 
{
    VertexPointer *fst = (VertexPointer *) elem1;
    VertexPointer *snd = (VertexPointer *) elem2;
    if ((*fst)->mass > (*snd)->mass) return 1;
    if ((*fst)->mass < (*snd)->mass) return -1;
    return 0;
}

static int comp_by_id(const void *elem1, const void *elem2) 
{
    VertexPointer *fst = (VertexPointer *) elem1;
    VertexPointer *snd = (VertexPointer *) elem2;
    if ((*fst)->id > (*snd)->id) return 1;
    if ((*fst)->id < (*snd)->id) return -1;
    return 0;
}

/* Public ********************************************************************/

void Placement_set_spiral(VertexSet vs, VertexPointer center)
{
    qsort((void *) vs.set, vs.n, sizeof(void *), comp_by_mass);

    int n;
    n = vs.n;
    while (fabs(sqrt(n) - (int) sqrt(n)) > EPS) {
        n++;
    }
    
    int dimx, dimy;
    dimx = (int) sqrt(n);
    dimy = dimx;
    
    double gapx, gapy;
    gapx = 1.0 / dimx;
    gapy = 1.0 / dimy;
    
    int x, y;
    x = 0; 
    y = 0;
    
    int dx, dy;
    dx = 0;
    dy = -1;

    int t;
    t = fmax(dimx, dimy);

    int i;
    int nfixed;
    Vector offset;
    nfixed = 0;
    offset = Vector_zero();
    for (i = 0; i < vs.n; i++) {
        VertexPointer v = *(vs.set + i);
        if (v->fixed) {
            /*Vector doff = Vector_initialize(v->pos.x, v->pos.y);*/
            /*offset = Vector_add(offset, doff);*/
            nfixed++;
        }
    }
    if (nfixed > 0) {
        /*offset = Vector_scalar_mult(offset, 1.0 / nfixed);*/
        offset = Vector_initialize(0.5, 0.5);
    }  else {
        offset = Vector_initialize(0.5, 0.5);
    }
    Vertex_set_position(center, offset);

    for (i = vs.n - 1; i >= 0; i--) {
        if ((-dimx / 2 <= x && x <= dimx / 2) && 
            (-dimy / 2 <= y && y <= dimy / 2))
        {
            double placex, placey;
            placex = (double) x * gapx;
            placey = (double) y * gapy;
            VertexPointer v = *(vs.set + i);
            if (!v->fixed) {
                Vector initpos = Vector_initialize(placex + offset.x, placey + offset.y);
                Vertex_set_position(v, initpos);
            }
        }
        if ((x == y) || ((x < 0) && (x == -y)) || ((x > 0) && (x == 1 - y))) {
            t = dx;
            dx = -dy;
            dy = t;
        }
        x += dx;
        y += dy; 
    }
    qsort((void *) vs.set, vs.n, sizeof(void *), comp_by_id);
}

void Placement_set_random(const VertexSet vs)
{
    srand(time(NULL));
    int i;
    for (i = 0; i < vs.n; i++) {
        double rx, ry;
        rx = ((double) rand()) / RAND_MAX;
        ry = ((double) rand()) / RAND_MAX;
        Vertex_set_position(*(vs.set + i), Vector_initialize(rx, ry));
    }
}

void Placement_set_grid(VertexSet vs) 
{
    int i, n, vdim, rows, cols;
    double gapx, gapy, offsetx, offsety, x, y;
    n = vs.n; 
    while (fabs(sqrt(n) - (int) sqrt(n)) > EPS) {
        n++;
    }
    vdim = sqrt(n);
    gapx = 1.0 / vdim;
    gapy = 1.0 / vdim;
    offsetx = gapx / 2;
    offsety = gapy / 2;
    rows = 0;
    cols = -1;
    for (i = 0; i < vs.n; i++) {
        if (i % vdim == 0) {
            rows++;
            cols = 0; 
        }
        x = cols * gapx + offsetx;
        y = rows * gapy + offsety; 
        Vertex_set_position(*(vs.set + i), Vector_initialize(x, y));
        cols++;
    }
}

