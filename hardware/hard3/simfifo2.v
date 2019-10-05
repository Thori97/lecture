module top;
   reg[7:0] Din;
   wire [7:0] Dout;
   reg ck, rst, Wen, Ren, flag;
   wire Fempty, Ffull;

/// For test purposes
   reg [7:0] FMEM[0:15];
   reg [3:0] Wptr, Rptr;
   reg 	     Fempty0, Ffull0;
   reg [7:0] obuf;
   wire [3:0] NWptr, NRptr;
   
   assign NWptr = Wptr + 1;
   assign NRptr = Rptr + 1;
/// For test purposes

   
   fifo fifo( Din, Dout, Wen, Ren, rst, ck, Fempty, Ffull );

   initial begin
	ck = 0;
	rst = 1;
	$dumpfile( "fifo.vcd" );
	$dumpvars;
	#20 rst = 0;
	#50 rst = 1;
	#10000 
	  $display( "OK\n" );
      $finish;
   end

   always @(negedge ck)begin
     flag = $random;
     if( flag == 1 )begin
	Wen <= 1;
	Ren <= 0;
	Din = $random;
     end else begin
	Wen <= 0;
	Ren <= 1;
     end
   end

   always #10 ck <= ~ck;


   
   always @(posedge ck) begin
      if( Fempty0 != Fempty || Ffull0 != Ffull || obuf != Dout ) begin
	 $finish;
      end
   end

   always @(posedge ck) begin
      if( !rst ) begin
	 Wptr <= 0;
	 Rptr <= 0;
	 Fempty0 <= 1;
	 Ffull0 <= 0;
      end else begin
	 if( Ren == 1 && Fempty0 != 1 ) begin
	    obuf <= FMEM[Rptr];
	    Rptr <= NRptr;
	    Ffull0 <= 0;
	    if( NRptr == Wptr ) Fempty0 <= 1;
	    else Fempty0 <= 0;
	 end
	 if( Wen == 1 && Ffull0 != 1 ) begin
	    FMEM[Wptr] <= Din;
	    Wptr <= Wptr + 1;
	    Fempty0 <= 0;
	    if( NWptr == Rptr ) Ffull0 <= 1;
	    else Ffull0 <= 0;
	    
	 end
      end
   end
      
endmodule
