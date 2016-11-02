module MSR
  # Represents (up to) three tracks from a magnetic stripe card.
  class Tracks
    # Reverse the direction of the tracks, in place.
    # @return [MSR::Tracks] the current instance
    def reverse!
      reversed = self.reverse

      @track1 = reversed.track1
      @track2 = reversed.track2
      @track3 = reversed.track3

      self # return ourself, just for convenience
    end

    # Compare two sets of tracks for equality. Two sets are said to be equal
    # if all of their corresponding pairs are equal.
    # @return [Boolean] `true` if the two are equal, `false` otherwise
    def ==(o)
      return unless o.is_a?(self.class)
      track1 == o.track1 && track2 == o.track2 && track3 == o.track3
    end
  end
end
