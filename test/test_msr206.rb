require "minitest/autorun"
require "msr"

class MSRMSR206Test < Minitest::Test
  # better hope this is your device...
  MSR_SERIAL_DEVICE = "/dev/ttyUSB0"

  def test_msr206_exists
    assert MSR.const_defined?(:MSR206)
  end

  def test_msr206_instantiate
    assert_raises RuntimeError do
      MSR::MSR206.new("/nodevicehere")
    end

    msr = MSR::MSR206.new(MSR_SERIAL_DEVICE)

    # we expect an instance of MSR::MSR206
    assert msr
    assert_instance_of MSR::MSR206, msr
  end

  def test_msr206_checks
    msr = MSR::MSR206.new(MSR_SERIAL_DEVICE)

    # the MSR should pass communications, sensor, and RAM checks
    assert_equal :pass, msr.comm_test!
    assert_equal :pass, msr.sensor_test!
    assert_equal :pass, msr.ram_test!
  end

  def test_msr206_firmware
    msr = MSR::MSR206.new(MSR_SERIAL_DEVICE)
    exp = /REV\w\d\.\d\d/

    firmware = msr.firmware

    # the firmware should be a human-readable string with the expected format
    assert_instance_of String, firmware
    assert_match exp, firmware
  end

  def test_msr206_model
    msr = MSR::MSR206.new(MSR_SERIAL_DEVICE)
    exp = /MSR-206-\d/

    model = msr.model

    # the model should be a human-readable string with the expected format
    assert_instance_of String, model
    assert_match exp, model
  end

  def test_msr206_coercivity
    msr = MSR::MSR206.new(MSR_SERIAL_DEVICE)

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
    msr = MSR::MSR206.new(MSR_SERIAL_DEVICE)

    # these should succeed, and are the only valid BPI values
    msr.bpi = 75
    msr.bpi = 210

    # MSR206#bpi= only likes Fixnums
    assert_raises TypeError do
      msr.bpi = "what would this even do"
    end

    # only 75 and 210 are valid BPIs
    assert_raises RuntimeError do
      msr.bpi = 74
    end

    assert_raises RuntimeError do
      msr.bpi = -1
    end
  end

  def test_msr206_bpc
    msr = MSR::MSR206.new(MSR_SERIAL_DEVICE)

    # these should succeed
    msr.bpc = [5, 5, 5]
    msr.bpc = [6, 6, 6]
    msr.bpc = [7, 7, 7]
    msr.bpc = [8, 8, 8]

    # MSR206#bpc= only likes Arrays of 3 Fixnums
    assert_raises TypeError do
      msr.bpc = 10
    end

    assert_raises TypeError do
      msr.bpc = [1, 2, 'not a number']
    end

    assert_raises ArgumentError do
      msr.bpc = [1, 2, 3, 4]
    end

    # only 5, 6, 7, and 8 are valid BPC values for each track
    assert_raises RuntimeError do
      msr.bpc = [9, 9, 9]
    end
  end

  def test_msr206_led
    msr = MSR::MSR206.new(MSR_SERIAL_DEVICE)

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
    msr = MSR::MSR206.new(MSR_SERIAL_DEVICE)

    tracks = msr.raw_read

    # make sure we get an instance of the correct class back, but
    # there's not much else we can test without a constant card
    assert tracks
    assert_instance_of MSR::Tracks, tracks
  end

  def test_msr206_iso_read
    msr = MSR::MSR206.new(MSR_SERIAL_DEVICE)

    tracks = msr.raw_read

    # make sure we get an instance of the correct class back, but
    # there's not much else we can test without a constant card
    assert tracks
    assert_instance_of MSR::Tracks, tracks
  end

  def test_msr206_raw_write
    skip # TODO
  end

  def test_msr206_iso_write
    skip # TODO
  end
end

