require "minitest/autorun"
require "msr"

class MSRMSR206Test < Minitest::Test
  def test_msr206_exists
    assert MSR.const_defined?(:MSR206)
  end

  def test_msr206_instantiate
    assert_raises RuntimeError do
      MSR::MSR206.new("/nodevicehere")
    end

    # better hope this is your device...
    msr = MSR::MSR206.new("/dev/ttyUSB0")

    # we expect an instance of MSR::MSR206
    assert msr
    assert_instance_of MSR::MSR206, msr
  end

  def test_msr206_checks
    msr = MSR::MSR206.new("/dev/ttyUSB0")

    # the MSR should pass communications, sensor, and RAM checks
    assert_equal :pass, msr.comm_test!
    assert_equal :pass, msr.sensor_test!
    assert_equal :pass, msr.ram_test!
  end

  def test_msr206_coercivity
    msr = MSR::MSR206.new("/dev/ttyUSB0")

    # changing the coercivity to something other than :hi/:lo should fail
    assert_raises ArgumentError do
      msr.coercivity = :bad
    end

    # the updated coercivity level should be immediately retrievable
    [:hi, :lo].each do |co|
      msr.coercivity = co
      assert_equal co, msr.coercivity
    end
  end

  def test_msr206_bpi
    skip # TODO
  end

  def test_msr206_bpc
    skip # TODO
  end

  def test_msr206_led
    msr = MSR::MSR206.new("/dev/ttyUSB0")

    # setting the LED to something unknown should fail
    assert_raises ArgumentError do
      msr.led = :bad
    end

    # these should all succeed, confirm by looking at device lights
    [:all, :none, :green, :yellow, :red].each do |color|
      msr.led = color
      sleep 0.5
    end

    msr.reset!
  end

  def test_msr206_raw_read
    skip # TODO
  end

  def test_msr206_iso_read
    skip # TODO
  end

  def test_msr206_raw_write
    skip # TODO
  end

  def test_msr206_iso_write
    skip # TODO
  end
end

