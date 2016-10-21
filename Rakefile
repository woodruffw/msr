$:.unshift "#{File.dirname(__FILE__)}/lib"
$:.unshift "#{File.dirname(__FILE__)}/test"
require "rake/testtask"

Rake::TestTask.new do |t|
	t.libs << 'test'
end

desc "Run tests"
task :default => :test

