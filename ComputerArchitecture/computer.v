// sample.bnrから疑似機械語コードを読みこみ，命令をinsに代入していく
module fetch(pc, ins); input	[7:0]	pc;
	output	[31:0]	ins;
	reg		[31:0]	ins_mem[0:255];
  initial begin
    $readmemb("sample.bnr", ins_mem);
  end
	assign ins = ins_mem[pc];
endmodule

// d_memにデータを格納し，wrenの01でwrite/readをするRAM
module data_mem(address, clk, write_data, wren, read_data);
	input	[7:0]	address;
	input	clk, wren;
	input	[7:0]	write_data;
	output	[7:0]	read_data;
	reg		[7:0]	d_mem[0:255];

	always @(posedge clk)
		if (wren == 0)	d_mem[address] <= write_data;
	assign read_data = d_mem[address];
endmodule

// 演算を行うCPU本体
module execute(clk, ins, pc, reg1, reg2, wra, result, nextpc);
	input	clk;
	input	[31:0]	ins, pc, reg1, reg2;
	output	[4:0]	wra;
	output	[31:0]	result, nextpc;
	wire	[5:0]	op;
	wire	[4:0]	shift, operation;
	wire	[25:0]	addr;
	wire	[31:0]	dpl_imm, operand2, alu_result, 
					nonbranch, branch, mem_address, dm_r_data;
	wire	[3:0]	wren;

  // 行う操作を決める関数
	function	[4:0]	opr_gen;
		input	[5:0]	op;
		input	[4:0]	operation;
		case (op)
      // R型はこっち
			6'd0:	opr_gen = operation;
      // I型でALuを用いるのは以下
			6'd1:	opr_gen = 5'd0;
			6'd4:	opr_gen = 5'd8;
			6'd5:	opr_gen = 5'd9;
			6'd6:	opr_gen = 5'd10;
      // 変更箇所(bgt0_sub)
      6'd36: opr_gen = 5'd2;
			default: opr_gen = 5'h1f;
		endcase
	endfunction



  // 計算を行います(jmp命令など簡単な演算以外はcalcでやるっぽい)
	function	[31:0]	alu;
		input	[4:0]	opr, shift;
		input	[31:0]	operand1, operand2;
		case (opr)
			5'd0:	alu = operand1 + operand2;
			5'd2:	alu = operand1 - operand2;
			5'd8:	alu = operand1 & operand2;
			5'd9:	alu = operand1 | operand2;
			5'd10:	alu = operand1 ^ operand2;
			5'd11:	alu = ~(operand1 & operand2);
			5'd16:	alu = operand1 << shift;
			5'd17:	alu = operand1 >> shift;
			5'd18:	alu = operand1 >>> shift;
			default:	alu = 32'hffffffff;
		endcase
	endfunction

  // alu以外の演算全般を行います
	function	[31:0]	calc;
		input	[5:0]	op;
		input	[31:0]	alu_result, dpl_imm, dm_r_data, pc;
		case (op)
      // 変更箇所(bgt0_sub)
			6'd0, 6'd1, 6'd4, 6'd5, 6'd6, 6'd36:	calc = alu_result;
			6'd3:	calc = dpl_imm << 16;
			6'd16:	calc = dm_r_data;
			6'd18:	calc = {{16{dm_r_data[15]}}, dm_r_data[15:0]};
			6'd20:	calc = {{24{dm_r_data[7]}}, dm_r_data[7:0]};
			6'd41:	calc = pc + 32'd1;
			default:	calc = 32'hffffffff;
		endcase
	endfunction

	function	[31:0]	npc;
		input	[5:0]	op;
		input	[31:0]	reg1, reg2, branch, nonbranch, addr;
		case (op)
			6'd32:	npc = (reg1 == reg2) ? branch : nonbranch;
			6'd33:	npc = (reg1 != reg2) ? branch : nonbranch;
			6'd34:	npc = (reg1 < reg2)  ? branch : nonbranch; 
			6'd35:	npc = (reg1 <= reg2) ? branch : nonbranch;
      // 変更箇所(bgt0_sub)
      6'd36:  npc = (reg1 > reg2)  ? branch : nonbranch;
			6'd40, 6'd41:	npc = addr;
			6'd42:	npc = reg1;
			default:	npc = nonbranch;
		endcase
	endfunction

	function	[4:0]	wreg;
		input	[5:0]	op;
		input	[4:0]	rs, rt, rd;
		case (op)
			6'd0:	wreg = rd;
			6'd1, 6'd3, 6'd4, 6'd5, 6'd6, 6'd16, 6'd18, 6'd20:	wreg = rt;
      // 変更箇所(bgt0_sub)
      6'd36: wreg = rs;
			6'd41:	wreg = 5'd31;
			default:	wreg = 5'd0;
		endcase
	endfunction

	function	[3:0]	wrengen;
		input	[5:0]	op;
		case (op)
			6'd24:	wrengen = 4'b0000;
			6'd26:	wrengen = 4'b1100;
			6'd28:	wrengen = 4'b1110;
			default:	wrengen = 4'b1111;
		endcase
	endfunction

	assign op = ins[31:26];
	assign shift = ins[10:6];
	assign operation = ins[4:0];
	assign dpl_imm = {{16{ins[15]}}, ins[15:0]};
	assign operand2 = (op == 6'd0 || op == 6'd36) ? reg2 : dpl_imm;
	assign alu_result = alu(opr_gen(op, operation), shift, reg1, operand2);

	assign mem_address = (reg1 + dpl_imm) >>> 2;
	assign wren = wrengen(op);
  initial $monitor("op=%d, operation=%d, dpl_imm=%b, alu_result=%d, result=%d, branch=%d, nonbranch=%d, pc=%d, nextpc=%d\n", op, operation, dpl_imm, alu_result, result, branch, nonbranch, pc, nextpc);
	data_mem data_mem_body0(mem_address[7:0], clk, reg2[7:0],   wren[0], dm_r_data[7:0]  );
	data_mem data_mem_body1(mem_address[7:0], clk, reg2[15:8],  wren[1], dm_r_data[15:8] );
	data_mem data_mem_body2(mem_address[7:0], clk, reg2[23:16], wren[2], dm_r_data[23:16]);
	data_mem data_mem_body3(mem_address[7:0], clk, reg2[31:24], wren[3], dm_r_data[31:24]);

	assign wra = wreg(op, ins[25:21], ins[20:16], ins[15:11]);
	assign result = calc(op, alu_result, dpl_imm, dm_r_data, pc);

	assign addr = ins[25:0];
	assign nonbranch = pc + 32'd1;
	assign branch = nonbranch + dpl_imm;
	assign nextpc = npc(op, reg1, reg2, branch, nonbranch, addr);
endmodule
			
// PC変えてるだけ
module writeback(clk, rstd, nextpc, pc);
	input	clk, rstd;
	input	[31:0]	nextpc;
	output	[31:0]	pc;
	reg		[31:0]	pc;
	always @(negedge rstd or posedge clk) begin
		if (rstd == 0)	pc <= 32'h00000000;
		else if (clk == 1)	pc <= nextpc;
	end
endmodule

// レジスターフィールド(reg1, reg2等が乗ってる．RAMとは別です)
// reg1, reg2の読み込み/書き込みはここで行います
module reg_file(clk, rstd, wr, ra1, ra2, wa, wren, rr1, rr2);
	input	clk, rstd, wren;
	input	[31:0]	wr;
	input	[4:0]	ra1, ra2, wa;
	output	[31:0]	rr1, rr2;
	reg		[31:0]	rf[0:31];

	assign rr1 = rf[ra1];
	assign rr2 = rf[ra2];

  // レジスターの内容を知りたいときにコメントアウトを外すと見れます
   initial $monitor("rf=[%d, %d, %d, %d, %d, %d]\n", rf[0], rf[1], rf[2], rf[3], rf[4], rf[5]);

	always @(negedge rstd or posedge clk) begin
		if (rstd == 0)	rf[0] <= 32'h00000000;
		else if (wren == 0)	rf[wa] <= wr;
	end
endmodule

module computer(clk, rstd);
	input	clk, rstd;
	wire	[31:0]	pc, ins, reg1, reg2, result, nextpc;
	wire	[4:0]	wra;
	wire	[3:0]	wren;

  // 他の内容を見たい場合はコメントアウトしてください(同時にmonitorが起動するのは1つのみなので)
  // initial begin
    // $monitor("pc=%d, ins=%b, reg1=%d, reg2=%d, result=%d\n", pc, ins, reg1, reg2, result);
  // end

	fetch fetch_body(pc[7:0], ins);
	execute execute_body(clk, ins, pc, reg1, reg2, wra, result, nextpc);
	writeback writeback_body(clk, rstd, nextpc, pc);
	reg_file rf_body(clk, rstd, result, ins[25:21], ins[20:16], wra, (~|wra), reg1, reg2);
endmodule