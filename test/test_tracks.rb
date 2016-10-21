require "minitest/autorun"
require "msr"

class MSRTracksTest < Minitest::Test
  def test_tracks_exists
    assert MSR.const_defined?(:Tracks)
  end

  def test_tracks_instantiate
    t1 = MSR::Track.new([1, 2, 3])
    t2 = MSR::Track.new([4, 5, 6])
    t3 = MSR::Track.new([7, 8, 9])
    tracks = MSR::Tracks.new(t1, t2, t3)

    # we expect an instance of MSR::Tracks
    assert tracks
    assert_instance_of MSR::Tracks, tracks

    # MSR::Tracks.new accepts exactly 3 args, all MSR::Track
    assert_raises ArgumentError do
      MSR::Tracks.new(1, t2, t3)
    end

    assert_raises ArgumentError do
      MSR::Tracks.new(t1, 2, t3)
    end

    assert_raises ArgumentError do
      MSR::Tracks.new(t1, t2, 3)
    end
  end

  def test_tracks_individual
    t1 = MSR::Track.new([1, 2, 3])
    t2 = MSR::Track.new([4, 5, 6])
    t3 = MSR::Track.new([7, 8, 9])
    tracks = MSR::Tracks.new(t1, t2, t3)

    # we expect three instances of MSR::Track, one for each track
    assert tracks.track1
    assert_instance_of MSR::Track, tracks.track1

    assert tracks.track2
    assert_instance_of MSR::Track, tracks.track2

    assert tracks.track3
    assert_instance_of MSR::Track, tracks.track3
  end

  def test_tracks_equality
    t1 = MSR::Track.new([1, 1, 1])
    t2 = MSR::Track.new([1, 1, 1])
    t3 = MSR::Track.new([1, 1, 1])
    tracks1 = MSR::Tracks.new(t1, t2, t3)

    t4 = MSR::Track.new([1, 2, 3])
    t5 = MSR::Track.new([4, 5, 6])
    t6 = MSR::Track.new([7, 8, 9])
    tracks2 = MSR::Tracks.new(t4, t5, t6)

    t7 = MSR::Track.new([1, 2, 3])
    t8 = MSR::Track.new([4, 5, 6])
    t9 = MSR::Track.new([7, 8, 9])
    tracks3 = MSR::Tracks.new(t7, t8, t9)

    # two MSR::Tracks are equal if all of their corresponding tracks are equal
    refute_equal tracks1, tracks2
    assert_equal tracks2, tracks3
  end
end
