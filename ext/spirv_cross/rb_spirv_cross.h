#ifndef RB_SPIRV_CROSS_H
#define RB_SPIRV_CROSS_H 1

#include "ruby.h"

#if __cplusplus
extern "C" {
#endif

extern VALUE rb_eSPIRVError;
extern VALUE rb_mSPIRV;
extern VALUE rb_mCross;
extern VALUE rb_cCompiler;

VALUE rb_spvx_compiler_initialize(VALUE self, VALUE binstr);
VALUE rb_spvx_compiler_all_shader_resources(VALUE self);
VALUE rb_spvx_compiler_active_shader_resources(VALUE self);
VALUE rb_spvx_compiler_entry_points(VALUE self);
VALUE rb_spvx_compiler_specialization_constants(VALUE self);

#if __cplusplus
}
#endif

#endif /* RB_SPIRV_CROSS_H */
