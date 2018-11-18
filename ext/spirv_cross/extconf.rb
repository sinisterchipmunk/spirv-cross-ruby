require "mkmf"
require 'rake'
require 'rake/file_utils'

include FileUtils

REVISION = 'ea6bdacd056b77ec1004822482d08b6f31473f25'

unless File.directory?(File.expand_path('spirv-cross', __dir__))
  sh 'git', 'clone', 'https://github.com/KhronosGroup/SPIRV-Cross', File.expand_path('spirv-cross', __dir__)
end

chdir File.expand_path('spirv-cross', __dir__) do
  sh 'git', 'fetch'
  sh 'git', 'checkout', REVISION
  sh 'make'
end

$CFLAGS << ' -I' << File.expand_path('spirv-cross', __dir__)
$CXXFLAGS << ' -I' << File.expand_path('spirv-cross', __dir__) << ' -std=c++11'
$LDFLAGS << ' -L' << File.expand_path('spirv-cross', __dir__)
have_library 'spirv-cross'

create_makefile("spirv/spirv_cross")
