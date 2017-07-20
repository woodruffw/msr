# frozen_string_literal: true

module MSR
  # Represents a connection to an MSR206 (or MSR206-compatible device).
  class MSR206
    # Whether or not the device is currently linked.
    # Corresponds to {#comm_test!} being passed.
    # @return [Boolean] `true` if linked, `false` otherwise
    def linked?
      comm_test! == :pass
    end
  end
end
