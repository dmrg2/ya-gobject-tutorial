#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libconfig.h>
#include <getopt.h>
#include "nameutils.h"

#define BAIL(...) ({ fprintf (stderr, __VA_ARGS__); })
#define BAILC(__code, ...) ({ { __code; } fprintf (stderr, __VA_ARGS__); })
#define BAILCT(__code, ...) ({ fprintf (stderr, __VA_ARGS__); { __code; } })
#define BAILR(...) ({ fprintf (stderr, __VA_ARGS__); return; })
#define BAILRC(__code, ...) ({ { __code; } fprintf (stderr, __VA_ARGS__); return; })
#define BAILR0(...) ({ fprintf (stderr ,__VA_ARGS__); return 0; })
#define BAILR0C(__code, ...) ({ { __code; } fprintf (stderr ,__VA_ARGS__); return 0; })

typedef struct _method_info {
    char       *name;
    char       *type;
    char       *arg;
    char       *argnames;
    int         null;
} method_info_t;

typedef struct _class_info {
    char       *classname;
    char       *parentname;
    char       *snakename;
    char       *macroname;
    char       *macrotype;
    char       *m_module;
    char       *m_type;
    int         haspriv;
    int         hasfin;
    int         hasdisp;
    int         derivable;
    int         hasconstr;
    int         vtreserve;
    int         n_ifaces;
    char *     *ifaces;
    int         n_headers;
    char *     *headers;
    int         n_props;
    char *     *props;
    int         n_methods;
    method_info_t *methods;
    int         n_virtuals;
    method_info_t *virtuals;
} class_info_t;

typedef struct _iface_info {
    char       *ifacename;
    char       *parentname;
    char       *snakename;
    char       *macroname;
    char       *macrotype;
    char       *m_module;
    char       *m_type;
    int         vtreserve;
    int         n_prereqs;
    char *     *prereqs;
    int         n_headers;
    char *     *headers;
    int         n_props;
    char *     *props;
    int         n_virtuals;
    method_info_t *virtuals;
} iface_info_t;

static int     display_help = 0;
static int     use_dashnames = 0;
static char *  filename = NULL;
static char *  outdir = NULL;

static void    synopsis (void);
static int     parseargs (int argc, char * argv[]);
static int     init_ifaceinfo (iface_info_t *ii, char *ifacename);
static void    clear_ifaceinfo (iface_info_t *ii);
static int     init_classinfo (class_info_t *ci, char *classname);
static void    clear_classinfo (class_info_t *ci);
static char   *args_to_argnames (const char * args);
static int     parseifaces (char *filename);
static int     parseclasses (char *filename);
static int     parseclass (config_setting_t *class);
static int     parseiface (config_setting_t *iface);

int main (int argc, char * argv[]) {
    if (argc < 2) {
        synopsis ();
        return 0;
    }
    if (!parseargs (argc, argv)) return 1;
    if (!parseifaces (filename)) return 1;
    if (!parseclasses (filename)) return 1;
    return 0;
}

static void synopsis (void) {
    printf ("Use: steamdrum [-p] [-f] [-d] class.cfg | -h");
}

static int parseargs (int argc, char * argv[]) {
    int opt;

    opterr = 1;
    while (-1 != (opt = getopt (argc, argv, "do:h")))
        switch (opt) {
        case 'd':
            use_dashnames = 1;
            break;
        case 'o':
            outdir = optarg;
            break;
        case 'h':
            synopsis ();
            puts ("\t-d use mod-typename names for output files\n"
                  "\t-h display this help");
            display_help = 1;
        case '?':
            BAILR0 ("bad option '%c'\n", optopt);
        }
    if (optind < argc) {
        filename = argv[optind];
    } else if (!display_help) {
        BAILR0 ("need class.cfg parameter\n");
    } else {
        return 1;
    }
    return 1;
}

static int parseifaces (char *filename) {
    int i;
    config_t          cfg;
    config_setting_t *ifaces;

    if (!filename) BAILR0 ("filename is NULL\n");
    if (!*filename) BAILR0 ("filename is empty\n");
    config_init (&cfg);
    if (CONFIG_TRUE != config_read_file (&cfg, filename)) {
        if (config_error_text (&cfg) != NULL)
            BAILR0 ("%s in %s:%d", config_error_text (&cfg), config_error_file (&cfg), config_error_line (&cfg));
        else
            BAILR0 ("error reading file\n");
    }
    ifaces = config_lookup (&cfg, "ifaces");
    if (!ifaces) { config_destroy (&cfg); return 1; }
    if (!config_setting_is_group (ifaces)) BAILR0C (config_destroy (&cfg), "'ifaces' is not group\n");
    for (i = 0; i < config_setting_length (ifaces); i++) {
        config_setting_t *iface;
        if ((iface = config_setting_get_elem (ifaces, i)) == NULL) BAILR0C (config_destroy (&cfg), "config error at iface #%d\n", i);
        if (!config_setting_is_group (iface)) BAILR0C (config_destroy (&cfg), "iface '%s' is not group\n", config_setting_name (iface));
        if (!parseiface (iface)) { config_destroy (&cfg); return 0; }
    }
    config_destroy (&cfg);
    return 1;
}

static int parseclasses (char *filename) {
    int i;
    config_t          cfg;
    config_setting_t *classes;

    if (!filename) BAILR0 ("filename is NULL\n");
    if (!*filename) BAILR0 ("filename is empty\n");
    config_init (&cfg);
    if (CONFIG_TRUE != config_read_file (&cfg, filename)) {
        if (config_error_text (&cfg) != NULL)
            BAILR0 ("%s in %s:%d", config_error_text (&cfg), config_error_file (&cfg), config_error_line (&cfg));
        else
            BAILR0 ("error reading file\n");
    }
    classes = config_lookup (&cfg, "classes");
    if (!classes) { config_destroy (&cfg); return 1; }
    if (!config_setting_is_group (classes)) BAILR0C (config_destroy (&cfg), "'classes'  is not group\n");
    for (i = 0; i < config_setting_length (classes); i++) {
        config_setting_t *class;
        if ((class = config_setting_get_elem (classes, i)) == NULL) BAILR0C (config_destroy (&cfg), "config error at class #%d\n", i);
        if (!config_setting_is_group (class)) BAILR0C (config_destroy (&cfg), "class '%s' is not group\n", config_setting_name (class));
        if (!parseclass (class)) { config_destroy (&cfg); return 0; }
    }
    config_destroy (&cfg);
    return 1;
}

static void __parseiface_hdr (FILE *fout, const char *basename, const iface_info_t *ii);
static void __parseiface_imp (FILE *fout, const char *basename, const iface_info_t *ii);
static void __parseclass_hdr (FILE *fout, const char *basename, const class_info_t *ci);
static void __parseclass_imp (FILE *fout, const char *basename, const class_info_t *ci);

static int parseiface (config_setting_t *iface) {
    config_setting_t *prereqs, *headers, *props, *virtuals;
    iface_info_t ii = {0};
    const char *basename;
    const char *parentname;
    char fname[4096];
    FILE *fout;
    int i, sv;

    if (!iface) return 0;
    if (!init_ifaceinfo (&ii, config_setting_name (iface))) BAILR0C (clear_ifaceinfo (&ii), "init_classinfo() failed for %s", config_setting_name (iface));
    if (use_dashnames)
        basename = to_dashname (ii.snakename);
    else
        basename = to_basename (ii.snakename);

    if (CONFIG_TRUE == config_setting_lookup_string (iface, "parent", &parentname)) {
        if (!parentname) BAILR0 ("%s.parent is NULL\n", config_setting_name (iface));
        if (!*parentname) BAILR0 ("%s.parent is empty\n", config_setting_name (iface));
        ii.parentname = strdup (parentname);
    } else {
        ii.parentname = strdup ("GObject");
    }

    sv = 0;
    if (CONFIG_TRUE == config_setting_lookup_int (iface, "vtreserve", &sv)) ii.vtreserve = sv;

    if ((prereqs = config_setting_lookup (iface, "prereqs")) != NULL) {
        if (!config_setting_is_array (prereqs)) BAILR0C (free ((void *) basename); clear_ifaceinfo (&ii), "%s.prereqs is not array\n", config_setting_name (iface));
        if (config_setting_length (prereqs) == 0) BAILR0C (free ((void *) basename); clear_ifaceinfo (&ii), "%s.prereqs is empty\n", config_setting_name (iface));
        if (config_setting_type (config_setting_get_elem (prereqs, 0)) != CONFIG_TYPE_STRING) BAILR0 ("%s.prereqs must contain strings\n", config_setting_name (iface));
        ii.n_prereqs = 0;
        ii.prereqs = calloc (sizeof (char *), config_setting_length (prereqs) + 1);
        for (i = 0; i < config_setting_length (prereqs); i++) {
            const char *iname = config_setting_get_string_elem (prereqs, i);
            if (!iname) BAILCT (continue, "%s.prereqs[%d] is NULL\n", config_setting_name (iface), i);
            if (!is_camel_case(iname)) BAILCT (continue, "%s.prereqs[%d] is not valid name for interface\n", config_setting_name (iface), i);
            ii.prereqs[ii.n_prereqs++] = strdup (iname);
        }
    }

    if ((headers = config_setting_lookup (iface, "headers")) != NULL) {
        if (!config_setting_is_array (headers)) BAILR0C (free ((void *) basename); clear_ifaceinfo (&ii), "%s.headers is not array\n", config_setting_name (iface));
        if (config_setting_length (headers) == 0) BAILR0C (free ((void *) basename); clear_ifaceinfo (&ii), "%s.headers is empty\n", config_setting_name (iface));
        if (config_setting_type (config_setting_get_elem (headers, 0)) != CONFIG_TYPE_STRING) BAILR0 ("%s.headers must contain strings\n", config_setting_name (iface));
        ii.n_headers = 0;
        ii.headers = calloc (sizeof (char *), config_setting_length (headers) + 1);
        for (i = 0; i < config_setting_length (headers); i++) {
            const char *hname = config_setting_get_string_elem (headers, i);
            if (!hname) BAILCT (continue, "%s.headers[%d] is NULL\n", config_setting_name (iface), i);
            if (!*hname) BAILCT (continue, "%s.headers[%d] is empty\n", config_setting_name (iface), i);
            ii.headers[ii.n_headers++] = strdup (hname);
        }
    }

    if ((props = config_setting_lookup (iface, "props")) != NULL) {
        if (!config_setting_is_array (props)) BAILR0C (free ((void *) basename); clear_ifaceinfo (&ii), "%s.props is not array\n", config_setting_name (iface));
        if (config_setting_length (props) == 0) BAILR0C (free ((void *) basename); clear_ifaceinfo (&ii), "%s.props is empty\n", config_setting_name (iface));
        if (config_setting_type (config_setting_get_elem (props, 0)) != CONFIG_TYPE_STRING) BAILR0 ("%s.props must contain string names\n", config_setting_name (iface));
        ii.n_props = 0;
        ii.props = calloc (sizeof (char *), config_setting_length (props) + 1);
        for (i = 0; i < config_setting_length (props); i++) {
            const char *pname = config_setting_get_string_elem (props, i);
            if (!pname) BAILCT (continue, "%s.props[%d] is NULL\n", config_setting_name (iface), i);
            if (!is_valid_pname (pname)) BAILCT (continue, "%s.props[%d] is not valid property name\n", config_setting_name (iface), i);
            ii.props[ii.n_props++] = strdup (pname);
        }
    }

    if ((virtuals = config_setting_lookup (iface, "virtuals")) != NULL) {
        if (!config_setting_is_group (virtuals)) BAILR0C (free ((void *) basename); clear_ifaceinfo (&ii), "%s.virtuals is not group\n", config_setting_name (iface));
        if (config_setting_length (virtuals) == 0) BAILR0C (free ((void *) basename); clear_ifaceinfo (&ii), "%s.virtuals is empty\n", config_setting_name (iface));
        ii.n_virtuals = 0;
        ii.virtuals = calloc (sizeof (method_info_t), config_setting_length (virtuals) + 1);
        for (i = 0; i < config_setting_length (virtuals); i++) {
            const char *mname, *mtype, *marg, *mnull;
            config_setting_t *method = config_setting_get_elem (virtuals, i);
            if (!method) BAILCT (continue, "config error on %s.virtuals[%d]\n", config_setting_name (iface), i);
            mname = config_setting_name (method);
            if (!is_valid_mname (mname)) BAILCT (continue, "%s.virtuals.'%s' is not valid method name\n", config_setting_name (iface), mname);
            if (!config_setting_is_array (method)) BAILCT (continue, "%s.virtuals.%s must contain array [\"<type>\", \"<args>\"{, \"NULL\"}]\n", config_setting_name (iface), mname);
            if (config_setting_length (method) < 2) BAILCT (continue, "%s.virtuals.%s array length too small\n", config_setting_name (iface), mname);
            if (config_setting_type (config_setting_get_elem (method, 0)) != CONFIG_TYPE_STRING) BAILR0 ("%s.virtuals.%s array must contain strings\n", config_setting_name (iface), mname);
            mtype = config_setting_get_string_elem (method, 0);
            if (!mtype) BAILCT (continue, "%s.virtuals.%s type is NULL\n", config_setting_name (iface), mname);
            if (!*mtype) mtype = "void";
            marg = config_setting_get_string_elem (method, 1);
            if (!marg) BAILCT (continue, "%s.virtuals.%s arg is NULL\n", config_setting_name (iface), mname);
            if (!*marg) marg = "void";
            if (config_setting_length (method) > 2) {
                mnull = config_setting_get_string_elem (method, 2);
                if (!mnull) BAILCT (continue, "%s.virtuals.%s pure flag is NULL\n", config_setting_name (iface), mname);
                if (!*mnull) BAILCT (continue, "%s.virtuals.%s pure flag is empty\n", config_setting_name (iface), mname);
            } else
                mnull = "NULL";
            ii.virtuals[ii.n_virtuals].name = strdup (mname);
            ii.virtuals[ii.n_virtuals].type = strdup (mtype);
            ii.virtuals[ii.n_virtuals].arg = strdup (marg);
            ii.virtuals[ii.n_virtuals].argnames = args_to_argnames (marg);
            ii.virtuals[ii.n_virtuals].null = (strcmp (mnull, "NULL") == 0);
            ii.n_virtuals++;
        }
    }

    memset (fname, 0, sizeof(fname));
    if (outdir && *outdir)
        snprintf (fname, sizeof (fname) - 1, "%s/%s.h", outdir, basename);
    else
        snprintf (fname, sizeof (fname) - 1, "%s.h", basename);
    fout = fopen (fname, "wt");
    if (!fout) BAILR0C (free ((void *) basename); clear_ifaceinfo (&ii), "fopen() failed for %s", fname);
    __parseiface_hdr (fout, basename, &ii);
    fclose (fout);
    printf ("%s\n", fname);

    memset (fname, 0, sizeof(fname));
    if (outdir && *outdir)
        snprintf (fname, sizeof (fname) - 1, "%s/%s.c", outdir, basename);
    else
        snprintf (fname, sizeof (fname) - 1, "%s.c", basename);
    fout = fopen (fname, "wt");
    if (!fout) BAILR0C (free ((void *) basename); clear_ifaceinfo (&ii), "fopen() failed for %s", fname);
    __parseiface_imp (fout, basename, &ii);
    fclose (fout);
    printf ("%s\n", fname);

    free ((void *) basename);
    clear_ifaceinfo (&ii);

    return 1;
}

static int parseclass (config_setting_t *class) {
    config_setting_t *ifaces, *headers, *props, *methods, *virtuals;
    class_info_t ci = {0};
    const char *basename;
    const char *parentname;
    char fname[4096];
    FILE *fout;
    int i, sv;

    if (!class) return 0;
    if (!init_classinfo (&ci, config_setting_name (class))) BAILR0C (clear_classinfo (&ci), "init_classinfo() failed for %s", config_setting_name (class));
    if (use_dashnames)
        basename = to_dashname (ci.snakename);
    else
        basename = to_basename (ci.snakename);

    ci.hasconstr = 1;

    if (CONFIG_TRUE == config_setting_lookup_string (class, "parent", &parentname)) {
        if (!parentname) BAILR0 ("%s.parent is NULL\n", config_setting_name (class));
        if (!*parentname) BAILR0 ("%s.parent is empty\n", config_setting_name (class));
        ci.parentname = strdup (parentname);
    } else {
        ci.parentname = strdup ("GObject");
    }

    sv = 1;
    if (CONFIG_TRUE == config_setting_lookup_bool (class, "hasconstr", &sv)) ci.hasconstr = sv;
    sv = 0;
    if (CONFIG_TRUE == config_setting_lookup_bool (class, "haspriv", &sv)) ci.haspriv = sv;
    if (ci.haspriv) { ci.hasdisp = 1; ci.hasfin = 1; }
    sv = 0;
    if (CONFIG_TRUE == config_setting_lookup_bool (class, "hasfin", &sv)) ci.hasfin = sv;
    sv = 0;
    if (CONFIG_TRUE == config_setting_lookup_bool (class, "hasdisp", &sv)) ci.hasdisp = sv;
    sv = 0;
    if (CONFIG_TRUE == config_setting_lookup_bool (class, "derivable", &sv)) ci.derivable = sv;
    sv = 0;
    if (CONFIG_TRUE == config_setting_lookup_bool (class, "hasconstr", &sv)) ci.hasconstr = sv;
    sv = 0;
    if (CONFIG_TRUE == config_setting_lookup_int (class, "vtreserve", &sv)) ci.vtreserve = sv;

    if ((ifaces = config_setting_lookup (class, "ifaces")) != NULL) {
        if (!config_setting_is_array (ifaces)) BAILR0C (free ((void *) basename); clear_classinfo (&ci), "%s.ifaces is not array\n", config_setting_name (class));
        if (config_setting_length (ifaces) == 0) BAILR0C (free ((void *) basename); clear_classinfo (&ci), "%s.ifaces is empty\n", config_setting_name (class));
        if (config_setting_type (config_setting_get_elem (ifaces, 0)) != CONFIG_TYPE_STRING) BAILR0 ("%s.ifaces must contain strings\n", config_setting_name (class));
        ci.n_ifaces = 0;
        ci.ifaces = calloc (sizeof (char *), config_setting_length (ifaces) + 1);
        for (i = 0; i < config_setting_length (ifaces); i++) {
            const char *iname = config_setting_get_string_elem (ifaces, i);
            if (!iname) BAILCT (continue, "%s.ifaces[%d] is NULL\n", config_setting_name (class), i);
            if (!is_camel_case(iname)) BAILCT (continue, "%s.ifaces[%d] is not valid name for interface\n", config_setting_name (class), i);
            ci.ifaces[ci.n_ifaces++] = strdup (iname);
        }
    }

    if ((headers = config_setting_lookup (class, "headers")) != NULL) {
        if (!config_setting_is_array (headers)) BAILR0C (free ((void *) basename); clear_classinfo (&ci), "%s.headers is not array\n", config_setting_name (class));
        if (config_setting_length (headers) == 0) BAILR0C (free ((void *) basename); clear_classinfo (&ci), "%s.headers is empty\n", config_setting_name (class));
        if (config_setting_type (config_setting_get_elem (headers, 0)) != CONFIG_TYPE_STRING) BAILR0 ("%s.headers must contain strings\n", config_setting_name (class));
        ci.n_headers = 0;
        ci.headers = calloc (sizeof (char *), config_setting_length (headers) + 1);
        for (i = 0; i < config_setting_length (headers); i++) {
            const char *hname = config_setting_get_string_elem (headers, i);
            if (!hname) BAILCT (continue, "%s.headers[%d] is NULL\n", config_setting_name (class), i);
            if (!*hname) BAILCT (continue, "%s.headers[%d] is empty\n", config_setting_name (class), i);
            ci.headers[ci.n_headers++] = strdup (hname);
        }
    }

    if ((props = config_setting_lookup (class, "props")) != NULL) {
        if (!config_setting_is_array (props)) BAILR0C (free ((void *) basename); clear_classinfo (&ci), "%s.props is not array\n", config_setting_name (class));
        if (config_setting_length (props) == 0) BAILR0C (free ((void *) basename); clear_classinfo (&ci), "%s.props is empty\n", config_setting_name (class));
        if (config_setting_type (config_setting_get_elem (props, 0)) != CONFIG_TYPE_STRING) BAILR0 ("%s.props must contain string names\n", config_setting_name (class));
        ci.n_props = 0;
        ci.props = calloc (sizeof (char *), config_setting_length (props) + 1);
        for (i = 0; i < config_setting_length (props); i++) {
            const char *pname = config_setting_get_string_elem (props, i);
            if (!pname) BAILCT (continue, "%s.props[%d] is NULL\n", config_setting_name (class), i);
            if (!is_valid_pname (pname)) BAILCT (continue, "%s.props[%d] is not valid property name\n", config_setting_name (class), i);
            ci.props[ci.n_props++] = strdup (pname);
        }
    }

    if ((methods = config_setting_lookup (class, "methods")) != NULL) {
        if (!config_setting_is_group (methods)) BAILR0C (free ((void *) basename); clear_classinfo (&ci), "%s.methods is not group\n", config_setting_name (class));
        if (config_setting_length (methods) == 0) BAILR0C (free ((void *) basename); clear_classinfo (&ci), "%s.methods is empty\n", config_setting_name (class));
        ci.n_methods = 0;
        ci.methods = calloc (sizeof (method_info_t), config_setting_length (methods) + 1);
        for (i = 0; i < config_setting_length (methods); i++) {
            const char *mname, *mtype, *marg;
            config_setting_t *method = config_setting_get_elem (methods, i);
            if (!method) BAILCT (continue, "config error on %s.methods[%d]\n", config_setting_name (class), i);
            mname = config_setting_name (method);
            if (!is_valid_mname (mname)) BAILCT (continue, "%s.methods[%d] name is not valid method name\n", config_setting_name (class), i);
            if (!config_setting_is_array (method)) BAILCT (continue, "%s.methods[%d] must contain array [\"<type>\", \"<args>\"]\n", config_setting_name (class), i);
            if (config_setting_length (method) < 2) BAILCT (continue, "%s.methods[%d] array length too small\n", config_setting_name (class), i);
            if (config_setting_type (config_setting_get_elem (method, 0)) != CONFIG_TYPE_STRING) BAILR0 ("%s.methods[%d] array must contain strings\n", config_setting_name (class), i);
            mtype = config_setting_get_string_elem (method, 0);
            if (!mtype) BAILCT (continue, "%s.methods[%d] type is NULL\n", config_setting_name (class), i);
            if (!*mtype) mtype = "void";
            marg = config_setting_get_string_elem (method, 1);
            if (!marg) BAILCT (continue, "%s.methods[%d] arg is NULL\n", config_setting_name (class), i);
            if (!*marg) marg = "void";
            ci.methods[ci.n_methods].name = strdup (mname);
            ci.methods[ci.n_methods].type = strdup (mtype);
            ci.methods[ci.n_methods].arg = strdup (marg);
            ci.n_methods++;
        }
    }

    if ((virtuals = config_setting_lookup (class, "virtuals")) != NULL) {
        if (!config_setting_is_group (virtuals)) BAILR0C (free ((void *) basename); clear_classinfo (&ci), "%s.virtuals is not group\n", config_setting_name (class));
        if (config_setting_length (virtuals) == 0) BAILR0C (free ((void *) basename); clear_classinfo (&ci), "%s.virtuals is empty\n", config_setting_name (class));
        ci.n_virtuals = 0;
        ci.virtuals = calloc (sizeof (method_info_t), config_setting_length (virtuals) + 1);
        for (i = 0; i < config_setting_length (virtuals); i++) {
            const char *mname, *mtype, *marg, *mnull;
            config_setting_t *method = config_setting_get_elem (virtuals, i);
            if (!method) BAILCT (continue, "config error on %s.virtuals[%d]\n", config_setting_name (class), i);
            mname = config_setting_name (method);
            if (!is_valid_mname (mname)) BAILCT (continue, "%s.virtuals.'%s' is not valid method name\n", config_setting_name (class), mname);
            if (!config_setting_is_array (method)) BAILCT (continue, "%s.virtuals.%s must contain array [\"<type>\", \"<args>\"{, \"NULL\"}]\n", config_setting_name (class), mname);
            if (config_setting_length (method) < 2) BAILCT (continue, "%s.virtuals.%s array length too small\n", config_setting_name (class), mname);
            if (config_setting_type (config_setting_get_elem (method, 0)) != CONFIG_TYPE_STRING) BAILR0 ("%s.virtuals.%s array must contain strings\n", config_setting_name (class), mname);
            mtype = config_setting_get_string_elem (method, 0);
            if (!mtype) BAILCT (continue, "%s.virtuals.%s type is NULL\n", config_setting_name (class), mname);
            if (!*mtype) mtype = "void";
            marg = config_setting_get_string_elem (method, 1);
            if (!marg) BAILCT (continue, "%s.virtuals.%s arg is NULL\n", config_setting_name (class), mname);
            if (!*marg) marg = "void";
            if (config_setting_length (method) > 2) {
                mnull = config_setting_get_string_elem (method, 2);
                if (!mnull) BAILCT (continue, "%s.virtuals.%s pure flag is NULL\n", config_setting_name (class), mname);
                if (!*mnull) BAILCT (continue, "%s.virtuals.%s pure flag is empty\n", config_setting_name (class), mname);
            } else
                mnull = "IMPL";
            ci.virtuals[ci.n_virtuals].name = strdup (mname);
            ci.virtuals[ci.n_virtuals].type = strdup (mtype);
            ci.virtuals[ci.n_virtuals].arg = strdup (marg);
            ci.virtuals[ci.n_virtuals].argnames = args_to_argnames (marg);
            ci.virtuals[ci.n_virtuals].null = (strcmp (mnull, "NULL") == 0);
            ci.n_virtuals++;
        }
    }

    memset (fname, 0, sizeof(fname));
    if (outdir && *outdir)
        snprintf (fname, sizeof (fname) - 1, "%s/%s.h", outdir, basename);
    else
        snprintf (fname, sizeof (fname) - 1, "%s.h", basename);
    fout = fopen (fname, "wt");
    if (!fout) BAILR0C (free ((void *) basename); clear_classinfo (&ci), "fopen() failed for %s", fname);
    __parseclass_hdr (fout, basename, &ci);
    fclose (fout);
    printf ("%s\n", fname);

    memset (fname, 0, sizeof(fname));
    if (outdir && *outdir)
        snprintf (fname, sizeof (fname) - 1, "%s/%s.c", outdir, basename);
    else
        snprintf (fname, sizeof (fname) - 1, "%s.c", basename);
    fout = fopen (fname, "wt");
    if (!fout) BAILR0C (free ((void *) basename); clear_classinfo (&ci), "fopen() failed for %s", fname);
    __parseclass_imp (fout, basename, &ci);
    fclose (fout);
    printf ("%s\n", fname);

    free ((void *) basename);
    clear_classinfo (&ci);

    return 1;
}

static void __parseiface_hdr (FILE *fout, const char *basename, const iface_info_t *ii) {
    int i;

    fprintf (fout, "#pragma once\n\n#include <glib-object.h>\n\n");
    if (ii->n_headers) {
        for (i = 0; i < ii->n_headers; i++)
            fprintf (fout, "#include \"%s\"\n", ii->headers[i]);
        fprintf (fout, "\n");
    }
    fprintf (fout, "G_BEGIN_DECLS\n\n");
    fprintf (fout, "#define %s (%s_get_type ())\n\n", ii->macrotype, ii->snakename);
    fprintf (fout, "G_DECLARE_INTERFACE (%s, %s, %s, %s, %s);\n\n", ii->ifacename, ii->snakename, ii->m_module, ii->m_type, ii->parentname);
    fprintf (fout, "struct _%sInterface\n{\n", ii->ifacename);
    fprintf (fout, "  GTypeInterface parent_iface;\n");
    if (ii->n_virtuals) {
        for (i = 0; i < ii->n_virtuals; i++) {
            if (strcmp (ii->virtuals[i].arg, "void") == 0)
                fprintf (fout, "  %s (*%s) (%s *self);\n", ii->virtuals[i].type, ii->virtuals[i].name, ii->ifacename);
            else
                fprintf (fout, "  %s (*%s) (%s *self, %s);\n", ii->virtuals[i].type, ii->virtuals[i].name, ii->ifacename, ii->virtuals[i].arg);
        }
    }
    if (ii->vtreserve)
        fprintf (fout, "  gpointer __vtable_resv[%d];\n", ii->vtreserve);
    fprintf (fout, "};\n\n");
    if (ii->n_virtuals) {
        for (i = 0; i < ii->n_virtuals; i++) {
            if (strcmp (ii->virtuals[i].arg, "void") == 0)
                fprintf (fout, "%s %s_%s (%s *self);\n", ii->virtuals[i].type, ii->snakename, ii->virtuals[i].name, ii->ifacename);
            else
                fprintf (fout, "%s %s_%s (%s *self, %s);\n", ii->virtuals[i].type, ii->snakename, ii->virtuals[i].name, ii->ifacename, ii->virtuals[i].arg);
        }
        fprintf (fout, "\n");
    }
    fprintf (fout, "G_END_DECLS\n");
}

static void __parseclass_hdr (FILE *fout, const char *basename, const class_info_t *ci) {
    int i;

    fprintf (fout, "#pragma once\n\n#include <glib-object.h>\n\n");
    if (ci->n_headers) {
        for (i = 0; i < ci->n_headers; i++)
            fprintf (fout, "#include \"%s\"\n", ci->headers[i]);
        fprintf (fout, "\n");
    }
    fprintf (fout, "G_BEGIN_DECLS\n\n");
    fprintf (fout, "#define %s (%s_get_type ())\n\n", ci->macrotype, ci->snakename);
    if (ci->derivable)
        fprintf (fout, "G_DECLARE_DERIVABLE_TYPE (%s, %s, %s, %s, %s);\n\n", ci->classname, ci->snakename, ci->m_module, ci->m_type, ci->parentname);
    else
        fprintf (fout, "G_DECLARE_FINAL_TYPE (%s, %s, %s, %s, %s);\n\n", ci->classname, ci->snakename, ci->m_module, ci->m_type, ci->parentname);
    if (ci->derivable) {
        fprintf (fout, "struct _%sClass\n{\n", ci->classname);
        fprintf (fout, "  %sClass parent_class;\n", ci->parentname);
        if (ci->n_virtuals) {
            for (i = 0; i < ci->n_virtuals; i++) {
                if (strcmp (ci->virtuals[i].arg, "void") == 0)
                    fprintf (fout, "  %s (*%s) (%s *self);\n", ci->virtuals[i].type, ci->virtuals[i].name, ci->classname);
                else
                    fprintf (fout, "  %s (*%s) (%s *self, %s);\n", ci->virtuals[i].type, ci->virtuals[i].name, ci->classname, ci->virtuals[i].arg);
            }
        }
        if (ci->vtreserve)
            fprintf (fout, "  gpointer __vtable_resv[%d];\n", ci->vtreserve);
        fprintf (fout, "};\n\n");
    }
    if (ci->hasconstr)
        fprintf (fout, "%s * %s_new (void);\n", ci->classname, ci->snakename);
    if (ci->n_methods) {
        for (i = 0; i < ci->n_methods; i++) {
            if (strcmp (ci->methods[i].arg, "void") == 0)
                fprintf (fout, "%s %s_%s (%s *self);\n", ci->methods[i].type, ci->snakename, ci->methods[i].name, ci->classname);
            else
                fprintf (fout, "%s %s_%s (%s *self, %s);\n", ci->methods[i].type, ci->snakename, ci->methods[i].name, ci->classname, ci->methods[i].arg);
        }
        fprintf (fout, "\n");
    }
    fprintf (fout, "G_END_DECLS\n");
}

static void __parseiface_imp (FILE *fout, const char *basename, const iface_info_t *ii) {
    int i;

    fprintf (fout, "#include \"%s.h\"\n\n", basename);

    /* std function prototypes */
    fprintf (fout, "static void         %s_default_init (%sInterface *iface);\n", ii->snakename, ii->ifacename);
    if (ii->n_virtuals) {
        for (i = 0; i < ii->n_virtuals; i++)
            if (!ii->virtuals[i].null) {
                if (strcmp (ii->virtuals[i].arg, "void") == 0)
                    fprintf (fout, "static %s %s_%s_impl (%s *self);\n", ii->virtuals[i].type, ii->snakename, ii->virtuals[i].name, ii->ifacename);
                else
                    fprintf (fout, "static %s %s_%s_impl (%s *self, %s);\n", ii->virtuals[i].type, ii->snakename, ii->virtuals[i].name, ii->ifacename, ii->virtuals[i].arg);
            }
    }
    fprintf (fout, "\n");

    /* iface definition macro */
    fprintf (fout, "G_DEFINE_INTERFACE (%s, %s, %s);\n\n", ii->ifacename, ii->snakename, ii->macrotype);

    /* mod_type_default_init () */
    fprintf (fout, "static void\n%s_default_init (%sInterface *iface)\n{\n", ii->snakename, ii->ifacename);
    if (ii->n_props) {
        for (i = 0; i < ii->n_props; i++) {
            fprintf (fout, "  g_object_interface_install_property (\n");
            fprintf (fout, "    iface,\n");
            fprintf (fout, "    g_param_spec_string (\"%s\",\n", ii->props[i]);
            fprintf (fout, "                         \"Short %s\",\n", ii->props[i]);
            fprintf (fout, "                         \"Long %s description\",\n", ii->props[i]);
            fprintf (fout, "                         NULL,\n");
            fprintf (fout, "                         G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));\n");
        }
    }
    if (ii->n_virtuals) {
        for (i = 0; i < ii->n_virtuals; i++) {
            if (!ii->virtuals[i].null)
                fprintf (fout, "  iface->%s = %s_%s_impl;\n", ii->virtuals[i].name, ii->snakename, ii->virtuals[i].name);
            else
                fprintf (fout, "  iface->%s = NULL;\n", ii->virtuals[i].name);
        }
    }
    if (ii->n_prereqs) {
        for (i = 0; i < ii->n_prereqs; i++) {
            const char *itype = to_macro_type (ii->prereqs[i]);
            if (itype) {
                fprintf (fout, "  g_type_interface_add_prerequisite (%s, %s);\n", ii->macrotype, itype);
                free ((void *) itype);
            }
        }
    }
    fprintf (fout, "  /* XXX %s::default_init() */\n", ii->ifacename);
    fprintf (fout, "}\n\n");

    /* virtuals implementations */
    if (ii->n_virtuals) {
        fprintf (fout, "\n");
        for (i = 0; i < ii->n_virtuals; i++)
            if (!ii->virtuals[i].null) {
                if (strcmp (ii->virtuals[i].arg, "void") == 0)
                    fprintf (fout, "static %s\n%s_%s_impl (%s *self)\n", ii->virtuals[i].type, ii->snakename, ii->virtuals[i].name, ii->ifacename);
                else
                    fprintf (fout, "static %s\n%s_%s_impl (%s *self, %s)\n", ii->virtuals[i].type, ii->snakename, ii->virtuals[i].name, ii->ifacename, ii->virtuals[i].arg);
                fprintf (fout, "{\n");
                fprintf (fout, "  /* XXX virtual %s::%s() */\n", ii->ifacename, ii->virtuals[i].name);
                if (strcmp (ii->virtuals[i].type, "void") != 0)
                    fprintf (fout, "  return (%s) 0;\n", ii->virtuals[i].type);
                fprintf (fout, "}\n");
            }
    }
    fprintf (fout, "\n");

    /* public interface to virtuals */
    if (ii->n_virtuals) {
        for (i = 0; i < ii->n_virtuals; i++) {
            fprintf (fout, "\n");
            if (strcmp (ii->virtuals[i].arg, "void") == 0)
                fprintf (fout, "%s\n%s_%s (%s *self)\n", ii->virtuals[i].type, ii->snakename, ii->virtuals[i].name, ii->ifacename);
            else
                fprintf (fout, "%s\n%s_%s (%s *self, %s)\n", ii->virtuals[i].type, ii->snakename, ii->virtuals[i].name, ii->ifacename, ii->virtuals[i].arg);
            fprintf (fout, "{\n");
            fprintf (fout, "  %sInterface *iface;\n", ii->ifacename);
            if (strcmp (ii->virtuals[i].type, "void") != 0) {
                fprintf (fout, "  %s res;\n\n", ii->virtuals[i].type);
                fprintf (fout, "  g_return_val_if_fail (%s_IS_%s (self), (%s) 0);\n", ii->m_module, ii->m_type, ii->virtuals[i].type);
                fprintf (fout, "  iface = %s_GET_IFACE (self);\n", ii->macroname);
                fprintf (fout, "  g_return_val_if_fail (iface->%s != NULL, (%s) 0);\n", ii->virtuals[i].name, ii->virtuals[i].type);
                if (ii->virtuals[i].argnames) {
                    fprintf (fout, "  res = iface->%s (self, %s);\n", ii->virtuals[i].name, ii->virtuals[i].argnames);
                } else {
                    fprintf (fout, "  res = iface->%s (self);\n", ii->virtuals[i].name);
                }
                fprintf (fout, "  /* XXX virtual %s::%s();*/;\n", ii->ifacename, ii->virtuals[i].name);
                fprintf (fout, "  return res;\n");
            } else {
                fprintf (fout, "\n");
                fprintf (fout, "  g_return_if_fail (%s_IS_%s (self));\n", ii->m_module, ii->m_type);
                fprintf (fout, "  iface = %s_GET_IFACE (self);\n", ii->macroname);
                fprintf (fout, "  g_return_if_fail (iface->%s != NULL);\n", ii->virtuals[i].name);
                if (ii->virtuals[i].argnames) {
                    fprintf (fout, "  iface->%s (self, %s);\n", ii->virtuals[i].name, ii->virtuals[i].argnames);
                } else {
                    fprintf (fout, "  iface->%s (self);\n", ii->virtuals[i].name);
                }
                fprintf (fout, "  /* XXX virtual %s::%s();*/;\n", ii->ifacename, ii->virtuals[i].name);
            }
            fprintf (fout, "}\n");
        }
    }
}

static void __parseclass_imp (FILE *fout, const char *basename, const class_info_t *ci) {
    int i;

    fprintf (fout, "#include \"%s.h\"\n\n", basename);

    /* property enum */
    if (ci->n_props) {
        char *pmacro;

        fprintf (fout, "typedef enum _%sPropEnum\n{\n", ci->classname);
        pmacro = pname_to_macro (ci->props[0]);
        fprintf (fout, "  PROP_%s = 1,\n", pmacro);
        free (pmacro);
        for (i = 1; i < ci->n_props; i++) {
            pmacro = pname_to_macro (ci->props[i]);
            fprintf (fout, "  PROP_%s,\n", pmacro);
            free (pmacro);
        }
        fprintf (fout, "  N_PROPERTIES\n");
        fprintf (fout, "} %sPropEnum;\n\n", ci->classname);
    }

    /* class structure */
    if (!ci->derivable) {
        fprintf (fout, "struct _%sClass\n{\n", ci->classname);
        fprintf (fout, "  %sClass parent_class;\n", ci->parentname);
        if (ci->n_virtuals) {
            for (i = 0; i < ci->n_virtuals; i++) {
                if (strcmp (ci->virtuals[i].arg, "void") == 0)
                    fprintf (fout, "  %s (*%s) (%s *self);\n", ci->virtuals[i].type, ci->virtuals[i].name, ci->classname);
                else
                    fprintf (fout, "  %s (*%s) (%s *self, %s);\n", ci->virtuals[i].type, ci->virtuals[i].name, ci->classname, ci->virtuals[i].arg);
            }
        }
        if (ci->vtreserve)
            fprintf (fout, "  gpointer __vtable_resv[%d];\n", ci->vtreserve);
        fprintf (fout, "};\n\n");
    }

    /* instance structure */
    fprintf (fout, "struct _%s\n{\n  %s parent_instance;\n};\n\n", ci->classname, ci->parentname);

    /* private structure */
    if (ci->haspriv) {
        fprintf (fout, "typedef struct _%sPrivate\n{\n", ci->classname);
        if (ci->n_props) {
            for (i = 0; i < ci->n_props; i++) {
                char *psnake = pname_to_snake (ci->props[i]);
                if (psnake)
                    fprintf (fout, "  gchar *%s;\n", psnake);
            }
        } else {
            fprintf (fout, "  gpointer dummy;\n");
        }
        fprintf (fout, "} %sPrivate;\n\n", ci->classname);
    }

    /* std function prototypes */
    fprintf (fout, "static void         %s_class_init (%sClass *klass);\n", ci->snakename, ci->classname);
    fprintf (fout, "static void         %s_init (%s *self);\n", ci->snakename, ci->classname);
    if (ci->hasdisp)
        fprintf (fout, "static void         %s_dispose (GObject *object);\n", ci->snakename);
    if (ci->hasfin)
        fprintf (fout, "static void         %s_finalize (GObject *object);\n", ci->snakename);
    if (ci->n_props) {
        fprintf (fout, "static void         %s_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);\n", ci->snakename);
        fprintf (fout, "static void         %s_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);\n", ci->snakename);
    }
    if (ci->n_ifaces) {
        for (i = 0; i < ci->n_ifaces; i++) {
            const char *isnake, *ipart;
            isnake = to_snake_case (ci->ifaces[i]);
            ipart = part_typename (isnake);
            if (ipart) {
                fprintf (fout, "static void         %s_%s_init (%sInterface *iface);\n", ci->snakename, ipart, ci->ifaces[i]);
                free ((void *) isnake);
                free ((void *) ipart);
            }
        }
    }
    if (ci->n_virtuals) {
        for (i = 0; i < ci->n_virtuals; i++)
            if (!ci->virtuals[i].null) {
                if (strcmp (ci->virtuals[i].arg, "void") == 0)
                    fprintf (fout, "static %s %s_%s_impl (%s *self);\n", ci->virtuals[i].type, ci->snakename, ci->virtuals[i].name, ci->classname);
                else
                    fprintf (fout, "static %s %s_%s_impl (%s *self, %s);\n", ci->virtuals[i].type, ci->snakename, ci->virtuals[i].name, ci->classname, ci->virtuals[i].arg);
            }
    }
    fprintf (fout, "\n");

    /* class definition macro */
    fprintf (fout, "G_DEFINE_TYPE_EXTENDED (%s, %s,\n", ci->classname, ci->snakename);
    fprintf (fout, "                        %s,\n", ci->macrotype);
    if (ci->derivable)
        fprintf (fout, "                        G_TYPE_FLAG_NONE,\n");
    else
        fprintf (fout, "                        G_TYPE_FLAG_FINAL,\n");
    if (ci->haspriv)
        fprintf (fout, "                        G_ADD_PRIVATE (%s)", ci->classname);
    else
        fprintf (fout, "                        {}");
    if (ci->n_ifaces) {
        for (i = 0; i < ci->n_ifaces; i++) {
            const char *isnake, *ipart, *itype;
            isnake = to_snake_case (ci->ifaces[i]);
            itype = to_macro_type (ci->ifaces[i]);
            ipart = part_typename (isnake);
            if (ipart) {
                fprintf (fout, "\n                        G_IMPLEMENT_INTERFACE (%s,", itype);
                fprintf (fout, "\n                                               %s_%s_init)", ci->snakename, ipart);
                free ((void *) isnake);
                free ((void *) itype);
                free ((void *) ipart);
            }
        }
    }
    fprintf (fout, ");\n\n");

    /* pspec cache */
    if (ci->n_props)
        fprintf (fout, "static GParamSpec *%s_prop_pspec[N_PROPERTIES] = {0};\n\n", ci->snakename);

    /* mod_type_class_init () */
    fprintf (fout, "static void\n%s_class_init (%sClass *klass)\n{\n", ci->snakename, ci->classname);
    if (ci->hasdisp || ci->hasfin || ci->n_props)
        fprintf (fout, "  GObjectClass *object_class = G_OBJECT_CLASS (klass);\n");
    fprintf (fout, "\n");
    if (ci->hasdisp)
        fprintf (fout, "  object_class->dispose = %s_dispose;\n", ci->snakename);
    if (ci->hasfin)
        fprintf (fout, "  object_class->finalize = %s_finalize;\n", ci->snakename);
    if (ci->n_props) {
        fprintf (fout, "  object_class->get_property = %s_get_property;\n", ci->snakename);
        fprintf (fout, "  object_class->set_property = %s_set_property;\n", ci->snakename);
    }
    if (ci->n_props) {
        for (i = 0; i < ci->n_props; i++) {
            char *pmacro = pname_to_macro (ci->props[i]);
            if (pmacro) {
                fprintf (fout, "  %s_prop_pspec[PROP_%s] =\n", ci->snakename, pmacro);
                fprintf (fout, "    g_param_spec_string (\"%s\",\n", ci->props[i]);
                fprintf (fout, "                         \"Short %s\",\n", ci->props[i]);
                fprintf (fout, "                         \"Long %s description\",\n", ci->props[i]);
                fprintf (fout, "                         NULL,\n");
                fprintf (fout, "                         G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);\n");
                free (pmacro);
            }
        }
        fprintf (fout, "  g_object_class_install_properties (object_class, N_PROPERTIES, %s_prop_pspec);\n", ci->snakename);
    }
    if (ci->n_virtuals) {
        for (i = 0; i < ci->n_virtuals; i++) {
            if (!ci->virtuals[i].null)
                fprintf (fout, "  klass->%s = %s_%s_impl;\n", ci->virtuals[i].name, ci->snakename, ci->virtuals[i].name);
            else
                fprintf (fout, "  klass->%s = NULL;\n", ci->virtuals[i].name);
        }
    }
    fprintf (fout, "  /* XXX %s::class_init() */\n", ci->classname);
    fprintf (fout, "}\n\n");

    /* mod_type_init () */
    fprintf (fout, "static void\n%s_init (%s * self)\n{\n", ci->snakename, ci->classname);
    if (ci->haspriv) {
        fprintf (fout, "  %sPrivate *priv;\n\n", ci->classname);
        fprintf (fout, "  priv = %s_get_instance_private (self);\n  (void)priv;\n", ci->snakename);
    }
    fprintf (fout, "  /* XXX %s::init() */\n", ci->classname);
    fprintf (fout, "}\n\n");

    /* interfaces init */
    if (ci->n_ifaces) {
        for (i = 0; i < ci->n_ifaces; i++) {
            const char *isnake, *ipart;
            isnake = to_snake_case (ci->ifaces[i]);
            ipart = part_typename (isnake);
            if (ipart) {
                fprintf (fout, "static void\n%s_%s_init (%sInterface *iface)\n{\n", ci->snakename, ipart, ci->ifaces[i]);
                fprintf (fout, "  /* XXX %s::%sInterface::init() */\n", ci->classname, ci->ifaces[i]);
                fprintf (fout, "}\n\n");
                free ((void *) isnake);
                free ((void *) ipart);
            }
        }
    }

    /* mod_type_dispose () */
    if (ci->hasdisp) {
        fprintf (fout, "static void\n%s_dispose (GObject *object)\n{\n", ci->snakename);
        if (ci->haspriv) {
            fprintf (fout, "  %sPrivate *priv;\n\n", ci->classname);
            fprintf (fout, "  priv = %s_get_instance_private (%s (object));  (void)priv;\n\n", ci->snakename, ci->macroname);
        }
        fprintf (fout, "  /* XXX %s::dispose() */\n", ci->classname);
        fprintf (fout, "  G_OBJECT_CLASS (%s_parent_class)->dispose (object);\n", ci->snakename);
        fprintf (fout, "}\n\n");
    }

    /* mod_type_finalize () */
    if (ci->hasfin) {
        fprintf (fout, "static void\n%s_finalize (GObject *object)\n{\n", ci->snakename);
        if (ci->haspriv) {
            fprintf (fout, "  %sPrivate *priv;\n\n", ci->classname);
            fprintf (fout, "  priv = %s_get_instance_private (%s (object));  (void)priv;\n\n", ci->snakename, ci->macroname);
            if (ci->n_props) {
                for (i = 0; i < ci->n_props; i++) {
                    char *psnake = pname_to_snake (ci->props[i]);
                    if (psnake) {
                        fprintf (fout, "  g_free (priv->%s);\n", psnake);
                        free (psnake);
                    }
                }
            }
        }
        fprintf (fout, "  /* XXX %s::finalize() */\n", ci->classname);
        fprintf (fout, "  G_OBJECT_CLASS (%s_parent_class)->finalize (object);\n", ci->snakename);
        fprintf (fout, "}\n\n");
    }

    /* mod_type_get_property () */
    if (ci->n_props) {
        fprintf (fout, "static void\n%s_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)\n{\n", ci->snakename);
        if (ci->haspriv)
            fprintf (fout, "  %sPrivate *priv;\n\n", ci->classname);
        fprintf (fout, "  g_return_if_fail (%s_IS_%s (object));\n", ci->m_module, ci->m_type);
        if (ci->haspriv)
            fprintf (fout, "  priv = %s_get_instance_private (%s (object));  (void)priv;\n\n", ci->snakename, ci->macroname);
        fprintf (fout, "  switch ((%sPropEnum) property_id)\n  {\n", ci->classname);
        for (i = 0; i < ci->n_props; i++) {
            char *pmacro = pname_to_macro (ci->props[i]);
            char *psnake = pname_to_snake (ci->props[i]);
            if (pmacro) {
                fprintf (fout, "    case PROP_%s:\n", pmacro);
                fprintf (fout, "      /* XXX %s::property[\"%s\"] */\n", ci->classname, ci->props[i]);
                if (ci->haspriv)
                    fprintf (fout, "      g_value_set_string (value, priv->%s);\n", psnake);
                fprintf (fout, "      break;\n");
                free (pmacro);
                free (psnake);
            }
        }
        fprintf (fout, "    default:\n");
        fprintf (fout, "      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);\n");
        fprintf (fout, "    }\n");
        fprintf (fout, "}\n\n");
    }

    /* mod_type_set_property () */
    if (ci->n_props) {
        fprintf (fout, "static void\n%s_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)\n{\n", ci->snakename);
        if (ci->haspriv)
            fprintf (fout, "  %sPrivate *priv;\n\n", ci->classname);
        fprintf (fout, "  g_return_if_fail (%s_IS_%s (object));\n", ci->m_module, ci->m_type);
        if (ci->haspriv)
            fprintf (fout, "  priv = %s_get_instance_private (%s (object));\n  (void)priv;\n", ci->snakename, ci->macroname);
        fprintf (fout, "  switch ((%sPropEnum) property_id)\n  {\n", ci->classname);
        for (i = 0; i < ci->n_props; i++) {
            char *pmacro = pname_to_macro (ci->props[i]);
            char *psnake = pname_to_snake (ci->props[i]);
            if (pmacro && psnake) {
                fprintf (fout, "    case PROP_%s:\n", pmacro);
                fprintf (fout, "      /* XXX %s::property[\"%s\"] */\n", ci->classname, ci->props[i]);
                if (ci->haspriv) {
                    fprintf (fout, "      g_free (priv->%s);\n", psnake);
                    fprintf (fout, "      priv->%s = g_value_dup_string (value);\n", psnake);
                }
                fprintf (fout, "      break;\n");
                free (pmacro);
                free (psnake);
            }
        }
        fprintf (fout, "    default:\n");
        fprintf (fout, "      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);\n");
        fprintf (fout, "    }\n");
        fprintf (fout, "}\n");
    }

    /* virtuals implementations */
    if (ci->n_virtuals) {
        fprintf (fout, "\n");
        for (i = 0; i < ci->n_virtuals; i++)
            if (!ci->virtuals[i].null) {
                if (strcmp (ci->virtuals[i].arg, "void") == 0)
                    fprintf (fout, "static %s\n%s_%s_impl (%s *self)\n", ci->virtuals[i].type, ci->snakename, ci->virtuals[i].name, ci->classname);
                else
                    fprintf (fout, "static %s\n%s_%s_impl (%s *self, %s)\n", ci->virtuals[i].type, ci->snakename, ci->virtuals[i].name, ci->classname, ci->virtuals[i].arg);
                fprintf (fout, "{\n");
                if (ci->haspriv)
                    fprintf (fout, "  %sPrivate *priv;\n\n", ci->classname);
                if (ci->haspriv)
                    fprintf (fout, "  priv = %s_get_instance_private (self);\n  (void)priv;\n", ci->snakename);
                fprintf (fout, "  /* XXX virtual %s::%s() */\n", ci->classname, ci->virtuals[i].name);
                if (strcmp (ci->virtuals[i].type, "void") != 0)
                    fprintf (fout, "  return (%s) 0;\n", ci->virtuals[i].type);
                fprintf (fout, "}\n");
            }
    }
    fprintf (fout, "\n");

    /* mod_type_new () */
    if (ci->hasconstr) {
        fprintf (fout, "%s *\n%s_new (void)\n{\n", ci->classname, ci->snakename);
        fprintf (fout, "  %s *self;\n", ci->classname);
        if (ci->haspriv)
            fprintf (fout, "  %sPrivate *priv;\n\n", ci->classname);
        fprintf (fout, "\n");
        fprintf (fout, "  self = %s (g_object_new (%s, NULL));\n", ci->macroname, ci->macrotype);
        if (ci->haspriv)
            fprintf (fout, "  priv = %s_get_instance_private (self);\n  (void)priv;\n", ci->snakename);
        fprintf (fout, "  /* XXX %s::new() */\n", ci->classname);
        fprintf (fout, "  return self;\n");
        fprintf (fout, "}\n");
    }

    /* public interface */
    if (ci->n_methods) {
        for (i = 0; i < ci->n_methods; i++) {
            fprintf (fout, "\n");
            if (strcmp (ci->methods[i].arg, "void") == 0)
                fprintf (fout, "%s\n%s_%s (%s *self)\n", ci->methods[i].type, ci->snakename, ci->methods[i].name, ci->classname);
            else
                fprintf (fout, "%s\n%s_%s (%s *self, %s)\n", ci->methods[i].type, ci->snakename, ci->methods[i].name, ci->classname, ci->methods[i].arg);
            fprintf (fout, "{\n");
            if (ci->haspriv)
                fprintf (fout, "  %sPrivate *priv;\n\n", ci->classname);
            if (strcmp (ci->methods[i].type, "void") != 0)
                fprintf (fout, "  g_return_val_if_fail (%s_IS_%s (self), (%s) 0);\n", ci->m_module, ci->m_type, ci->methods[i].type);
            else
                fprintf (fout, "  g_return_if_fail (%s_IS_%s (self));\n", ci->m_module, ci->m_type);
            if (ci->haspriv)
                fprintf (fout, "  priv = %s_get_instance_private (self);\n  (void)priv;\n", ci->snakename);
            fprintf (fout, "  /* XXX %s::%s() */\n", ci->classname, ci->methods[i].name);
            if (strcmp (ci->methods[i].type, "void") != 0)
                fprintf (fout, "  return (%s) 0;\n", ci->methods[i].type);
            fprintf (fout, "}\n");
        }
    }

    /* public interface to virtuals */
    if (ci->n_virtuals) {
        for (i = 0; i < ci->n_virtuals; i++) {
            fprintf (fout, "\n");
            if (strcmp (ci->virtuals[i].arg, "void") == 0)
                fprintf (fout, "%s\n%s_%s (%s *self)\n", ci->virtuals[i].type, ci->snakename, ci->virtuals[i].name, ci->classname);
            else
                fprintf (fout, "%s\n%s_%s (%s *self, %s)\n", ci->virtuals[i].type, ci->snakename, ci->virtuals[i].name, ci->classname, ci->virtuals[i].arg);
            fprintf (fout, "{\n");
            fprintf (fout, "  %sClass *class;\n", ci->classname);
            if (strcmp (ci->virtuals[i].type, "void") != 0) {
                fprintf (fout, "  %s res;\n\n", ci->virtuals[i].type);
                fprintf (fout, "  g_return_val_if_fail (%s_IS_%s (self), (%s) 0);\n", ci->m_module, ci->m_type, ci->virtuals[i].type);
                fprintf (fout, "  class = %s_GET_CLASS (self);\n", ci->macroname);
                fprintf (fout, "  g_return_val_if_fail (class->%s != NULL, (%s) 0);\n", ci->virtuals[i].name, ci->virtuals[i].type);
                if (ci->virtuals[i].argnames) {
                    fprintf (fout, "  res = class->%s (self, %s);\n", ci->virtuals[i].name, ci->virtuals[i].argnames);
                } else {
                    fprintf (fout, "  res = class->%s (self);\n", ci->virtuals[i].name);
                }
                fprintf (fout, "  /* XXX virtual %s::%s();*/;\n", ci->classname, ci->virtuals[i].name);
                fprintf (fout, "  return res;\n");
            } else {
                fprintf (fout, "\n");
                fprintf (fout, "  g_return_if_fail (%s_IS_%s (self));\n", ci->m_module, ci->m_type);
                fprintf (fout, "  class = %s_GET_CLASS (self);\n", ci->macroname);
                fprintf (fout, "  g_return_if_fail (class->%s != NULL);\n", ci->virtuals[i].name);
                if (ci->virtuals[i].argnames) {
                    fprintf (fout, "  class->%s (self, %s);\n", ci->virtuals[i].name, ci->virtuals[i].argnames);
                } else {
                    fprintf (fout, "  class->%s (self);\n", ci->virtuals[i].name);
                }
                fprintf (fout, "  /* XXX virtual %s::%s();*/;\n", ci->classname, ci->virtuals[i].name);
            }
            fprintf (fout, "}\n");
        }
    }
}

static int init_ifaceinfo (iface_info_t *ii, char *ifacename) {
    if (!ii || !ifacename) return 0;
    if (!is_camel_case (ifacename)) return 0;
    if ((ii->ifacename = to_camel_case (ifacename)) == NULL) return 0;
    ii->parentname = NULL;
    if ((ii->snakename = to_snake_case (ifacename)) == NULL) return 0;
    if ((ii->macroname = to_macro_case (ifacename)) == NULL) return 0;
    if ((ii->macrotype = to_macro_type (ifacename)) == NULL) return 0;
    if ((ii->m_module = part_modulename (ii->macroname)) == NULL) return 0;
    if ((ii->m_type = part_typename (ii->macroname)) == NULL) return 0;
    return 1;
}

static void clear_ifaceinfo (iface_info_t *ii) {
    int i;
    if (!ii) return;
    if (ii->ifacename) free (ii->ifacename);
    if (ii->parentname) free (ii->parentname);
    if (ii->snakename) free (ii->snakename);
    if (ii->macroname) free (ii->macroname);
    if (ii->macrotype) free (ii->macrotype);
    if (ii->m_module) free (ii->m_module);
    if (ii->m_type) free (ii->m_type);
    if (ii->headers != NULL) {
        for (i = 0; i < ii->n_headers; i++)
            if (ii->headers[i] != NULL) free (ii->headers[i]);
        free (ii->headers);
    }
    if (ii->prereqs != NULL) {
        for (i = 0; i < ii->n_prereqs; i++)
            if (ii->prereqs[i] != NULL) free (ii->prereqs[i]);
        free (ii->prereqs);
    }
    if (ii->props != NULL) {
        for (i = 0; i < ii->n_props; i++)
            if (ii->props[i] != NULL) free (ii->props[i]);
        free (ii->props);
    }
    if (ii->virtuals != NULL) {
        for (i = 0; i < ii->n_virtuals; i++) {
            if (ii->virtuals[i].name != NULL) free (ii->virtuals[i].name);
            if (ii->virtuals[i].type != NULL) free (ii->virtuals[i].type);
            if (ii->virtuals[i].arg != NULL) free (ii->virtuals[i].arg);
            if (ii->virtuals[i].argnames != NULL) free (ii->virtuals[i].argnames);
        }
        free (ii->virtuals);
    }
}

static int init_classinfo (class_info_t *ci, char *classname) {
    if (!ci || !classname) return 0;
    if (!is_camel_case (classname)) return 0;
    if ((ci->classname = to_camel_case (classname)) == NULL) return 0;
    ci->parentname = NULL;
    if ((ci->snakename = to_snake_case (classname)) == NULL) return 0;
    if ((ci->macroname = to_macro_case (classname)) == NULL) return 0;
    if ((ci->macrotype = to_macro_type (classname)) == NULL) return 0;
    if ((ci->m_module = part_modulename (ci->macroname)) == NULL) return 0;
    if ((ci->m_type = part_typename (ci->macroname)) == NULL) return 0;
    return 1;
}

static void clear_classinfo (class_info_t *ci) {
    int i;
    if (!ci) return;
    if (ci->classname) free (ci->classname);
    if (ci->parentname) free (ci->parentname);
    if (ci->snakename) free (ci->snakename);
    if (ci->macroname) free (ci->macroname);
    if (ci->macrotype) free (ci->macrotype);
    if (ci->m_module) free (ci->m_module);
    if (ci->m_type) free (ci->m_type);
    if (ci->headers != NULL) {
        for (i = 0; i < ci->n_headers; i++)
            if (ci->headers[i] != NULL) free (ci->headers[i]);
        free (ci->headers);
    }
    if (ci->ifaces != NULL) {
        for (i = 0; i < ci->n_ifaces; i++)
            if (ci->ifaces[i] != NULL) free (ci->ifaces[i]);
        free (ci->ifaces);
    }
    if (ci->props != NULL) {
        for (i = 0; i < ci->n_props; i++)
            if (ci->props[i] != NULL) free (ci->props[i]);
        free (ci->props);
    }
    if (ci->methods != NULL) {
        for (i = 0; i < ci->n_methods; i++) {
            if (ci->methods[i].name != NULL) free (ci->methods[i].name);
            if (ci->methods[i].type != NULL) free (ci->methods[i].type);
            if (ci->methods[i].arg != NULL) free (ci->methods[i].arg);
            if (ci->methods[i].argnames != NULL) free (ci->methods[i].argnames);
        }
        free (ci->methods);
    }
    if (ci->virtuals != NULL) {
        for (i = 0; i < ci->n_virtuals; i++) {
            if (ci->virtuals[i].name != NULL) free (ci->virtuals[i].name);
            if (ci->virtuals[i].type != NULL) free (ci->virtuals[i].type);
            if (ci->virtuals[i].arg != NULL) free (ci->virtuals[i].arg);
            if (ci->virtuals[i].argnames != NULL) free (ci->virtuals[i].argnames);
        }
        free (ci->virtuals);
    }
}

static char * args_to_argnames (const char * args) {
    char *tmp = (char *)args, *tok;
    char *res;

    if (!args) return NULL;
    if (!*args) return NULL;
    if (strcmp (args, "void") == 0) return NULL;
    tmp = strdup (args);
    res = calloc (1, strlen (args) + 1);
    tok = strtok (tmp, ",");
    while (tok) {
        char * lastspace = strrchr (tok, ' ');
        char * altspace = strrchr (tok, '*');
        char * pname = NULL;
        if (!lastspace || (lastspace < altspace))
            pname = altspace + 1;
        else
            pname = lastspace + 1;
        if (!pname) pname = "__parse_failed_param";
        if (!*res) {
            strcat (res, pname);
        } else {
            strcat (res, ", ");
            strcat (res, pname);
        }
        tok = strtok (NULL, ",");
    }
    free ((void *)tmp);
    if (!*res) { free ((void *) res); res = NULL; }
    return res;
}
