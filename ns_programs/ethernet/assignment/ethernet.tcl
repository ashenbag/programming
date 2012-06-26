# tcl script for ns-2 to simulate an Ethernet LAN

# Interface buffer size
Queue set limit_ 100

# A semicolon is needed before a # for a comment in the middle of a line !!

set opt(tr) ethernet.tr;     # Trace file name
set opt(namtr)  ethernet.nam
set opt(node)   4;      # No. of nodes, n
set opt(bw)     10Mb;    # Nominal link bandwidth
set opt(delay)  0.1ms;   # Nominal link delay
set opt(ll)     LL;      # Link layer to use in ns-2
set opt(ifq)    Queue/DropTail
                        # Drop tail queue to be used for buffers
                        # Drop tail queue drops from the tail end
                        # when full.
set opt(mac)    Mac/802_3
                        # MAC protocol type
set opt(chan)   Channel;

set opt(offered_load) 3.0Mb
set opt(packet_size) 512
set opt(stop_time) 11.0

# Some helper functions
proc finish {} {
        global ns opt trfd

        $ns flush-trace
        close $trfd
        exit 0
}

proc create-trace {} {
        global ns opt
        global lan

        if [file exists $opt(tr)] {
#               catch "exec rm -f $opt(tr) $opt(tr)-bw [glob $opt(tr).*]"
                catch "exec rm -f $opt(tr) $opt(tr)-bw"
        }

        set trfd [open $opt(tr) w]
        $ns trace-all $trfd
        if {$opt(namtr) != ""} {
                $ns namtrace-all [open $opt(namtr) w]
        }
        return $trfd
}


proc create-topology {} {
        global ns opt
        global lan node

        set num $opt(node)
        for {set i 0} {$i < $num} {incr i} {
                set node($i) [$ns node]
                lappend nodelist $node($i)
        }

# This really specifies how the LAN should look like.

        set lan [$ns newLan $nodelist $opt(bw) $opt(delay) \
                        -llType $opt(ll) -ifqType $opt(ifq) \
                        -macType $opt(mac) -chanType $opt(chan)]
}

## MAIN ##

        set ns [new Simulator]
        set trfd [create-trace]

        create-topology

# Set num = n-1
        set num [ expr $opt(node) - 1 ]

# Nodes 0 to n-2 are sources. Set up UDP sources on each of them
# and attach the sources to the sinks situated on the last node, n-1.
# Do this in a loop.

        for {set i 0} {$i < $num} {incr i} {

# Create and attach the UDP agent on the source nodes
                set udp($i) [new Agent/UDP]
                $ns attach-agent $node($i) $udp($i)

# Create an application (CBR) that will actually generate the
# traffic. Specify the parameters and attach it to UDP.

                set cbr($i) [new Application/Traffic/CBR]
#                $cbr($i) set packetSize_ 512
#                $cbr($i) set rate_ 1.0Mb
                $cbr($i) set packetSize_ $opt(packet_size)
                $cbr($i) set rate_ $opt(offered_load)
                $cbr($i) set random_ 1
                $cbr($i) attach-agent $udp($i)

# The sink is just the other end of the UDP agent.
# All sinks are located on the last node, n-1.
                set sink($i) [new Agent/Null]
                $ns attach-agent $node($num) $sink($i)
                $ns connect $udp($i) $sink($i)

# Instruct ns to start and stop the cbr sources at specific times
                $ns at 1.0 "$cbr($i) start"
                $ns at $opt(stop_time) "$cbr($i) stop"
                }

# The finish event will execute at the specified time.
# Note that we finish the simulation a little later after the
# sources stop just to make sure that all packets in transit
# are received. But the effective length of the simulation
# is as long as the cbr sources run.

        set finish_time [ expr $opt(stop_time) + 1 ]
        $ns at $finish_time "finish"
# Now start the simulator.
        $ns run
