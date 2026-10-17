#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "nameutils.h"

#define T_REPORT_IS( __exp, __fn, __nm) printf("%s %s(\"%s\") = %d\n", (__fn(__nm) == __exp) ? "ok" : "!!", #__fn, __nm, __fn(__nm))
#define T_REPORT_S( __exp, __fn, __nm) do { char *__s1, *__s2; printf("%s %s(\"%s\") = \"%s\"\n", (strcmp(__s1 = __fn(__nm), __exp) == 0) ? "ok" : "!!", #__fn, __nm, __s2 = __fn(__nm)); free(__s1); free(__s2); } while(0)

int main(void) {
    T_REPORT_IS(1, is_macro_case, "TUT_OBJ01");
    T_REPORT_IS(0, is_macro_case, "TUT_OBJ01_");
    T_REPORT_IS(0, is_macro_case, "TUTOBJ01");
    T_REPORT_IS(0, is_macro_case, "TUT_Obj01");
    T_REPORT_IS(1, is_macro_case, "T_O");
    T_REPORT_IS(1, is_macro_case, "TUT_TYPE_OBJ");
    T_REPORT_IS(1, is_macro_case, "TUT_TYPE_OBJ_123");
    T_REPORT_IS(1, is_macro_case, "TUT_05_OBJ");
    T_REPORT_IS(0, is_macro_case, "5TUT_OBJ");
    T_REPORT_IS(1, is_macro_case, "TUT5_OBJ");
    T_REPORT_IS(0, is_macro_case, "_TUT_TYPE_OBJ");

    T_REPORT_IS(1, is_snake_case, "tut_obj01");
    T_REPORT_IS(0, is_snake_case, "tut_obj01_");
    T_REPORT_IS(0, is_snake_case, "tutobj01");
    T_REPORT_IS(0, is_snake_case, "tut_Obj01");
    T_REPORT_IS(1, is_snake_case, "t_o");
    T_REPORT_IS(1, is_snake_case, "tut_type_obj");
    T_REPORT_IS(1, is_snake_case, "tut_type_obj_123");
    T_REPORT_IS(1, is_snake_case, "tut_05_obj");
    T_REPORT_IS(0, is_snake_case, "5tut_obj");
    T_REPORT_IS(1, is_snake_case, "tut5_obj");
    T_REPORT_IS(0, is_snake_case, "_tut_type_obj");

    T_REPORT_IS(0, is_macro_type, "TUT_OBJ01");
    T_REPORT_IS(0, is_macro_type, "TUT_OBJ01_");
    T_REPORT_IS(0, is_macro_type, "TUTOBJ01");
    T_REPORT_IS(0, is_macro_type, "TUT_Obj01");
    T_REPORT_IS(0, is_macro_type, "T_O");
    T_REPORT_IS(1, is_macro_type, "TUT_TYPE_OBJ");
    T_REPORT_IS(1, is_macro_type, "TUT_TYPE_OBJ_123");
    T_REPORT_IS(0, is_macro_type, "TUT_05_OBJ");
    T_REPORT_IS(0, is_macro_type, "5TUT_OBJ");
    T_REPORT_IS(0, is_macro_type, "TUT5_OBJ");
    T_REPORT_IS(0, is_macro_type, "_TUT_TYPE_OBJ");

    T_REPORT_IS(1, is_camel_case, "TutObj01");
    T_REPORT_IS(0, is_camel_case, "Tutobj01");
    T_REPORT_IS(1, is_camel_case, "Tut21Obj");
    T_REPORT_IS(1, is_camel_case, "TutO");
    T_REPORT_IS(1, is_camel_case, "TutOOO");
    T_REPORT_IS(0, is_camel_case, "1TutObj01");
    T_REPORT_IS(0, is_camel_case, "Tut_Obj01");
    T_REPORT_IS(1, is_camel_case, "TO");
    T_REPORT_IS(1, is_camel_case, "TOO");
    T_REPORT_IS(1, is_camel_case, "TOOOOOO");

    T_REPORT_IS(1, is_valid_pname, "T");
    T_REPORT_IS(1, is_valid_pname, "t");
    T_REPORT_IS(1, is_valid_pname, "T_t");
    T_REPORT_IS(1, is_valid_pname, "T-t");
    T_REPORT_IS(1, is_valid_pname, "my-prop_name");
    T_REPORT_IS(1, is_valid_pname, "my_prop-name");
    T_REPORT_IS(0, is_valid_pname, "1my_prop-name");
    T_REPORT_IS(0, is_valid_pname, "_myprop-name");
    T_REPORT_IS(0, is_valid_pname, "-my-prop-name");

    T_REPORT_IS(1, is_valid_mname, "T");
    T_REPORT_IS(1, is_valid_mname, "t");
    T_REPORT_IS(1, is_valid_mname, "T_t");
    T_REPORT_IS(0, is_valid_mname, "T-t");
    T_REPORT_IS(0, is_valid_mname, "my-prop_name");
    T_REPORT_IS(0, is_valid_mname, "my_prop-name");
    T_REPORT_IS(0, is_valid_mname, "1my_prop-name");
    T_REPORT_IS(0, is_valid_mname, "_myprop-name");
    T_REPORT_IS(0, is_valid_mname, "-my-prop-name");

    T_REPORT_S("TUT", part_modulename, "TUT_OBJ06");
    T_REPORT_S("OBJ06", part_typename, "TUT_OBJ06");
    T_REPORT_S("g", part_modulename, "g_obj06_alt");
    T_REPORT_S("obj06_iface_alt", part_typename, "m_obj06_iface_alt");

    T_REPORT_S("TUT_OBJ_IMPL", to_macro_case_ct, "TUT_OBJ_IMPL");
    T_REPORT_S("TUT_OBJ_IMPL", to_macro_case_ct, "tut_obj_impl");
    T_REPORT_S("TUT_OBJ_IMPL", to_macro_case_ct, "TUT_TYPE_OBJ_IMPL");
    T_REPORT_S("TUT_OBJ_IMPL", to_macro_case_ct, "TutObjImpl");
    T_REPORT_S("TUT12_OBJ3_IMPL", to_macro_case_ct, "Tut12Obj3Impl");

    T_REPORT_S("TUT_TYPE_OBJ_IMPL", to_macro_type_ct, "TUT_OBJ_IMPL");
    T_REPORT_S("TUT_TYPE_OBJ_IMPL", to_macro_type_ct, "tut_obj_impl");
    T_REPORT_S("TUT_TYPE_OBJ_IMPL", to_macro_type_ct, "TUT_TYPE_OBJ_IMPL");
    T_REPORT_S("TUT_TYPE_OBJ_IMPL", to_macro_type_ct, "TutObjImpl");
    T_REPORT_S("TUT12_TYPE_OBJ3_IMPL", to_macro_type_ct, "Tut12Obj3Impl");
    T_REPORT_S("TUT_TYPE_OBJ_TYPE_IMPL", to_macro_type_ct, "TUT_OBJ_TYPE_IMPL");

    T_REPORT_S("tut_obj_impl", to_snake_case_ct, "TUT_OBJ_IMPL");
    T_REPORT_S("tut_obj_impl", to_snake_case_ct, "tut_obj_impl");
    T_REPORT_S("tut_obj_impl", to_snake_case_ct, "TUT_TYPE_OBJ_IMPL");
    T_REPORT_S("tut_obj_impl", to_snake_case_ct, "TutObjImpl");
    T_REPORT_S("tut12_obj3_impl", to_snake_case_ct, "Tut12Obj3Impl");

    T_REPORT_S("TutObjImpl", to_camel_case, "TUT_OBJ_IMPL");
    T_REPORT_S("TutObjImpl", to_camel_case, "tut_obj_impl");
    T_REPORT_S("TutTypeObjImpl", to_camel_case, "TUT_TYPE_OBJ_IMPL");
    T_REPORT_S("TutObjImpl", to_camel_case, "TutObjImpl");
    T_REPORT_S("Tut12TypeObj3Impl", to_camel_case, "TUT12_TYPE_OBJ3_IMPL");

    T_REPORT_S("TUT_OBJ_IMPL", to_macro_case, "TUT_OBJ_IMPL");
    T_REPORT_S("TUT_OBJ_IMPL", to_macro_case, "tut_obj_impl");
    T_REPORT_S("TUT_TYPE_OBJ_IMPL", to_macro_case, "TUT_TYPE_OBJ_IMPL");
    T_REPORT_S("TUT_OBJ_IMPL", to_macro_case, "TutObjImpl");
    T_REPORT_S("TUT12_OBJ3_IMPL", to_macro_case, "Tut12Obj3Impl");

    T_REPORT_S("TUT_TYPE_OBJ_IMPL", to_macro_type, "TUT_OBJ_IMPL");
    T_REPORT_S("TUT_TYPE_OBJ_IMPL", to_macro_type, "tut_obj_impl");
    T_REPORT_S("TUT_TYPE_TYPE_OBJ_IMPL", to_macro_type, "TUT_TYPE_OBJ_IMPL");
    T_REPORT_S("TUT_TYPE_OBJ_IMPL", to_macro_type, "TutObjImpl");
    T_REPORT_S("TUT12_TYPE_OBJ3_IMPL", to_macro_type, "Tut12Obj3Impl");
    T_REPORT_S("TUT_TYPE_OBJ_TYPE_IMPL", to_macro_type, "TUT_OBJ_TYPE_IMPL");
    T_REPORT_S("TUT_TYPE_INAMED", to_macro_type, "TutINamed");

    T_REPORT_S("tut_obj_impl", to_snake_case, "TUT_OBJ_IMPL");
    T_REPORT_S("tut_obj_impl", to_snake_case, "tut_obj_impl");
    T_REPORT_S("tut_type_obj_impl", to_snake_case, "TUT_TYPE_OBJ_IMPL");
    T_REPORT_S("tut_obj_impl", to_snake_case, "TutObjImpl");
    T_REPORT_S("tut12_obj3_impl", to_snake_case, "Tut12Obj3Impl");

    T_REPORT_S("TutObjImpl", to_camel_case, "TUT_OBJ_IMPL");
    T_REPORT_S("TutObjImpl", to_camel_case, "tut_obj_impl");
    T_REPORT_S("TutTypeObjImpl", to_camel_case, "TUT_TYPE_OBJ_IMPL");
    T_REPORT_S("TutObjImpl", to_camel_case, "TutObjImpl");
    T_REPORT_S("Tut12TypeObj3Impl", to_camel_case, "TUT12_TYPE_OBJ3_IMPL");

    T_REPORT_S("t", pname_to_snake, "T");
    T_REPORT_S("t", pname_to_snake, "t");
    T_REPORT_S("t_t", pname_to_snake, "T_t");
    T_REPORT_S("t_t", pname_to_snake, "T-t");
    T_REPORT_S("my_prop_name", pname_to_snake, "my-prop_name");
    T_REPORT_S("my_prop_name", pname_to_snake, "my_prop-name");

    T_REPORT_S("tutobjimpl", to_basename, "TUT_OBJ_IMPL");
    T_REPORT_S("tutobjimpl", to_basename, "tut_obj_impl");
    T_REPORT_S("tut-objimpl", to_dashname, "TUT_OBJ_IMPL");
    T_REPORT_S("tut-objimpl", to_dashname, "tut_obj_impl");

    return 0;
}
