# ======================================================================
# Define options
# ======================================================================

set val(chan)       Channel
set val(mac)        Mac/802_3
set val(ifq)        Queue/DropTail
set val(ll)         LL
set val(seed)       1.0
set val(nn)         3 
set val(stop)       300
set val(bw)         10Mb
set val(delay)      0.1ms
set val(ll)         LL

# =====================================================================
# Main Program
# ======================================================================

#
# Initialize Global Variables
#

# create simulator instance

set ns		[new Simulator]

# create trace object for ns and nam

set tracefd	[open ethernet_trace.tr w]
set namtrace    [open ethernet_nam.nam w]

$ns trace-all $tracefd
$ns namtrace-all $namtrace

for {set i 0} {$i < $val(nn) } {incr i} {
	set node_($i) [$ns node]	
	lappend node_list $node_($i)
}

$ns lan [ns newLan $node_list \
                   $val(bw) \
		   $val(delay) \
		   -llType $val(ll) \
		   -ifqType $val(ifq) \
		   -macType $val(mac) \
		   -chanType $val(chan)
]

for {set i 0} {$i < ($val(nn)-1)} {incr i} {

	#Setup a UDP connection
	set udp [new Agent/UDP]
	$ns attach-agent $node_($i) $udp($i)
	set cbr [new Application/Traffic/CBR]
	$cbr($i) set type_ CBR
	$cbr($i) set packet_Size_ 512
	$cbr($i) set rate_ 1.0Mb
	$cbr($i) set random_ 1
	$cbr($i) attach-agent $udp($i)

	set sink($i) [new Agent/Null]
	$ns connect $udp($i) $sink($i)
	$ns attach-agent $node($val(nn)) $sink($i)

	#Schedule events
	$ns at 0.1 "$cbr($i) start"
	$ns at 5 "$cbr($i) stop"
	$ns at 6 "finish"
}

#Start the simulation
$ns run

#Define a procedure finish
proc finish { } {
	global ns tracefd namtrace
	$ns flush-trace
	close $tracefd
	close $namtrace
	exec nam ethernet_nam.nam &
	exit 0
}
