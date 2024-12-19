/* memory.c    2024-12-19 */

/* Copyright 2021 Plan 9 Foundation, 2024 Emmanuel Paradis */

/* This file is part of the R-package `voronoi'. */
/* See the file ../../LICENSE for licensing issues. */

#include <R.h>
#include <Rinternals.h>
#include "vdefs.h"

extern int sqrt_nsites, siteidx ;

void
freeinit(Freelist * fl, int size)
    {
    fl->head = (Freenode *)NULL ;
    fl->nodesize = size ;
    }

char *
getfree(Freelist * fl)
    {
    int i ;
    Freenode * t ;
    if (fl->head == (Freenode *)NULL)
        {
        t =  (Freenode *) myalloc(sqrt_nsites * fl->nodesize) ;
        for(i = 0 ; i < sqrt_nsites ; i++)
            {
            makefree((Freenode *)((char *)t+i*fl->nodesize), fl) ;
            }
        }
    t = fl->head ;
    fl->head = (fl->head)->nextfree ;
    return ((char *)t) ;
    }

void
makefree(Freenode * curr, Freelist * fl)
    {
    curr->nextfree = fl->head ;
    fl->head = curr ;
    }

int total_alloc ;

char *
myalloc(unsigned n)
    {
    char * t ;
    if ((t=malloc(n)) == (char *) 0)
        {
        error("Insufficient memory processing site %d (%d bytes in use)\n",
        siteidx, total_alloc) ;
        /* exit(0) ; */
        }
    total_alloc += n ;
    return (t) ;
    }