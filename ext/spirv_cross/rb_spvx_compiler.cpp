#include "rb_spirv_cross.h"
#include "spirv_cross.hpp"

using namespace spirv_cross;
using namespace std;
using namespace spv;

void compiler_free(void* data) {
  Compiler *comp = (Compiler *) data;
  delete comp;
}

size_t compiler_size(const void* data) {
  return sizeof(Compiler);
}

static const rb_data_type_t compiler_type = {
  .wrap_struct_name = "SPIRVCompiler",
  .function = {
    .dmark = NULL,
    .dfree = compiler_free,
    .dsize = compiler_size
  },
  .parent = NULL,
  .data = NULL,
  .flags = RUBY_TYPED_FREE_IMMEDIATELY
};

extern "C" {
  VALUE rb_spirv_type(Compiler &compiler, uint32_t type_id) {
    VALUE vres = rb_hash_new();
    auto &base_type = compiler.get_type(type_id);
    switch(base_type.basetype) {
      case SPIRType::BaseType::Void:          rb_hash_aset(vres, ID2SYM(rb_intern("type")), ID2SYM(rb_intern("void")));           break;
      case SPIRType::BaseType::Boolean:       rb_hash_aset(vres, ID2SYM(rb_intern("type")), ID2SYM(rb_intern("boolean")));        break;
      case SPIRType::BaseType::Char:          rb_hash_aset(vres, ID2SYM(rb_intern("type")), ID2SYM(rb_intern("char")));           break;
      case SPIRType::BaseType::Int:           rb_hash_aset(vres, ID2SYM(rb_intern("type")), ID2SYM(rb_intern("int")));            break;
      case SPIRType::BaseType::UInt:          rb_hash_aset(vres, ID2SYM(rb_intern("type")), ID2SYM(rb_intern("uint")));           break;
      case SPIRType::BaseType::Int64:         rb_hash_aset(vres, ID2SYM(rb_intern("type")), ID2SYM(rb_intern("int64")));          break;
      case SPIRType::BaseType::UInt64:        rb_hash_aset(vres, ID2SYM(rb_intern("type")), ID2SYM(rb_intern("uint64")));         break;
      case SPIRType::BaseType::AtomicCounter: rb_hash_aset(vres, ID2SYM(rb_intern("type")), ID2SYM(rb_intern("atomic_counter"))); break;
      case SPIRType::BaseType::Half:          rb_hash_aset(vres, ID2SYM(rb_intern("type")), ID2SYM(rb_intern("half")));           break;
      case SPIRType::BaseType::Float:         rb_hash_aset(vres, ID2SYM(rb_intern("type")), ID2SYM(rb_intern("float")));          break;
      case SPIRType::BaseType::Double:        rb_hash_aset(vres, ID2SYM(rb_intern("type")), ID2SYM(rb_intern("double")));         break;
      case SPIRType::BaseType::Struct:        rb_hash_aset(vres, ID2SYM(rb_intern("type")), ID2SYM(rb_intern("struct")));         break;
      case SPIRType::BaseType::Image:         rb_hash_aset(vres, ID2SYM(rb_intern("type")), ID2SYM(rb_intern("image")));          break;
      case SPIRType::BaseType::SampledImage:  rb_hash_aset(vres, ID2SYM(rb_intern("type")), ID2SYM(rb_intern("sampled_image")));  break;
      case SPIRType::BaseType::Sampler:       rb_hash_aset(vres, ID2SYM(rb_intern("type")), ID2SYM(rb_intern("sampler")));        break;
      default:                                rb_hash_aset(vres, ID2SYM(rb_intern("type")), ID2SYM(rb_intern("unknown")));
    }

    rb_hash_aset(vres, ID2SYM(rb_intern("width")),      UINT2NUM(base_type.width));
    rb_hash_aset(vres, ID2SYM(rb_intern("vecsize")),    UINT2NUM(base_type.vecsize));
    rb_hash_aset(vres, ID2SYM(rb_intern("columns")),    UINT2NUM(base_type.columns));
    rb_hash_aset(vres, ID2SYM(rb_intern("is_pointer")), base_type.pointer ? Qtrue : Qfalse);

    if (base_type.basetype == SPIRType::BaseType::Image || base_type.basetype == SPIRType::BaseType::SampledImage) {
      VALUE vimg = rb_hash_new();
      rb_hash_aset(vimg, ID2SYM(rb_intern("type")),       rb_spirv_type(compiler, base_type.image.type));
      switch(base_type.image.dim) {
        case Dim::Dim1D:          rb_hash_aset(vimg, ID2SYM(rb_intern("dimensions")), INT2NUM(1));                        break;
        case Dim::Dim2D:          rb_hash_aset(vimg, ID2SYM(rb_intern("dimensions")), INT2NUM(2));                        break;
        case Dim::Dim3D:          rb_hash_aset(vimg, ID2SYM(rb_intern("dimensions")), INT2NUM(3));                        break;
        case Dim::DimCube:        rb_hash_aset(vimg, ID2SYM(rb_intern("dimensions")), ID2SYM(rb_intern("cube")));         break;
        case Dim::DimRect:        rb_hash_aset(vimg, ID2SYM(rb_intern("dimensions")), ID2SYM(rb_intern("rect")));         break;
        case Dim::DimBuffer:      rb_hash_aset(vimg, ID2SYM(rb_intern("dimensions")), ID2SYM(rb_intern("buffer")));       break;
        case Dim::DimSubpassData: rb_hash_aset(vimg, ID2SYM(rb_intern("dimensions")), ID2SYM(rb_intern("subpass_data"))); break;
        case Dim::DimMax:         rb_hash_aset(vimg, ID2SYM(rb_intern("dimensions")), ID2SYM(rb_intern("max")));          break;
        default:                  rb_hash_aset(vimg, ID2SYM(rb_intern("dimensions")), ID2SYM(rb_intern("unknown")));
      }
      rb_hash_aset(vimg, ID2SYM(rb_intern("depth")),        base_type.image.depth   ? Qtrue : Qfalse);
      rb_hash_aset(vimg, ID2SYM(rb_intern("arrayed")),      base_type.image.arrayed ? Qtrue : Qfalse);
      rb_hash_aset(vimg, ID2SYM(rb_intern("multisampled")), base_type.image.ms      ? Qtrue : Qfalse);
      switch(base_type.image.sampled) {
        case 1:  rb_hash_aset(vimg, ID2SYM(rb_intern("sampling")), ID2SYM(rb_intern("sampled")));          break;
        case 2:  rb_hash_aset(vimg, ID2SYM(rb_intern("sampling")), ID2SYM(rb_intern("image_load_store"))); break;
        default: rb_hash_aset(vimg, ID2SYM(rb_intern("sampling")), Qfalse);
      }
      switch(base_type.image.format) {
        case ImageFormatRgba32f:      rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("rgba32f")));      break;
        case ImageFormatRgba16f:      rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("rgba16f")));      break;
        case ImageFormatR32f:         rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("r32f")));         break;
        case ImageFormatRgba8:        rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("rgba8")));        break;
        case ImageFormatRgba8Snorm:   rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("rgba8snorm")));   break;
        case ImageFormatRg32f:        rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("rg32f")));        break;
        case ImageFormatRg16f:        rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("rg16f")));        break;
        case ImageFormatR11fG11fB10f: rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("r11fg11fb10f"))); break;
        case ImageFormatR16f:         rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("r16f")));         break;
        case ImageFormatRgba16:       rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("rgba16")));       break;
        case ImageFormatRgb10A2:      rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("rgb10a2")));      break;
        case ImageFormatRg16:         rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("rg16")));         break;
        case ImageFormatRg8:          rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("rg8")));          break;
        case ImageFormatR16:          rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("r16")));          break;
        case ImageFormatR8:           rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("r8")));           break;
        case ImageFormatRgba16Snorm:  rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("rgba16snorm")));  break;
        case ImageFormatRg16Snorm:    rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("rg16snorm")));    break;
        case ImageFormatRg8Snorm:     rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("rg8snorm")));     break;
        case ImageFormatR16Snorm:     rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("r16snorm")));     break;
        case ImageFormatR8Snorm:      rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("r8snorm")));      break;
        case ImageFormatRgba32i:      rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("rgba32i")));      break;
        case ImageFormatRgba16i:      rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("rgba16i")));      break;
        case ImageFormatRgba8i:       rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("rgba8i")));       break;
        case ImageFormatR32i:         rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("r32i")));         break;
        case ImageFormatRg32i:        rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("rg32i")));        break;
        case ImageFormatRg16i:        rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("rg16i")));        break;
        case ImageFormatRg8i:         rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("rg8i")));         break;
        case ImageFormatR16i:         rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("r16i")));         break;
        case ImageFormatR8i:          rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("r8i")));          break;
        case ImageFormatRgba32ui:     rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("rgba32ui")));     break;
        case ImageFormatRgba16ui:     rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("rgba16ui")));     break;
        case ImageFormatRgba8ui:      rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("rgba8ui")));      break;
        case ImageFormatR32ui:        rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("r32ui")));        break;
        case ImageFormatRgb10a2ui:    rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("rgb10a2ui")));    break;
        case ImageFormatRg32ui:       rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("rg32ui")));       break;
        case ImageFormatRg16ui:       rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("rg16ui")));       break;
        case ImageFormatRg8ui:        rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("rg8ui")));        break;
        case ImageFormatR16ui:        rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("r16ui")));        break;
        case ImageFormatR8ui:         rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("r8ui")));         break;
        case ImageFormatMax:          rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("max")));          break;
        default:                      rb_hash_aset(vimg, ID2SYM(rb_intern("format")), ID2SYM(rb_intern("unknown")));
      }
      switch(base_type.image.access) {
        case AccessQualifierReadOnly:  rb_hash_aset(vimg, ID2SYM(rb_intern("access")), ID2SYM(rb_intern("read_only")));  break;
        case AccessQualifierWriteOnly: rb_hash_aset(vimg, ID2SYM(rb_intern("access")), ID2SYM(rb_intern("write_only"))); break;
        case AccessQualifierReadWrite: rb_hash_aset(vimg, ID2SYM(rb_intern("access")), ID2SYM(rb_intern("read_write"))); break;
        case AccessQualifierMax:       rb_hash_aset(vimg, ID2SYM(rb_intern("access")), ID2SYM(rb_intern("max")));        break;
        default:                       rb_hash_aset(vimg, ID2SYM(rb_intern("access")), ID2SYM(rb_intern("unknown")));   break;
      }
      rb_hash_aset(vres, ID2SYM(rb_intern("image")), vimg);
    }

    switch(base_type.storage) {
      case StorageClassGeneric:         rb_hash_aset(vres, ID2SYM(rb_intern("storage_class")), ID2SYM(rb_intern("generic")));           break;
      case StorageClassUniformConstant: rb_hash_aset(vres, ID2SYM(rb_intern("storage_class")), ID2SYM(rb_intern("uniform_constant")));  break;
      case StorageClassInput:           rb_hash_aset(vres, ID2SYM(rb_intern("storage_class")), ID2SYM(rb_intern("input")));             break;
      case StorageClassUniform:         rb_hash_aset(vres, ID2SYM(rb_intern("storage_class")), ID2SYM(rb_intern("uniform")));           break;
      case StorageClassOutput:          rb_hash_aset(vres, ID2SYM(rb_intern("storage_class")), ID2SYM(rb_intern("output")));            break;
      case StorageClassWorkgroup:       rb_hash_aset(vres, ID2SYM(rb_intern("storage_class")), ID2SYM(rb_intern("workgroup")));         break;
      case StorageClassCrossWorkgroup:  rb_hash_aset(vres, ID2SYM(rb_intern("storage_class")), ID2SYM(rb_intern("cross_work_group")));  break;
      case StorageClassPrivate:         rb_hash_aset(vres, ID2SYM(rb_intern("storage_class")), ID2SYM(rb_intern("private")));           break;
      case StorageClassFunction:        rb_hash_aset(vres, ID2SYM(rb_intern("storage_class")), ID2SYM(rb_intern("function")));          break;
      case StorageClassPushConstant:    rb_hash_aset(vres, ID2SYM(rb_intern("storage_class")), ID2SYM(rb_intern("push_constant")));     break;
      case StorageClassAtomicCounter:   rb_hash_aset(vres, ID2SYM(rb_intern("storage_class")), ID2SYM(rb_intern("atomic_counter")));    break;
      case StorageClassImage:           rb_hash_aset(vres, ID2SYM(rb_intern("storage_class")), ID2SYM(rb_intern("image")));             break;
      case StorageClassStorageBuffer:   rb_hash_aset(vres, ID2SYM(rb_intern("storage_class")), ID2SYM(rb_intern("storage_buffer")));    break;
      case StorageClassMax:             rb_hash_aset(vres, ID2SYM(rb_intern("storage_class")), ID2SYM(rb_intern("max")));               break;
      default:                          rb_hash_aset(vres, ID2SYM(rb_intern("storage_class")), ID2SYM(rb_intern("unknown")));
    }

    VALUE vmembers = rb_ary_new();
    rb_hash_aset(vres, ID2SYM(rb_intern("members")), vmembers);
    for (unsigned i = 0; i < base_type.member_types.size(); i++) {
      uint32_t mtype_id = base_type.member_types[i];
      // VALUE vmember = rb_hash_new();
      VALUE vmember = rb_spirv_type(compiler, mtype_id);
      rb_ary_push(vmembers, vmember);
      // rb_hash_aset(vmember, ID2SYM(rb_intern("type")), rb_spirv_type(compiler, mtype_id));
      auto &member_type = compiler.get_type(mtype_id);
      size_t member_size = compiler.get_declared_struct_member_size(base_type, i);
      rb_hash_aset(vmember, ID2SYM(rb_intern("size")), SIZET2NUM(member_size));

      // Get member offset within this struct.
      size_t offset = compiler.type_struct_member_offset(base_type, i);
      rb_hash_aset(vmember, ID2SYM(rb_intern("offset")), SIZET2NUM(offset));

      if (!member_type.array.empty())
      {
          // Get array stride, e.g. float4 foo[]; Will have array stride of 16 bytes.
          size_t array_stride = compiler.type_struct_member_array_stride(base_type, i);
          rb_hash_aset(vmember, ID2SYM(rb_intern("array_stride")), SIZET2NUM(array_stride));
      }

      if (member_type.columns > 1)
      {
          // Get bytes stride between columns (if column major), for float4x4 -> 16 bytes.
          size_t matrix_stride = compiler.type_struct_member_matrix_stride(base_type, i);
          rb_hash_aset(vmember, ID2SYM(rb_intern("matrix_stride")), SIZET2NUM(matrix_stride));
      }
      const string &name = compiler.get_member_name(base_type.self, i);
      rb_hash_aset(vmember, ID2SYM(rb_intern("name")), rb_str_new2(name.c_str()));
    }

    return vres;
  }

  VALUE shader_resource_vector_to_ary(Compiler &compiler, vector<Resource> &vec, bool print_ssbo) {
    VALUE ret = rb_ary_new();
    for (size_t i = 0; i < vec.size(); i++) {
      Resource &res = vec[i];

      // just merge extra data into the type hash instead of creating a nested type hash
      VALUE vres = rb_spirv_type(compiler, res.type_id);

      // VALUE vres = rb_hash_new();
      // Docs say base type is only really useful for decorations, which we do further down, so
      // we'll omit it here to save redundancy.
      // // rb_hash_aset(vres, ID2SYM(rb_intern("base_type")), rb_spirv_type(compiler, res.base_type_id));
      // rb_hash_aset(vres, ID2SYM(rb_intern("type")), rb_spirv_type(compiler, res.type_id));

      vector<BufferRange> ranges = compiler.get_active_buffer_ranges(res.id);
      VALUE vranges = rb_ary_new();
      rb_hash_aset(vres, ID2SYM(rb_intern("active_buffer_ranges")), vranges);
      for (auto &range : ranges) {
        VALUE vrange = rb_hash_new();
        rb_hash_aset(vrange, ID2SYM(rb_intern("index")),  UINT2NUM(range.index));
        rb_hash_aset(vrange, ID2SYM(rb_intern("offset")), SIZET2NUM(range.offset));
        rb_hash_aset(vrange, ID2SYM(rb_intern("size")),   SIZET2NUM(range.range));
        rb_ary_push(vranges, vrange);
      }

      auto &type = compiler.get_type(res.type_id);

      /* The following code was basically lifted from spirv-cross/main.cpp */

      if (print_ssbo && compiler.buffer_is_hlsl_counter_buffer(res.id))
        continue;

      // If we don't have a name, use the fallback for the type instead of the variable
      // for SSBOs and UBOs since those are the only meaningful names to use externally.
      // Push constant blocks are still accessed by name and not block name, even though they are technically Blocks.
      bool is_push_constant = compiler.get_storage_class(res.id) == StorageClassPushConstant;
      bool is_block = compiler.get_decoration_bitset(type.self).get(DecorationBlock) ||
                      compiler.get_decoration_bitset(type.self).get(DecorationBufferBlock);
      bool is_sized_block = is_block && (compiler.get_storage_class(res.id) == StorageClassUniform ||
                                         compiler.get_storage_class(res.id) == StorageClassUniformConstant);
      uint32_t fallback_id = !is_push_constant && is_block ? res.base_type_id : res.id;

      uint32_t block_size = 0;
      uint32_t runtime_array_stride = 0;
      if (is_sized_block)
      {
        auto &base_type = compiler.get_type(res.base_type_id);
        block_size = uint32_t(compiler.get_declared_struct_size(base_type));
        runtime_array_stride = uint32_t(compiler.get_declared_struct_size_runtime_array(base_type, 1) -
                                        compiler.get_declared_struct_size_runtime_array(base_type, 0));
      }

      Bitset mask;
      if (print_ssbo)
        mask = compiler.get_buffer_block_flags(res.id);
      else
        mask = compiler.get_decoration_bitset(res.id);

      VALUE ary = rb_ary_new();
      rb_hash_aset(vres, ID2SYM(rb_intern("array_sizes")), ary);
      for (size_t j = 0; j < type.array.size(); j++) {
        if (type.array_size_literal[j])
          rb_ary_push(ary, UINT2NUM(type.array[j]));
        else
          rb_ary_push(ary, ID2SYM(rb_intern("expression")));
      }

      rb_hash_aset(vres, ID2SYM(rb_intern("name")),
                         rb_str_new2(!res.name.empty() ?
                                     res.name.c_str() :
                                     compiler.get_fallback_name(fallback_id).c_str()));

      if (mask.get(DecorationLocation))
        rb_hash_aset(vres, ID2SYM(rb_intern("location")), UINT2NUM(compiler.get_decoration(res.id, DecorationLocation)));
      if (mask.get(DecorationDescriptorSet))
        rb_hash_aset(vres, ID2SYM(rb_intern("descriptor_set")), UINT2NUM(compiler.get_decoration(res.id, DecorationDescriptorSet)));
      if (mask.get(DecorationBinding))
        rb_hash_aset(vres, ID2SYM(rb_intern("binding")), UINT2NUM(compiler.get_decoration(res.id, DecorationBinding)));
      if (mask.get(DecorationInputAttachmentIndex))
        rb_hash_aset(vres, ID2SYM(rb_intern("attachment")), UINT2NUM(compiler.get_decoration(res.id, DecorationInputAttachmentIndex)));
      if (mask.get(DecorationNonReadable))
        rb_hash_aset(vres, ID2SYM(rb_intern("write_only")), Qtrue);
      if (mask.get(DecorationNonWritable))
        rb_hash_aset(vres, ID2SYM(rb_intern("read_only")), Qtrue);
      if (is_sized_block)
      {
        rb_hash_aset(vres, ID2SYM(rb_intern("block_size")), UINT2NUM(block_size));
        if (runtime_array_stride)
          rb_hash_aset(vres, ID2SYM(rb_intern("unsized_array_stride")), UINT2NUM(runtime_array_stride));
      }

      uint32_t counter_id = 0;
      if (print_ssbo && compiler.buffer_get_hlsl_counter_buffer(res.id, counter_id))
        rb_hash_aset(vres, ID2SYM(rb_intern("hlsl_counter_buffer_id")), UINT2NUM(counter_id));

      rb_ary_push(ret, vres);
    }
    return ret;
  }

  inline VALUE shader_resources_to_hash(Compiler *comp, ShaderResources &resources) {
    VALUE vsr = rb_hash_new();
    #define HASH_PUSH_RESOURCE_ARY(name, ssbo) \
      rb_hash_aset(vsr, ID2SYM(rb_intern(#name)), shader_resource_vector_to_ary(*comp, resources.name, ssbo));
    HASH_PUSH_RESOURCE_ARY(uniform_buffers,       false);
    HASH_PUSH_RESOURCE_ARY(storage_buffers,       true);
    HASH_PUSH_RESOURCE_ARY(stage_inputs,          false);
    HASH_PUSH_RESOURCE_ARY(stage_outputs,         false);
    HASH_PUSH_RESOURCE_ARY(subpass_inputs,        false);
    HASH_PUSH_RESOURCE_ARY(storage_images,        false);
    HASH_PUSH_RESOURCE_ARY(sampled_images,        false);
    HASH_PUSH_RESOURCE_ARY(atomic_counters,       false);
    HASH_PUSH_RESOURCE_ARY(push_constant_buffers, false);
    HASH_PUSH_RESOURCE_ARY(separate_images,       false);
    HASH_PUSH_RESOURCE_ARY(separate_samplers,     false);
    return vsr;
  }

  VALUE rb_spvx_compiler_all_shader_resources(VALUE self) {
    Compiler *comp;
    TypedData_Get_Struct(rb_ivar_get(self, rb_intern("@handle")), Compiler, &compiler_type, comp);
    ShaderResources resources = comp->get_shader_resources();
    return shader_resources_to_hash(comp, resources);
  }

  VALUE rb_spvx_compiler_active_shader_resources(VALUE self) {
    Compiler *comp;
    TypedData_Get_Struct(rb_ivar_get(self, rb_intern("@handle")), Compiler, &compiler_type, comp);
    auto active = comp->get_active_interface_variables();
    ShaderResources resources = comp->get_shader_resources(active);
    // comp->set_enabled_interface_variables(move(active));
    return shader_resources_to_hash(comp, resources);
  }

  VALUE rb_spvx_compiler_entry_points(VALUE self) {
    Compiler *comp;
    TypedData_Get_Struct(rb_ivar_get(self, rb_intern("@handle")), Compiler, &compiler_type, comp);
    std::vector<EntryPoint> entry_points = comp->get_entry_points_and_stages();
    VALUE ret = rb_ary_new();
    for (auto &ep : entry_points) {
      VALUE vep = rb_hash_new();
      rb_ary_push(ret, vep);
      rb_hash_aset(vep, ID2SYM(rb_intern("name")), rb_str_new2(ep.name.c_str()));
      switch(ep.execution_model) {
        case ExecutionModel::ExecutionModelVertex:                 rb_hash_aset(vep, ID2SYM(rb_intern("execution_model")), ID2SYM(rb_intern("vertex")));                  break;
        case ExecutionModel::ExecutionModelTessellationControl:    rb_hash_aset(vep, ID2SYM(rb_intern("execution_model")), ID2SYM(rb_intern("tessellation_control")));    break;
        case ExecutionModel::ExecutionModelTessellationEvaluation: rb_hash_aset(vep, ID2SYM(rb_intern("execution_model")), ID2SYM(rb_intern("tessellation_evaluation"))); break;
        case ExecutionModel::ExecutionModelGeometry:               rb_hash_aset(vep, ID2SYM(rb_intern("execution_model")), ID2SYM(rb_intern("geometry")));                break;
        case ExecutionModel::ExecutionModelFragment:               rb_hash_aset(vep, ID2SYM(rb_intern("execution_model")), ID2SYM(rb_intern("fragment")));                break;
        case ExecutionModel::ExecutionModelGLCompute:              rb_hash_aset(vep, ID2SYM(rb_intern("execution_model")), ID2SYM(rb_intern("gl_compute")));              break;
        case ExecutionModel::ExecutionModelKernel:                 rb_hash_aset(vep, ID2SYM(rb_intern("execution_model")), ID2SYM(rb_intern("kernel")));                  break;
        case ExecutionModel::ExecutionModelMax:                    rb_hash_aset(vep, ID2SYM(rb_intern("execution_model")), ID2SYM(rb_intern("max")));                     break;
        default:                                                   rb_hash_aset(vep, ID2SYM(rb_intern("execution_model")), ID2SYM(rb_intern("unknown")));                     break;
      }
    }
    return ret;
  }

  VALUE rb_spvx_compiler_specialization_constants(VALUE self) {
    Compiler *comp;
    TypedData_Get_Struct(rb_ivar_get(self, rb_intern("@handle")), Compiler, &compiler_type, comp);
    vector<SpecializationConstant> consts = comp->get_specialization_constants();
    VALUE vconsts = rb_ary_new();
    for (auto &c : consts) {
      VALUE vconst = rb_hash_new();
      rb_hash_aset(vconst, ID2SYM(rb_intern("constant_id")), INT2NUM(c.constant_id));
      rb_hash_aset(vconst, ID2SYM(rb_intern("name")),        rb_str_new2(comp->get_name(c.id).c_str()));
      rb_ary_push(vconsts, vconst);
    }
    return vconsts;
  }

  VALUE rb_spvx_compiler_initialize(VALUE self, VALUE binstr) {
    size_t len = RSTRING_LEN(binstr) / sizeof(uint32_t);
    uint32_t *data = (uint32_t *) RSTRING_PTR(binstr);
    vector<uint32_t> spirv;
    for (size_t i = 0; i < len; i++) {
      spirv.push_back(data[i]);
    }

    try {
      Compiler *compiler = new Compiler(move(spirv));
      rb_ivar_set(self, rb_intern("@handle"), TypedData_Wrap_Struct(rb_cData, &compiler_type, compiler));
    } catch(const exception &ex) {
      rb_raise(rb_eSPIRVError, "%s", ex.what());
    }

    return self;
  }
}
