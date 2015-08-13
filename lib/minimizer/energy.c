/*****************************************************************************

* File Name: objective.c 
* Author: Ludvp0g Sundström

* Description: The objective function F = F1 + F2 + F3 + F4 where FN considers
* N vertices. 

* Creation Date: 05-07-2015

*****************************************************************************/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "graph.h"
#include "util.h"

/* Private ******************************************************************/

static double first_order(const VertexSet vs) {

    double rtn;
    rtn = 0;
    int i;
    for (i = 0; i < vs.n; i++) {
        VertexPointer v = *(vs.set + i);
        rtn += Vertex_potential_energy(v);
    }
    return rtn;
}

static double second_order_attraction(const BondSet bs) 
{
    double rtn;
    rtn = 0; 

    int i;
    for (i = 0; i < bs.n; i++) {
        BondPointer bp;
        bp = *(bs.set + i);
        rtn += Bond_attraction_energy(bp);
    }

    return rtn;
}

static double second_order_repulsion(const GridPointer grid) 
{
    double rtn;
    rtn = 0.0;
    
    int i;
    for (i = 0; i < grid->npz; i++) {
        ZonePointer zp = *(grid->pzps + i);
        VertexPointer vp0 = zp->members;
        while (vp0->next) {
            VertexPointer vp1;
            vp1 = vp0->next; 
            while (vp1) {
                Pair pr;
                if (vp0->id > vp1->id) {
                    pr = Pair_initialize(vp1, vp0);
                } else {
                    pr = Pair_initialize(vp0, vp1);
                }
                rtn += VertexPair_repulsion_energy(pr);
                vp1 = vp1->next;
            }
            vp0 = vp0->next;
        }
    }
    ZonePairPointer z2p = grid->azps;
    while (z2p) {
        VertexPointer vp0;
        vp0 = z2p->fst->members;
        while (vp0) {
            VertexPointer vp1;
            vp1 = z2p->snd->members;
            while (vp1) {
                Pair pr;
                if (vp0->id > vp1->id) {
                    pr = Pair_initialize(vp1, vp0);
                } else {
                    pr = Pair_initialize(vp0, vp1);
                }
                rtn += VertexPair_repulsion_energy(pr);
                vp1 = vp1->next;          
            }
            vp0 = vp0->next;
        }
        z2p = z2p->next;
    }
    return rtn;
}

static double second_order(const GraphPointer graph)
{
    double e2a, e2r;
    e2a = second_order_attraction(graph->bs);
    e2r = second_order_repulsion(graph->grid);

    return e2a + e2r; 
}

static double third_order(const BondPairPointer con)
{
    double rtn;
    rtn = 0; 

    BondPairPointer bpr;
    bpr = con;

    while (bpr) {
        rtn += BondPair_angular_energy(bpr);
        bpr = bpr->next;
    }

    return rtn;
}

static double fourth_order(const BondCrossPointer crs)
{
    double rtn;
    rtn = 0;

    BondCrossPointer bcrs;
    bcrs = crs;
    while (bcrs) {
        rtn += BondCross_crossing_energy(bcrs);
        bcrs = bcrs->next;
    }
    return rtn;
}

/* Public *******************************************************************/

double Energy_calculate(const GraphPointer graph) 
{
    double e1, e2, e3, e4;
    e1 = first_order(graph->vs);
    e2 = second_order(graph);
    e3 = third_order(graph->con);
    e4 = fourth_order(graph->crs);

    return e1 + e2 + e3 + e4;
}

/* Testing facade ***********************************************************/

double (*test_first_order_energy)(const VertexSet vs) = first_order;
double (*test_second_order_energy)(const GraphPointer graph) = second_order;
double (*test_second_order_attraction_energy)(const BondSet bs) = 
        second_order_attraction;
double (*test_second_order_repulsion_energy)(const GridPointer grid) = 
        second_order_repulsion;
double (*test_third_order_energy)(const BondPairPointer con) = third_order;
double (*test_fourth_order_energy)(const BondCrossPointer crs) = fourth_order;
