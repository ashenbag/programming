$infile =$ARGV[0];

$packet_size = 0;
$total_rx_count=0;
$simulation_time = 0;
$system_throughput = 0;

open(DATA,"$infile" ) || die "could't open $inflie$!";

while(<DATA>)
{
	@x=split(' ');
	if(($x[0] eq 'r'))
	{
		$total_rx_count++;

	}
}
$simulation_time = $x[1];
$packet_size = $x[5];

print("\n Packet Size = $packet_size bytes");
print("\n Total Receive Count = $total_rx_count");
print("\n Simulation Time = $simulation_time seconds");

if($total_rx_count != 0)
{
	$system_throughput = ($total_rx_count) / $simulation_time;
        print("\n System Throughput = $system_throughput packets per second.");
	$system_throughput = ($total_rx_count * $packet_size * 8) / $simulation_time;
	$system_throughput = $system_throughput / 1000;
        print("\n System Throughput = $system_throughput Kbps");
}
else
{
	print("\n No packet reached.");
}

print("\n");
print("\n");
close DATA;
exit(0);
