#!/bin/tclsh

set ip4_addr 0.255.254.245
set ip6_addr1 fe80:0000:00e0:0000:61f7:80db:fb59:6687
set ip6_addr2 fe80::61f7:80db:fb59:66c7
set mac_addr 00:1c:aE:66:62:A7
set date_value 2013:01:27
set time_value 22:47:00

proc isipv4 {string} {
	set octet {(?:\d|[1-9]\d|1\d\d|2[0-4]\d|25[0-5])}
	set ipv4_pattern "^[join [list $octet $octet $octet $octet] {\.}]\$"
	puts $ipv4_pattern
	return [regexp -- $ipv4_pattern $string]
}
puts "pm: [isipv4 $ip4_addr]"
if {[regexp {^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}$} $ip4_addr]} {
	puts "ipv4 address ok"
} else {
	puts "ipv4 not ok"
}
set ip6_pattern {[a-f|\d]{4}}
if {[regexp "^($ip6_pattern):($ip6_pattern):($ip6_pattern):($ip6_pattern):($ip6_pattern):($ip6_pattern):($ip6_pattern):($ip6_pattern)$" $ip6_addr1]} {
	puts "ipv6 address ok"
} else {
	puts "ipv6 not ok"
}

