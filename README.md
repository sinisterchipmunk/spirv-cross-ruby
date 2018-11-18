# SPIRV::Cross

Provides Ruby bindings for [SPIRV-Cross](https://github.com/KhronosGroup/SPIRV-Cross).

* **NOTE:** In addition to reflection,
  [SPIRV-Cross](https://github.com/KhronosGroup/SPIRV-Cross) provides the ability
  to compile a shader to other languages (GLSL, HLSL, C++, etc). However, that
  functionality has not been exposed to Ruby by this gem as of this writing.
  If this functionality is desired, [create an issue](https://github.com/sinisterchipmunk/spirv-cross-ruby/issues)
  so that the need can be tracked. Or better yet, add it yourself and send a pull
  request. (Unit tests, please.)


## Installation

Add this line to your application's Gemfile:

```ruby
gem 'spirv-cross'
```

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install spirv-cross

## CLI

In addition to the Ruby API (see below), this gem bundles an executable called
`spirv-dump`. It takes in a single command line argument (filename) and prints
all information that can be reflected from it by this gem. This is useful for
quickly taking a look at a SPIR-V shader, for example for debugging purposes.

    $ spirv-dump path/to/shader.spv

## API

    compiler = SPIRV::Cross::Compiler(File.open('path/to/shader.spv', 'rb').read)

    pp compiler.specialization_constants
    #=> [{:constant_id=>20, :name=>"Const"}]

    pp compiler.entry_points
    #=> [{:name=>"main", :execution_model=>:vertex}]

    pp compiler.shader_resources
    #=> Hash containing all resources defined by this shader

    pp compiler.active_shader_resources
    #=> Hash containing only resources actually used by this shader

In both cases, the shader resources hash looks like:

    {:uniform_buffers=>
      [{:type=>:struct,
        :width=>0,
        :vecsize=>1,
        :columns=>1,
        :is_pointer=>true,
        :storage_class=>:uniform,
        :members=>
         [{:type=>:float,
           :width=>32,
           :vecsize=>4,
           :columns=>4,
           :is_pointer=>false,
           :storage_class=>:generic,
           :members=>[],
           :size=>64,
           :offset=>0,
           :matrix_stride=>16,
           :name=>"model"},
          {:type=>:float,
           :width=>32,
           :vecsize=>4,
           :columns=>4,
           :is_pointer=>false,
           :storage_class=>:generic,
           :members=>[],
           :size=>64,
           :offset=>64,
           :matrix_stride=>16,
           :name=>"view"},
          {:type=>:float,
           :width=>32,
           :vecsize=>4,
           :columns=>4,
           :is_pointer=>false,
           :storage_class=>:generic,
           :members=>[],
           :size=>64,
           :offset=>128,
           :matrix_stride=>16,
           :name=>"proj"}],
        :active_buffer_ranges=>
         [{:index=>2, :offset=>128, :size=>64},
          {:index=>1, :offset=>64, :size=>64},
          {:index=>0, :offset=>0, :size=>64}],
        :array_sizes=>[],
        :name=>"UniformBufferObject",
        :descriptor_set=>0,
        :binding=>0,
        :block_size=>192}],
     :storage_buffers=>[],
     :stage_inputs=>
      [{:type=>:float,
        :width=>32,
        :vecsize=>3,
        :columns=>1,
        :is_pointer=>true,
        :storage_class=>:input,
        :members=>[],
        :active_buffer_ranges=>[],
        :array_sizes=>[],
        :name=>"inPosition",
        :location=>0},
       {:type=>:float,
        :width=>32,
        :vecsize=>3,
        :columns=>1,
        :is_pointer=>true,
        :storage_class=>:input,
        :members=>[],
        :active_buffer_ranges=>[],
        :array_sizes=>[],
        :name=>"inColor",
        :location=>1},
       {:type=>:float,
        :width=>32,
        :vecsize=>2,
        :columns=>1,
        :is_pointer=>true,
        :storage_class=>:input,
        :members=>[],
        :active_buffer_ranges=>[],
        :array_sizes=>[],
        :name=>"inTexCoord",
        :location=>2}],
     :stage_outputs=>
      [{:type=>:float,
        :width=>32,
        :vecsize=>3,
        :columns=>1,
        :is_pointer=>true,
        :storage_class=>:output,
        :members=>[],
        :active_buffer_ranges=>[],
        :array_sizes=>[],
        :name=>"fragColor",
        :location=>0},
       {:type=>:float,
        :width=>32,
        :vecsize=>2,
        :columns=>1,
        :is_pointer=>true,
        :storage_class=>:output,
        :members=>[],
        :active_buffer_ranges=>[],
        :array_sizes=>[],
        :name=>"fragTexCoord",
        :location=>1}],
     :subpass_inputs=>[],
     :storage_images=>[],
     :sampled_images=>[],
     :atomic_counters=>[],
     :push_constant_buffers=>[],
     :separate_images=>[],
     :separate_samplers=>[]}

## Development

After checking out the repo, run `bin/setup` to install dependencies. Then, run `rake test` to run the tests. You can also run `bin/console` for an interactive prompt that will allow you to experiment.

To install this gem onto your local machine, run `bundle exec rake install`. To release a new version, update the version number in `version.rb`, and then run `bundle exec rake release`, which will create a git tag for the version, push git commits and tags, and push the `.gem` file to [rubygems.org](https://rubygems.org).

## Contributing

Bug reports and pull requests are welcome on GitHub at https://github.com/sinisterchipmunk/spirv-cross-ruby.

## License

The gem is available as open source under the terms of the [MIT License](https://opensource.org/licenses/MIT).
