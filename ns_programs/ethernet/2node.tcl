#
#Simple LAN topology
#

# Define options
set val(chan)       Channel
set val(mac)        Mac/802_3
set val(ifq)        Queue/DropTail
set val(ifqlen)     1
set val(ll)         LL
set val(seed)       1.0
set val(nn)         1
set val(stop)       300
set val(bw)         10Mb
set val(delay)      1ms
set val(ll)         LL
set val(phytype)    Phy/WiredPhy


# Create simulator instance
set ns [new Simulator]


# Define colors for flows in NAM
$ns color 1 Red
$ns color 2 Green
$ns color 3 Blue


# Create Trace and NAM
set tracefd [open 2node_trace.tr w]
set namtrace [open 2node_nam.nam w]
$ns trace-all $tracefd
$ns namtrace-all $namtrace


# Create nodes
for {set i 0} {$i < $val(nn) } {incr i} {
	set lannode($i) [$ns node]
	lappend lannode_list $lannode($i)
}


# Create LAN
set lan [$ns newLan $lannode_list $val(bw) $val(delay) -llType $val(ll) -ifqType $val(ifq) -macType $val(mac) -chanType $val(chan)]


# Set NAM positions

# Create Agents
# Setup a TCP connection
set ln1_tcp [new Agent/TCP]
$ln1_tcp set class_ 2
$ns attach-agent $lannode(0) $ln1_tcp

set ln2_sink [new Agent/TCPSink]
$ns attach-agent $lannode(1) $ln2_sink

$ns connect $ln1_tcp $ln2_sink
$ln1_tcp set fid_ 1

# Setup a FTP over TCP connection
set ftp [new Application/FTP]
$ftp attach-agent $ln1_tcp
$ftp set type_ FTP


# Define a procedure finish
proc finish { } {
	global ns tracefd namtrace
	$ns flush-trace
	close $tracefd
	close $namtrace
	exec nam 2node_nam.nam &
	exit 0
}


# Schedule events
$ns at 0.1 "$ftp start"
$ns at 5 "$ftp stop"
$ns at 6 "finish"



# Start the simulation
$ns run
