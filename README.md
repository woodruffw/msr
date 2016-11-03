msr
===

[![Gem Version](https://badge.fury.io/rb/msr.svg)](https://badge.fury.io/rb/msr)

A ruby gem for controlling magnetic stripe reader-writers (on UNIX/UNIX-likes).

Uses [libmsr](https://github.com/woodruffw/libmsr).

## Installation

```
$ gem install msr # make sure you have libmsr and ruby development headers
```

## Example

Full documentation is
[available on RubyDoc](http://www.rubydoc.info/gems/msr/).

```ruby
require 'msr'

msr = MSR::MSR505C.new("/dev/ttyS0") # or your serial device

puts "OK" if msr.linked?

# control an LED on the reader
msr.led = :all # :green, :yellow, :red, :all, :none

# get and set coercivity
msr.coercivity # => :high, :low
msr.coercivity = :high # :low

tracks = msr.raw_read # => MSR::Tracks

tracks.reverse!

msr.raw_write tracks
```

## License

`ruby-msr` is licensed under the MIT License.

For the exact terms, see the [license](./LICENSE) file.
