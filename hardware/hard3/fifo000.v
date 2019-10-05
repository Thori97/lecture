module fifo ( Din, Dout, Wen, Ren, rst, ck, Fempty, Ffull );
input [7:0] Din;
output [7:0] Dout;
input Wen, Ren
output Fempty, Ffull;

reg  [7:0] FMEM[0:1X];
reg  [3:0] Wptr, Rptr;
reg  Fempty, Ffull;
reg  [7:0] obuf;
wire [3:0] NWptr, NRptr;


always @(posedge ck) begin
    if( !rst ) begin
    end else begin
    end
end
endmodule
