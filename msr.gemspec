require_relative "lib/msr"

Gem::Specification.new do |s|
  s.name = 'msr'
  s.version = MSR::VERSION
  s.summary = 'msr - magnetic stripe reader/writer library.'
  s.description = 'A library for controlling magnetic stripe reader/writers.'
  s.authors = ['William Woodruff']
  s.email = 'william@tuffbizz.com'
  s.files = Dir['LICENSE', 'README.md', '.yardopts', 'lib/**/*', 'ext/**/*']
  s.extensions << 'ext/msr/extconf.rb'
  s.homepage = 'https://github.com/woodruffw/ruby-msr'
  s.license = 'MIT'
end
