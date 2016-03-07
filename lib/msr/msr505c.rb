require_relative 'msr206'

# the MSR505C is firmware-compatible with the MSR206, so just alias the class
module MSR
	MSR505C = MSR206
end
