module testcount4r2m;
  wire		[3:0] out;
  reg		ck, res;
  initial begin
	$monitor( "%t %b %b %b", $time, ck, res, out);
	ck<=0;
	res<=0;
	#40
	res <= 1;
	#60
	  if( out == 4'b1000 )  $display("OK\n");
     
	$finish;
  end
  always #10  ck <= ~ck;
  count4r2m  cnt ( out, ck, res );
endmodule

