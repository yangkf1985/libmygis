/*
    Copyright (c) 2004-2008, Jeremy Cole and others

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef COMPARE_H
#define COMPARE_H

typedef struct compare_st {
  char *name;
  char *oper;
  int (*func) (void *, void *);
} COMPARE;

extern COMPARE compare_all;
extern COMPARE compare_string_cs_eq;
extern COMPARE compare_string_cs_ne;
extern COMPARE compare_string_ci_eq;
extern COMPARE compare_string_ci_ne;

#endif /* COMPARE_H */
