<?php
// compute Erdos multiplication persistence of numbers x=dddddd...dddd (n times d), and for next x with increasing n.
// display and store best persistences found.
// usage: php gmp_persistance.php digit nb [pmin]
// ex: php gmp_persistance.php 9 149497
// pmin is used to display only results with p(x)>=pmin

include("inc_mulperErdos.php");

$digit=8; if (isset($argv[1])) $digit=$argv[1];
$n=314985; if (isset($argv[2])){ $n=1*$argv[2]; }
$pmin=0; if (isset($argv[3])){ $pmin=1*$argv[3]; }

$fmax="max_${digit}_from${n}.txt";
$nDeb=$n;

// compute first (dont make a n-length string: directly computes the product digit^n)
echo "compute first...";
$keep=gmp_pow($digit, $n);
echo "done.\n";

$best=array();
for(;;){
	echo "$nDeb..$n".chr(13);
	$t1=microtime(true);
	$p=persistErdosGMP($keep);
	$t2=microtime(true);
	if (!isset($best[$p])){
		$best[$p]=$n;
		if ($p>=$pmin){
			echo "p($digit"."^"."$n)=$p ".number_format($t2-$t1, 3, "'", "")."\n";
			$f=fopen($fmax,"a");
			fwrite($f, "$digit^$n=$p\n");
			fclose($f);
		}
	}
	$n++;
	if ($digit==2) $keep=gmp_add($keep, $keep); else $keep=gmp_mul($keep, $digit);
}

