# frozen_string_literal: true

module MSR
  # Represents a single track from a magnetic stripe card.
  class Track
    # Whether or not the track contains any data.
    # @return [Boolean] `true` if empty, `false` otherwise
    def empty?
      data.empty?
    end

    # Reverse the direction of the track, in place.
    # @return [MSR::Tracks] the current instance
    def reverse!
      @data = reverse.data

      self # return ourself, just for convenience
    end

    # Return a string representation of the track's data.
    # @note May or may not be human readable.
    # @return [String] a string representation of track data
    def to_s
      data.map(&:chr).join
    end

    # Compare two tracks for equality. Two tracks are said to be equal if they
    # contain the same data, in the same order.
    # @param other [Track] the track to compare to
    # @return [Boolean] `true` if the two are equal, `false` otherwise
    def ==(other)
      return unless other.is_a?(self.class)
      data == other.data
    end
  end
end
