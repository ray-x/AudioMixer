
module	adder16_8(
					A1,
					A2,
					A3,
					A4,
					A5,
					A6,
					A7,
					A8,
					S
					);
input	[15:0]	A1;
input	[15:0]	A2;
input	[15:0]	A3;
input	[15:0]	A4;
input	[15:0]	A5;
input	[15:0]	A6;
input	[15:0]	A7;
input	[15:0]	A8;
output	[15:0]	S;

wire	[15:0]	A21;
wire	[15:0]	A22;
wire	[15:0]	A23;
wire	[15:0]	A24;
wire	[15:0]	A31;
wire	[15:0]	A32;
//step1
adder16		adder16_ins1(.A(A1),.B(A2),.S(A21));
adder16		adder16_ins2(.A(A3),.B(A4),.S(A22));
adder16		adder16_ins3(.A(A5),.B(A6),.S(A23));
adder16		adder16_ins4(.A(A7),.B(A8),.S(A24));

//step2
adder16		adder16_ins5(.A(A21),.B(A22),.S(A31));
adder16		adder16_ins6(.A(A23),.B(A24),.S(A32));
//step3
adder16		adder16_ins7(.A(A31),.B(A32),.S(S));

endmodule
