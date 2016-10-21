module MSR
  # Represents a single track from a magnetic stripe card.
  class Track
    # Compare two tracks for equality. Two tracks are said to be equal if they
    # contain the same data, in the same order.
    # @return [Boolean] `true` if the two are equal, `false` otherwise
    def ==(o)
      return unless o.is_a?(self.class)
      data == o.data
    end
  end
end
