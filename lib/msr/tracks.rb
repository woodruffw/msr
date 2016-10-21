module MSR
  class Tracks
    def ==(o)
      return unless o.is_a?(self.class)
      track1 == o.track1 && track2 == o.track2 && track3 == o.track3
    end
  end
end
