# frozen_string_literal: true

module MSR
  # Represents (up to) three tracks from a magnetic stripe card.
  class Tracks
    # Whether or not all tracks are empty.
    # @return [Boolean] `true` if all tracks are empty, `false` otherwise
    def empty?
      tracks.all?(&:empty?)
    end

    # Reverse the direction of the tracks, in place.
    # @return [MSR::Tracks] the current instance
    def reverse!
      @tracks = reverse.tracks

      self # return ourself, just for convenience
    end

    # Compare two sets of tracks for equality. Two sets are said to be equal
    # if all of their corresponding pairs are equal.
    # @param other [Tracks] the tracks to compare to
    # @return [Boolean] `true` if the two are equal, `false` otherwise
    def ==(other)
      return unless other.is_a?(self.class)
      track1 == other.track1 && track2 == other.track2 && track3 == other.track3
    end
  end
end
