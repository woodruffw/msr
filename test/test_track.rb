require "minitest/autorun"
require "msr"

class MSRTrackTest < Minitest::Test
  def test_track_exists
    assert MSR.const_defined?(:Track)
  end

  def test_track_instantiate
    track = MSR::Track.new([1, 2, 3, 4])

    # we expect an instance of MSR::Track
    assert track
    assert_instance_of MSR::Track, track

    # MSR::Track.new only accepts Array
    assert_raises TypeError do
      MSR::Track.new(5)
    end

    # MSR::Track.new only accepts Arrays of Fixnums
    assert_raises TypeError do
      MSR::Track.new(["this is a bad element"])
    end

    # input data is limited to < MAX_TRACK_LEN elements
    assert_raises ArgumentError do
      MSR::Track.new([*1..MSR::Track::MAX_TRACK_LEN + 1])
    end
  end

  def test_track_data_and_length
    track = MSR::Track.new([1, 2, 3, 4])

    # track data is stored as an Array
    assert_instance_of Array, track.data
    assert_equal [1, 2, 3, 4], track.data

    # track length is stored as a Fixnum
    assert_instance_of Fixnum, track.length
    assert_equal 4, track.length
  end

  def test_track_equality
    track1 = MSR::Track.new([1, 2, 3, 4])
    track2 = MSR::Track.new([5, 6, 7, 8])
    track3 = MSR::Track.new([5, 6, 7, 8])

    # two tracks are equal if their data arrays are equal
    refute_equal track1, track2
    assert_equal track2, track3
  end
end
