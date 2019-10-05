module mul(A, B, O, ck, start, fin);
input[7:0] A, B;
input ck, start;
output [16:0] O;
output fin;
reg [7:0] INA, INB;
reg [16:0] O;
reg [3:0] st;
reg fin;
always @(posedge ck) begin
    if( start == 1) begin
    O <= 0;
    st <= 0;
    fin <= 0;
    INA <= A; INB <= B;
end else begin
    O<=(O<<1) + INA * INB[7-st];
    st <= st +1;
    if(st == 7) begin
    fin <= 1;
    end
    else if(st == 8) begin
    fin <= 0;
end

end
end
endmodule
