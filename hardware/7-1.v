module CPU(CK, RST, IA, ID, DA, DD, RW);
input CK, RST;
input [15:0] ID;
output RW;
output [15:0] IA, DA;
inout[15:0] DD;
reg[1:0] STAGE; 


always @(posedge CK)  begin
if( RST == 1 ) begin
end else beginif( STAGE == 0 )begin
STAGE <= 1;
end else if( STAGE == 1 ) begin
end
STAGE <= 2;
end else if( STAGE == 2) begin
STAGE <= 3;
end else if( STAGE == 3 ) begin
STAGE <= 0;
end
end
end
end module