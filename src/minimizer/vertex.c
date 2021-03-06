/*****************************************************************************

* File Name: vertex.c

* Author: Ludvpig Sundström

* Description: 

* Creation Date: 28-07-2015

*****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "util.h"
#include "vertex.h"
#include "constants.h"

/* Public  ******************************************************************/

Vertex Vertex_initialize(
       const int id, 
       const Vector pos,
       const double padding,
       char *label,
       const int fixed
    )
{
    Vertex rtn;
    rtn.id = id;
    rtn.mass = DEFAULT_MASS;
    rtn.pos = pos;
    rtn.padding = padding;
    rtn.fixed = fixed;
    rtn.energy = 0.0;
    rtn.gradient = Vector_zero();
    rtn.g = Vector_zero(); 
    rtn.h = Vector_zero();
    Vertex_set_position(&rtn, pos);
    rtn.label = label;
    Vertex_reset_dynamics(&rtn);
    return rtn;
}

VertexPointer Vertex_create(
       const int id, 
       const Vector pos,
       const double padding,
       char *label,
       const int fixed
    )
{
    VertexPointer rtn;
    rtn = Util_allocate_initialize(1, sizeof(Vertex));
    *rtn = Vertex_initialize(id, pos, padding, label, fixed);

    return rtn;
}

void Vertex_reset_dynamics(const VertexPointer v) 
{
    v->next = NULL;
}

Vertex Vertex_copy(const Vertex v)
{
    Vertex rtn;
    rtn.id = v.id;
    rtn.pos = v.pos;
    rtn.padding = v.padding;
    rtn.tl = v.tl;
    rtn.br = v.br;
    rtn.pos0 = v.pos0;
    rtn.grad0 = v.grad0;
    rtn.mass = v.mass;
    rtn.next = v.next;
    rtn.energy = v.energy;
    rtn.gradient = v.gradient;
    rtn.g = v.g;
    rtn.h = v.h;

    return rtn;
}

VertexPointer Vertex_copy_pointer(const VertexPointer v)
{
    VertexPointer rtn;
    rtn = Util_allocate(1, sizeof(Vertex));
    *rtn = Vertex_copy(*v);

    return rtn;
}

void Vertex_set_position(const VertexPointer v, const Vector pos)
{
    v->pos.x = pos.x;
    v->pos.y = pos.y;

    v->tl.x = pos.x - VERTEX_BASE_WIDTH - (v->padding / 2.0);
    v->tl.y = pos.y - VERTEX_BASE_HEIGHT - (v->padding / 2.0);

    v->br.x = pos.x + VERTEX_BASE_WIDTH + (v->padding / 2.0);
    v->br.y = pos.y + VERTEX_BASE_HEIGHT + (v->padding / 2.0);
}

void Vertex_move(const VertexPointer v, const Vector ds) 
{
    if (!v->fixed) {
        if (DEBUG) {
            if (v->id == 0) {
                printf("moving vertex 0:\n");
                Vertex_print(v);
            }
        }
        Vector new_pos;
        new_pos = Vector_add(v->pos0, ds);
        Vertex_set_position(v, new_pos);
    } 
}

void Vertex_print(VertexPointer v) 
{
    char *vecstr = Vector_string(v->pos);
    printf("[id: %d, %s]\n", v->id, vecstr);
    free(vecstr);
}

void Vertex_free(VertexPointer v)
{
    free(v->label);
    free(v);
}

