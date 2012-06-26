$infile =$ARGV[0];
$outfile=$ARGV[1];

$count=0;
$pktcount=0;
$pktid=0;
$singledelay=0;
$totaldelay=0;
$endtoend=0;
$meandelay=0;

open(DATA,"$infile" ) || die "could't open $inflie$!";
open(DATA1,">$outfile") || die "Mention File.";

while(<DATA>){

	@x=split(' ');
	if($x[3] eq 'AGT')
	{
		if($x[0] eq 's')
		{
			$pktid=$x[5];

			$stime=$x[1];

		}
		while(<DATA>)
		{
			my @x=split(' ');
			if($x[0] eq 'r')
			{
				if($x[5]== $pktid)
				{
					$pktcount++;	
					$rtime=$x[1];
#					printf("\n Packet: $pktcount");
				}
				$singledelay=$rtime-$stime;
#		                printf("Single=$singledelay\n");
#				print DATA1 "$singledelay\n";
			}

#	        print DATA1 "$singledelay\n";
			$totaldealy=$totaldelay+$singledelay;
#		printf("Total $totaldelay\n");
		}

	}
}
$meandelay=$singledelay/$pktcount;

print("avgdelay=$meandelay");
close DATA1;
close DATA;
exit(0);
