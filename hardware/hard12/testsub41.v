module testsub41;
  wire		[4:0] s;
  reg		[3:0] a, b;
  initial begin
	$monitor( "%t %b - %b = %b", $time, a, b, s);
	a <= 0;   b <= 0;
	#40	a <= 8;   b <= 3;
	#40	a <= 7;   b <= 1;
	#40	a <= $random;   b <= $random;
	#40	a <= $random;   b <= $random;
	#40
	$finish;
  end
  sub4 sub ( s,a,b );
endmodule