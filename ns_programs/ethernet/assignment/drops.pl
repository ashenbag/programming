$infile =$ARGV[0];

$packet_size = 0;
$total_enqueue_count = 0;
$total_receive_count = 0;
$total_drop_count = 0;
$simulation_time = 0;

open(DATA,"$infile" ) || die "could't open $inflie$!";

while(<DATA>)
{
	@x=split(' ');
	if(($x[0] eq '+'))
	{
		$total_enqueue_count++;
	}
	elsif(($x[0] eq 'r'))
	{
		$total_receive_count++;
	}
	elsif(($x[0] eq 'd'))
	{
		$total_drop_count++;
	}
}
$simulation_time = $x[1];
$packet_size = $x[5];

print("\n Packet Size = $packet_size bytes");
print("\n Simulation Time = $simulation_time seconds");
print("\n Total Enqueue Count = $total_enqueue_count");
print("\n Total Receive Count = $total_receive_count");
print("\n Total Drop Count = $total_drop_count");

print("\n");
print("\n");
close DATA;
exit(0);
