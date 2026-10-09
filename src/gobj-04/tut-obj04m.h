#pragma once
#include <glib-object.h>
#include "tut-obj04.h"

/* Simple final derived type overriding virtual methods, using convenience macros */

G_BEGIN_DECLS

#define TUT_TYPE_OBJ04M (tut_obj04m_get_type())

G_DECLARE_FINAL_TYPE(TutObj04M, tut_obj04m, TUT, OBJ04M, TutObj04);

/* Public interface to our object */
TutObj04M *tut_obj04m_new (void);
#define tut_obj04m_vwhoami(__self) tut_obj04_vwhoami (TUT_OBJ04 (__self))
#define tut_obj04m_vpure(__self) tut_obj04_vpure (TUT_OBJ04 (__self))

G_END_DECLS
