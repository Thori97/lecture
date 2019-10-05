module simcpu;
reg CK, RST;
wire RW;
wire [15:0] IA, DA, DD;
reg [15:0] ID, DDi;
reg [15:0] IMEM [0:127], DMEM[0:127];

wire  [15:0]  DMEM00, DMEM01, DMEM02;
	CPU c(CK,RST,IA,ID,DA,DD,RW);

assign DMEM00 = DMEM[0];
assign DMEM01 = DMEM[1];
assign DMEM02 = DMEM[2];

   assign DD = ((RW == 1) ? DDi : 'b Z);

initial begin
	CK = 0;
	RST = 0;
#5	RST = 1;
#100	RST = 0;
	
        #10000 $finish;
end

always @(negedge CK) begin
   if( DA == 'b 0 && DD == 'b 0100 && RW == 0 ) begin
      $display( "OK" );
      $finish;
   end
end

always @(negedge CK) begin
	ID = IMEM[IA];
end

always @(negedge CK) begin
   if( RW == 1 ) DDi = DMEM[DA];
   else DMEM[DA] = DD;
end

initial begin
DMEM[0]= 5;
DMEM[1]= 15;
//
IMEM[0]='b 1100_0001_0000_0000; // IMM  R1, [0]
IMEM[1]='b 1100_0111_0000_0000; // IMM  R7, [0]
IMEM[2]='b 1100_1000_0000_0001; // IMM  R8, [1]
IMEM[3]='b 1100_1001_0000_0010; // IMM  R9, [2]
IMEM[4]='b 1100_0011_0000_0001; // IMM  R3, [1]
IMEM[5]='b 1100_0101_0000_1101; // IMM  R5, [13]
IMEM[6]='b 1100_0110_0000_1001; // IMM  R6, [9]
IMEM[7]='b 1011_0010_0000_0111; // LD R2, R7
IMEM[8]='b 1011_0100_0000_1000; // LD R4, R8
IMEM[9]='b 0000_0001_0001_0010; // ADD  R1, R1, R2
IMEM[10]='b 0001_0100_0100_0011; // SUB  R4, R4, R3
IMEM[11]='b 1001_0000_0000_0101; // BR f=0, R5
IMEM[12]='b 1000_0000_0000_0110; // JMP R0, R6
IMEM[13]='b 1010_0000_0001_1001; // ST R1, R9
end

always #10 CK = ~CK;

endmodule
