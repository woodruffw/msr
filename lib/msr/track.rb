module MSR
  class Track
    def ==(o)
      return unless o.is_a?(self.class)
      data == o.data
    end
  end
end
