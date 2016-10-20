module MSR
  class MSR206
    def linked?
      comm_test! == :pass
    end
  end
end
