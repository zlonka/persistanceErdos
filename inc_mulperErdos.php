<?php
//////////////// persistErdo lib /////////////////
//
// function persistErdosStr(string $str)
// function persistErdosGMP(GMP $a)
//
// usage with string
// persistErdosStr(string $str)
// ex:
// $p=persistErdosStr($str="267777777889999");
// echo "x=$str pE(x)=$p\n";
//
// usage with computed digit product
// persistErdosGMP(GMP $a)
// ex:
// $digit=9; $n=147272; $x=gmp_pow($digit, $n);
// $p=persistErdosGMP($x);
// echo "x=$digit$digit$digit...$digit$digit$digit ($n times) pE(x)=$p\n";
//
//////////////////////////////////////////////////

function persistErdosStr(string $str){
	$len=strlen($str);
	if ($len<=1) return 1;
	$occ=array();
	for($j=0;$j<10;$j++) $occ[$j]=0;
	for($j=$len-1;$j>=0;$j--) $occ[$str[$j]]++;
	$a=1;
	for($k=2;$k<10;$k++){
		if ($occ[$k]) $a *= gmp_pow($k, $occ[$k]);
	}
	return persistErdosGMP($a);
}
function persistErdosGMP(GMP $a){
	$occ=array();
	for($i=1;;$i++){
		$str=(string)$a;
		$len=strlen($str);
		if ($len<=1) return $i;
		for($j=0;$j<10;$j++) $occ[$j]=0;
		for($j=$len-1;$j>=0;$j--) $occ[$str[$j]]++;

		$a=1;
		for($k=2;$k<10;$k++){
			if ($occ[$k]) $a *= gmp_pow($k, $occ[$k]);
		}
	}
	return $i;
}
?>
