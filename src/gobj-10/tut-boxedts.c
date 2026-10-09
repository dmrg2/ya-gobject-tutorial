#include "tut-boxedts.h"

G_DEFINE_BOXED_TYPE (TutBoxedTS, tut_boxedts, tut_boxedts_copy, tut_boxedts_free)

TutBoxedTS *
tut_boxedts_new (void)
{
    TutBoxedTS *self;

    g_print ("entered tut_boxedts_new()\n");
    self = g_new0 (TutBoxedTS, 1);
    return self;
}

TutBoxedTS *
tut_boxedts_new_data (gint64 tv_sec, gint64 tv_nsec)
{
    TutBoxedTS *self;

    g_print ("entered tut_boxedts_new_data()\n");
    self = g_new (TutBoxedTS, 1);
    if (self)
    {
        self->ts.tv_sec = tv_sec;
        self->ts.tv_nsec = tv_nsec;
    }
    return self;
}

TutBoxedTS *
tut_boxedts_copy (TutBoxedTS *boxedts)
{
    TutBoxedTS *res;

    g_print ("entered tut_boxedts_copy()\n");
    if (!boxedts) return NULL;
    res = g_new (TutBoxedTS, 1);
    if (res) *res = *boxedts;
    return res;
}

void
tut_boxedts_free (TutBoxedTS *boxedts)
{
    g_print ("entered tut_boxedts_free()\n");
    g_free(boxedts);
}
