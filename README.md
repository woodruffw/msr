msr
===

A ruby gem for controlling magnetic stripe reader-writers (on UNIX/UNIX-likes).

Uses [libmsr](https://github.com/woodruffw/libmsr).

## Roadmap

```ruby
require 'msr'

msr = MSR::MSR505C.new("/dev/ttyS0") # or your serial device

puts "OK" if msr.linked?

# control an LED on the reader
msr.led!(:red) # :green, :yellow, :all, :off

# get and set coercivity
msr.coercivity # => :high, :low
msr.coercivity = :high # :low

# more to come
```

