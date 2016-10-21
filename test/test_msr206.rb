require "minitest/autorun"
require "msr"

class MSRMSR206Test < Minitest::Test
  def test_msr206_exists
    assert MSR.const_defined?(:MSR206)
  end

  def test_msr206_instantiate
    # TODO: unstub
    skip
  end
end

