
module mul_gain(
				IN,
				GAIN,
				OUT
				);
				
input	[15:0]	IN;
input	[7:0]	GAIN;
output	[15:0]	OUT;

wire	[15:0]	adder11;
wire	[15:0]	adder12;
wire	[15:0]	adder13;
wire	[15:0]	adder14;
wire	[15:0]	adder15;
wire	[15:0]	adder16;
wire	[15:0]	adder17;
wire	[15:0]	adder18;

assign	adder11 = GAIN[7]?({IN[15],IN[15],IN[15],IN[15],IN[15:4]}):16'h0000;
assign	adder12 = GAIN[6]?({IN[15],IN[15],IN[15],IN[15],IN[15],IN[15:5]}):16'h0000;
assign	adder13 = GAIN[5]?({IN[15],IN[15],IN[15],IN[15],IN[15],IN[15],IN[15:6]}):16'h0000;
assign	adder14 = GAIN[4]?({IN[15],IN[15],IN[15],IN[15],IN[15],IN[15],IN[15],IN[15:7]}):16'h0000;
assign	adder15 = GAIN[3]?({IN[15],IN[15],IN[15],IN[15],IN[15],IN[15],IN[15],IN[15],IN[15:8]}):16'h0000;
assign	adder16 = GAIN[2]?({IN[15],IN[15],IN[15],IN[15],IN[15],IN[15],IN[15],IN[15],IN[15],IN[15:9]}):16'h0000;
assign	adder17 = GAIN[1]?({IN[15],IN[15],IN[15],IN[15],IN[15],IN[15],IN[15],IN[15],IN[15],IN[15],IN[15:10]}):16'h0000;
assign	adder18 = GAIN[0]?({IN[15],IN[15],IN[15],IN[15],IN[15],IN[15],IN[15],IN[15],IN[15],IN[15],IN[15],IN[15:11]}):16'h0000;

adder16_8	adder16_8_ins1(.A1(adder11),.A2(adder12),.A3(adder13),.A4(adder14),.A5(adder15),.A6(adder16),.A7(adder17),.A8(adder18),.S(OUT));
				
endmodule 