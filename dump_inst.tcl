#!/usr/bin/env tclsh
load ./rvfuntcl.so

if {[llength $argv] != 1} {
	puts "Usage: $argv0 <opcode>"
}
puts [rvfun disasm [lindex $argv 0]]

