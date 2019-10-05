module fifo ( Din, Dout, Wen, Ren, rst, ck, Fempty, Ffull );
input [7:0] Din;
output [7:0] Dout;
input Wen, Ren, rst, ck;
output Fempty, Ffull;

reg  [7:0] FMEM[0:15];
reg  [3:0] Wptr, Rptr;
reg  Fempty, Ffull;
reg  [7:0] obuf;
wire [3:0] NWptr, NRptr;

assign Dout = obuf; assign NWptr = Wptr +1; assign NRptr = Rptr +1;

always @(posedge ck) begin
    if( !rst ) begin
    Wptr <= 0;
    Rptr <= 0;
    Fempty <= 1;
    Ffull <= 0;
    end else begin
    if(Ren == 1&&Fempty!= 1)begin
        obuf <= FMEM[Rptr];
        Rptr <= NRptr;
        Ffull <= 0;
        if(NRptr==Wptr) Fempty<=1;
        else Fempty <=  0;
    end
    if(Wen ==1 && Ffull != 1) begin
        FMEM[Wptr]<= Din;
        Wptr <= Wptr+1;
        Fempty<=0;
        if(NWptr==Rptr) Ffull<=1;
        else Ffull<=0;
    end
end
end
endmodule