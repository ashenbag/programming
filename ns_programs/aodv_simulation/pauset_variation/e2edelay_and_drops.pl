#!/usr/bin/perl
#
#Program to calculate end to end delay.
#

use strict;
use warnings;

#Input trace file
my($infile) =$ARGV[0]; 

#Keep track of variables
my($enqueue_time) = 0;
my($receive_time) = 0;
my($packet_id) = 0;
my($delay) = 0;
my($total_enqueue_count) = 0;
my($total_receive_count) = 0;
my($total_drop_count) = 0;
my($sum_of_delay) = 0;
my($average_delay) = 0;
my($simulation_time) = 0;
my(@x);
my(%hash_table);
my($hash_key) = 0;
my($hash_value) = 0;
my($last_packet_id) = 0;

open(DATA,"<","$infile" ) || die "could't open $infile$!";

while(<DATA>)
{
	@x=split(' ');
	if($x[3] eq 'AGT')
	{
		$hash_key = $x[0].$x[5];
		$hash_value = $x[1];
		$hash_table{"$hash_key"} = $hash_value;

		if($x[5] gt $last_packet_id)
		{
			$last_packet_id = $x[5];
		}
	}
}

$simulation_time = $x[1];

for($packet_id = 0;$packet_id<=$last_packet_id;$packet_id++)
{
	$hash_key = "s".$packet_id;
        $enqueue_time = $hash_table{"$hash_key"};
        $total_enqueue_count++;
	$hash_key = "r".$packet_id;
	if(exists($hash_table{$hash_key}))
	{
		$receive_time = $hash_table{"$hash_key"};
	}
	else
	{
                $total_drop_count++;
		next;
	}
	$total_receive_count++;
	$delay = $receive_time - $enqueue_time;
	$sum_of_delay = $sum_of_delay + $delay;

#	$delay = $delay * 1000;
#       print("\nDelay:$delay");

}

print("\n Simulation Time = $simulation_time seconds");
print("\n Total Enqueue Count = $total_enqueue_count");
print("\n Total Receive Count = $total_receive_count");
print("\n Total Drop Count = $total_drop_count");

if($total_receive_count != 0 )
{
	$average_delay = $sum_of_delay / $total_receive_count;
	$average_delay = $average_delay * 1000;
	print("\n Average End to End Delay = $average_delay milliseconds");
}
else
{
	print("\n No packet received.");
}

print("\n");
print("\n");
close DATA;
exit(0);
