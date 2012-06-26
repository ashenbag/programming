$infile =$ARGV[0];
$rp = $ARGV[1];

$packet_size = 512;
$total_tx_count=0;
$total_rx_count=0;
$simulation_time = 0;
$system_throughput = 0;
$system_pdr = 0;
$total_routing_tx_count = 0;
$total_routing_load = 0;

open(DATA,"$infile" ) || die "could't open $inflie$!";

while(<DATA>)
{
	@x=split(' ');
	if($x[18] eq 'AGT' && $x[44] eq 'cbr')
	{
		if(($x[0] eq 's'))
		{
			$total_tx_count++;
		}
		if(($x[0] eq 'r'))
		{
			$total_rx_count++;
		}
	}
        #DSR
	if($rp eq 1)
	{
		if(($x[8].'.255' eq $x[30]) && ($x[48] eq '1') || ($x[52] eq '1') || ($x[60] eq '1'))
		{
			if(($x[0] eq 's'))
			{
				$total_routing_tx_count++;
			}
		}
	}
        #AODV
	else
	{
		if(($x[8].'.255' eq $x[30]) && ($x[60] eq 'REQUEST') || ($x[56] eq 'REPLY') || ($x[56] eq 'ERROR'))
		{
			if(($x[0] eq 's'))
			{
				$total_routing_tx_count++;
			}
		}
	}
}
$simulation_time = $x[2];
#$packet_size = $x[36];

print("\n Packet Size = $packet_size bytes");
print("\n Simulation Time = $simulation_time seconds");
print("\n Total Transmit Count = $total_tx_count");
print("\n Total Receive Count = $total_rx_count");

if($total_rx_count != 0)
{
	$system_throughput = ($total_rx_count) / $simulation_time;
        #print("\n System Throughput = $system_throughput packets per second.");
	$system_throughput = ($total_rx_count * $packet_size * 8) / $simulation_time;
	$system_throughput = $system_throughput / 1000;
        #print("\n System Throughput = $system_throughput Kbps");
        $system_pdr = ($total_rx_count/$total_tx_count);
        print("\n Packet delivery ratio (PDR) = $system_pdr");
        print("\n Total Routing Packets sent = $total_routing_tx_count");
        $total_routing_load = $total_routing_tx_count / $total_rx_count;
        print("\n Routing Load = $total_routing_load");

}
else
{
        print("\n Packet delivery ratio (PDR) = 0");
	print("\n No packet reached!");
}

print("\n");
print("\n");
close DATA;
exit(0);
