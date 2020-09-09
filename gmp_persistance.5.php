<?php
// calcule la persistance a la Erdos du nombre dddddd (d n fois), ainsi que pour les n suivants
// ex : php gmp_persistance.php 149497 8
/*
persistErdos(8^2)=3
persistErdos(8^4)=4
persistErdos(8^6)=6
persistErdos(8^9)=7
persistErdos(8^39)=9
persistErdos(8^55)=10
persistErdos(8^73)=11
persistErdos(8^90)=13
persistErdos(8^319)=14
persistErdos(8^459)=15
persistErdos(8^941)=16
persistErdos(8^1589)=17
persistErdos(8^3998)=18
persistErdos(8^7924)=19
persistErdos(8^14581)=20	// ML
persistErdos(8^32935)=21	// ML
persistErdos(8^68764)=22	// ML
persistErdos(8^149497)=23	// ML
persistErdos(8^314985)=24	// ML
persistErdos(8^744484)=25	// JC
persistErdos(8^1525361)=26	// ML
persistErdos(8^6728155)=27	// ML
persistErdos(8^???)=28
persistErdos(8^???)=29
*/

$n=1; if (isset($argv[1])){ $n=1*$argv[1]; }
$digit=8; if (isset($argv[2])) $digit=$argv[2];

// if working with huge numbers, uncomment next line
// ini_set("memory_limit", "4096M");
$decal=15; // if you have enough memory, use $decal=16 : it will consume more memory but will be faster
$precalc=1<<$decal;
echo "precalc x^$precalc...";
echo "(decal=$decal precalc=$precalc) ";
$tp=array();	// used to precalc for each digit : digit^1, digit^2, digit^3, ...
for($j=2;$j<10;$j++){
	echo "$j ";
	$tp[$j]=array();
	$tp[$j][0]=1;
	for($i=1;$i<=$precalc;$i++) $tp[$j][$i]=gmp_mul($tp[$j][$i-1], $j);
}
echo "done.\n";

$fmax="max_${digit}_from${n}.txt";
$a=str_repeat($digit, $n);
$tmax=array();
$fmax="max_${digit}_from${n}.txt";
$nDeb=$n;

// compute first
$str=$a;
$len=strlen($str);
$nb5=floor($len/$precalc);
$residu=$len%$precalc;
$s=1;
$s=gmp_mul($s, $tp[$digit][$residu]);
for($k=$residu;$k<$len;$k+=$precalc) $s=gmp_mul($s, $tp[$digit][$precalc]);
$keep=$s; // $keep=gmp_strval($s);
// echo "mult($digit"."^"."$n)=".gmp_strval($s)."\n";
$best=array();
for(;;){
	echo "$nDeb..$n".chr(13);
	$t1=microtime(true);
	$p=persistErdosOptSecond($keep);
	$t2=microtime(true);
	if (!isset($best[$p])){
		$best[$p]=$n;
		echo "p($digit"."^"."$n)=$p ".number_format($t2-$t1, 3, "'", "")."\n";
		$f=fopen($fmax,"a");
		fwrite($f, "$digit^$n=$p\n");
		fclose($f);
	}
	$n++;
	if ($digit==2) $keep=gmp_add($keep, $keep); else $keep=gmp_mul($keep, $digit);
}
die("ok");
function persistErdosOptSecond($a){
	global $tp;
	global $decal, $precalc;
	$ten = gmp_init("10");
	for($i=1;;$i++){
		// echo "i=$i a=".gmp_strval($a)."\n";
		// if (gmp_cmp($a, $ten)==-1) return $i;
		$str=(string)$a;
		$len=strlen($str);
		if ($len<=1) return $i;
		$occ=array();
		for($j=0;$j<10;$j++) $occ[$j]=0;
		for($j=0;$j<$len;$j++) $occ[$str{$j}]++;

		$n=1;

		// 2
		$nbocc=$occ[2];
		$n256 = $nbocc >> $decal;
		$reste=$nbocc-($n256<<$decal);
		// for($j=0;$j<$reste;$j++) $n = gmp_add($n, $n);
		$n *= $tp[2][$reste];
		for($j=0;$j<$n256;$j++) $n *= $tp[2][$precalc];

		// 3..9
		for($k=3;$k<10;$k++){
			$nbocc=$occ[$k];
			$n256 = $nbocc >> $decal;
			$reste=$nbocc-($n256<<$decal);
			// for($j=0;$j<$reste;$j++) $n = gmp_mul($n, $k);
			$n *= $tp[$k][$reste];
			for($j=0;$j<$n256;$j++) $n *= $tp[$k][$precalc];
		}
		$a=$n;
		// echo "persistErdosOpt::a=".gmp_strval($a)."\n";
	}
	return $i;
}

