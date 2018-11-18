require "test_helper"

class SPIRV::CrossTest < Minitest::Test
  def test_that_it_has_a_version_number
    refute_nil ::SPIRV::Cross::VERSION
  end

  def read_spirv(fn)
    File.open(File.expand_path('shaders/%s.spv' % fn, __dir__), 'rb').read
  end

  def test_shader_resources
    vert = SPIRV::Cross::Compiler.new(read_spirv('vert'))
    assert_kind_of Hash, vert.shader_resources
    assert_kind_of Hash, vert.active_shader_resources

    assert_equal 3, vert.active_shader_resources[:uniform_buffers][0][:members].size
    assert_equal 3, vert.active_shader_resources[:uniform_buffers][0][:active_buffer_ranges].size
    assert_equal 'UniformBufferObject', vert.active_shader_resources[:uniform_buffers][0][:name]

    assert_equal 3, vert.active_shader_resources[:stage_inputs].size
    assert_equal 2, vert.active_shader_resources[:stage_outputs].size

    frag = SPIRV::Cross::Compiler.new(read_spirv('frag'))
    assert_equal 2, frag.active_shader_resources[:stage_inputs].size
    assert_equal 1, frag.active_shader_resources[:stage_outputs].size
    assert_equal 1, frag.active_shader_resources[:sampled_images].size
  end

  def test_specialization_constants
    compiler = SPIRV::Cross::Compiler.new(read_spirv('vert'))
    assert_equal [{constant_id: 20, name: 'Const'}],
                 compiler.specialization_constants
  end

  def test_entry_points
    compiler = SPIRV::Cross::Compiler.new(read_spirv('vert'))
    assert_equal [{name: "main", execution_model: :vertex}], compiler.entry_points
  end
end
