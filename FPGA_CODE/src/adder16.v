
module adder16(A,B,S,Cout);

input	[15:0]	A;
input	[15:0]	B;
output	[15:0]	S;
output			Cout;

assign	{Cout,S} = A + B;

endmodule
