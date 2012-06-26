$infile =$ARGV[0];

$packet_size = 0;
$dequeue_time = 0;
$receive_time = 0;
$source_address = 0;
$destination_address = 0;
$packet_id = 0;
$delay = 0;
$total_receive_count = 0;
$sum_of_delay = 0;
$average_delay = 0;
$simulation_time = 0;
$file_postion = 0;

open(DATA,"<","$infile" ) || die "could't open $inflie$!";

while(<DATA>)
{
	@x=split(' ');
	if(($x[0] eq '-'))
	{
		$dequeue_time = $x[1];
		$source_address = $x[8];
		$destination_address = $x[9];
		$packet_id = $x[11];
	}
	$file_position = tell(DATA);
	while(<DATA>)
	{
		if(($x[0] eq 'r'))
		{
			if(($x[8] == $source_address) && ($x[9] == $destination_address) && ($x[11] == $packet_id))
			{
				$receive_time = $x[1];
				$total_receive_count++;
				$delay = $receive_time - $dequeue_time;
				$sum_of_delay = $sum_of_delay + $delay;

				last;
			}
		}
	}
	seek(DATA,$file_position,0);
}

$simulation_time = $x[1];
$packet_size = $x[5];

print("\n Packet Size = $packet_size bytes");
print("\n Simulation Time = $simulation_time seconds");
print("\n Total Receive Count = $total_receive_count");

if($total_receive_count != 0 )
{
	$average_delay = $sum_of_delay / $total_receive_count;
	$average_delay = $average_delay * 1000;
	print("\n Average Network Delay = $average_delay milliseconds");
}
else
{
	print("\n No packet received.");
}

print("\n");
print("\n");
close DATA;
exit(0);
