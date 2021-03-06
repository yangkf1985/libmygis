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

#include <stdlib.h>
#include <string.h>

#include "mygis_priv.h"
#include "projection.h"
#include "projection_priv.h"

PROJECTION *projection_init()
{
  PROJECTION *proj = NULL;

  DBUG_ENTER("projection_init");
  if(!(proj = PROJECTION_INIT))
  {
    DBUG_RETURN(NULL);
  }
  
  projection_unset(proj);

  DBUG_RETURN(proj);
}

int projection_set(PROJECTION *proj, char *from, char *to)
{
  DBUG_ENTER("projection_set");

  projection_unset(proj);

  proj->from      = strdup(from);
  proj->to        = strdup(to);

#ifdef HAVE_PROJECTION
  if(!(proj->proj4_pj_from = (projPJ)pj_init_plus(proj->from)))
    goto err2;

  if(!(proj->proj4_pj_to   = (projPJ)pj_init_plus(proj->to)))
  {
    pj_free((projPJ)proj->proj4_pj_from);
    goto err2;
  }
#endif /* HAVE PROJECTION */

  proj->is_set = 1;

  DBUG_RETURN(0);

 err2:
  free(proj->to);
 err1:
  free(proj->from);
 err:
  DBUG_RETURN(-1);
}

void projection_unset(PROJECTION *proj)
{
  DBUG_ENTER("projection_unset");

  if(!proj->is_set)
  {
#ifdef HAVE_PROJECTION
    pj_free((projPJ)proj->proj4_pj_to);
    pj_free((projPJ)proj->proj4_pj_from);
#endif /* HAVE_PROJECTION */
    free(proj->to);
    free(proj->to_name);
    free(proj->from);
    free(proj->from_name);
  }

  proj->proj4_pj_from = NULL;
  proj->proj4_pj_to   = NULL;

  proj->from_name = NULL;
  proj->from      = NULL;
  proj->to_name   = NULL;
  proj->to        = NULL;  

  proj->is_set    = 0;

  DBUG_VOID_RETURN;
}

POINT *projection_transform(PROJECTION *proj, POINT *point)
{
  double trash_z = 0.0;
  POINT *ret = point;
  int rc;

  DBUG_ENTER("projection_transform");

#ifdef HAVE_PROJECTION
  if(!proj->is_set) 
    goto err;

  rc= pj_transform((projPJ)proj->proj4_pj_from, (projPJ)proj->proj4_pj_to,
                   1, 0, &ret->x, &ret->y, &trash_z);

  if(rc)
    DBUG_RETURN(NULL);
#endif /* HAVE_PROJECTION */

 err:
  DBUG_RETURN(ret);
}

void projection_dump(PROJECTION *proj)
{
  DBUG_ENTER("projection_dump");

  printf("\n");
  printf("PROJECTION: Dump: 0x%08x\n", (int)proj);
  printf("  Structure:\n");
  printf("    is_set:         %i\n", proj->is_set);
  printf("    from:           %s\n", proj->is_set?proj->from:"");
  printf("    to:             %s\n", proj->is_set?proj->to:"");
#ifdef HAVE_PROJECTION
  printf("    proj4_pj_from:  " PTR_FORMAT "\n", PTR_CAST(proj->proj4_pj_from));
  printf("    proj4_pj_to:    " PTR_FORMAT "\n", PTR_CAST(proj->proj4_pj_to));
#else
  printf("    (projection support is disabled)\n");
#endif /* HAVE_PROJECTION */
  if(!proj->is_set)
  {
    printf("    (projection is not usable)\n");
  }
  printf("\n\n");

  DBUG_VOID_RETURN;
}

void projection_free(PROJECTION *proj)
{
  DBUG_ENTER("projection_free");
  projection_unset(proj);
  free(proj);
  DBUG_VOID_RETURN;
}

