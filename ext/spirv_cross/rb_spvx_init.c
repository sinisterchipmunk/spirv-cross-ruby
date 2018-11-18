#include "rb_spirv_cross.h"

VALUE rb_eSPIRVError;
VALUE rb_mSPIRV;
VALUE rb_mCross;
VALUE rb_cCompiler;

void Init_spirv_cross() {
  rb_mSPIRV      = rb_define_module("SPIRV");
  rb_mCross      = rb_define_module_under(rb_mSPIRV, "Cross");
  rb_cCompiler   = rb_define_class_under(rb_mCross, "Compiler", rb_cObject);
  rb_eSPIRVError = rb_define_class_under(rb_mCross, "Error",    rb_eStandardError);

  rb_define_method(rb_cCompiler, "initialize",               rb_spvx_compiler_initialize,               1);
  rb_define_method(rb_cCompiler, "active_shader_resources",  rb_spvx_compiler_active_shader_resources,  0);
  rb_define_method(rb_cCompiler, "all_shader_resources",     rb_spvx_compiler_all_shader_resources,     0);
  rb_define_method(rb_cCompiler, "entry_points",             rb_spvx_compiler_entry_points,             0);
  rb_define_method(rb_cCompiler, "specialization_constants", rb_spvx_compiler_specialization_constants, 0);

  rb_define_alias(rb_cCompiler, "shader_resources", "all_shader_resources");
}
