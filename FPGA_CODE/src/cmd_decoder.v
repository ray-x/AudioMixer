
module cmd_decoder(
					clk,
					nRst,
					wr_en,						//1:write enable 0:write disable
					cmd_data_sw,				//1:command 0:data
					data,
					ws,
					codec1_in,
					codec2_in,
					codec3_in,
					codec4_in,
					codec5_in,
					codec6_in,
					codec7_in,
					codec8_in,
					codec1_out,
					codec2_out,
					codec3_out,
					codec4_out
					);
//Port Declaration
input			clk;
input			nRst;
input			wr_en;
input			cmd_data_sw;
input	[7:0]	data;
input			ws;
input	[15:0]	codec1_in;
input	[15:0]	codec2_in;
input	[15:0]	codec3_in;
input	[15:0]	codec4_in;
input	[15:0]	codec5_in;
input	[15:0]	codec6_in;
input	[15:0]	codec7_in;
input	[15:0]	codec8_in;
output	reg	[15:0]	codec1_out;
output	reg [15:0]	codec2_out;
output	reg [15:0]	codec3_out;
output	reg [15:0]	codec4_out;
//Inside Signal
reg		[7:0]	cmd_r;
reg		[7:0]	data_r;
reg				ws_reg;

reg		[7:0]	codec_1to1_gain;
reg		[7:0]	codec_2to1_gain;
reg		[7:0]	codec_3to1_gain;
reg		[7:0]	codec_4to1_gain;
reg		[7:0]	codec_5to1_gain;
reg		[7:0]	codec_6to1_gain;
reg		[7:0]	codec_7to1_gain;
reg		[7:0]	codec_8to1_gain;

reg		[7:0]	codec_1to2_gain;
reg		[7:0]	codec_2to2_gain;
reg		[7:0]	codec_3to2_gain;
reg		[7:0]	codec_4to2_gain;
reg		[7:0]	codec_5to2_gain;
reg		[7:0]	codec_6to2_gain;
reg		[7:0]	codec_7to2_gain;
reg		[7:0]	codec_8to2_gain;

reg		[7:0]	codec_1to3_gain;
reg		[7:0]	codec_2to3_gain;
reg		[7:0]	codec_3to3_gain;
reg		[7:0]	codec_4to3_gain;
reg		[7:0]	codec_5to3_gain;
reg		[7:0]	codec_6to3_gain;
reg		[7:0]	codec_7to3_gain;
reg		[7:0]	codec_8to3_gain;

reg		[7:0]	codec_1to4_gain;
reg		[7:0]	codec_2to4_gain;
reg		[7:0]	codec_3to4_gain;
reg		[7:0]	codec_4to4_gain;
reg		[7:0]	codec_5to4_gain;
reg		[7:0]	codec_6to4_gain;
reg		[7:0]	codec_7to4_gain;
reg		[7:0]	codec_8to4_gain;

reg		[7:0]	codec_1to5_gain;
reg		[7:0]	codec_2to5_gain;
reg		[7:0]	codec_3to5_gain;
reg		[7:0]	codec_4to5_gain;
reg		[7:0]	codec_5to5_gain;
reg		[7:0]	codec_6to5_gain;
reg		[7:0]	codec_7to5_gain;
reg		[7:0]	codec_8to5_gain;

reg		[7:0]	codec_1to6_gain;
reg		[7:0]	codec_2to6_gain;
reg		[7:0]	codec_3to6_gain;
reg		[7:0]	codec_4to6_gain;
reg		[7:0]	codec_5to6_gain;
reg		[7:0]	codec_6to6_gain;
reg		[7:0]	codec_7to6_gain;
reg		[7:0]	codec_8to6_gain;

reg		[7:0]	codec_1to7_gain;
reg		[7:0]	codec_2to7_gain;
reg		[7:0]	codec_3to7_gain;
reg		[7:0]	codec_4to7_gain;
reg		[7:0]	codec_5to7_gain;
reg		[7:0]	codec_6to7_gain;
reg		[7:0]	codec_7to7_gain;
reg		[7:0]	codec_8to7_gain;

reg		[7:0]	codec_1to8_gain;
reg		[7:0]	codec_2to8_gain;
reg		[7:0]	codec_3to8_gain;
reg		[7:0]	codec_4to8_gain;
reg		[7:0]	codec_5to8_gain;
reg		[7:0]	codec_6to8_gain;
reg		[7:0]	codec_7to8_gain;
reg		[7:0]	codec_8to8_gain;


//Parameter
localparam LD_1TO1_GAIN = 8'b1100_0000;
localparam LD_2TO1_GAIN = 8'b1100_0001;
localparam LD_3TO1_GAIN = 8'b1100_0010;
localparam LD_4TO1_GAIN = 8'b1100_0011;
localparam LD_5TO1_GAIN = 8'b1100_0100;
localparam LD_6TO1_GAIN = 8'b1100_0101;
localparam LD_7TO1_GAIN = 8'b1100_0110;
localparam LD_8TO1_GAIN = 8'b1100_0111;

localparam LD_1TO2_GAIN = 8'b1100_1000;
localparam LD_2TO2_GAIN = 8'b1100_1001;
localparam LD_3TO2_GAIN = 8'b1100_1010;
localparam LD_4TO2_GAIN = 8'b1100_1011;
localparam LD_5TO2_GAIN = 8'b1100_1100;
localparam LD_6TO2_GAIN = 8'b1100_1101;
localparam LD_7TO2_GAIN = 8'b1100_1110;
localparam LD_8TO2_GAIN = 8'b1100_1111;

localparam LD_1TO3_GAIN = 8'b1101_0000;
localparam LD_2TO3_GAIN = 8'b1101_0001;
localparam LD_3TO3_GAIN = 8'b1101_0010;
localparam LD_4TO3_GAIN = 8'b1101_0011;
localparam LD_5TO3_GAIN = 8'b1101_0100;
localparam LD_6TO3_GAIN = 8'b1101_0101;
localparam LD_7TO3_GAIN = 8'b1101_0110;
localparam LD_8TO3_GAIN = 8'b1101_0111;

localparam LD_1TO4_GAIN = 8'b1101_1000;
localparam LD_2TO4_GAIN = 8'b1101_1001;
localparam LD_3TO4_GAIN = 8'b1101_1010;
localparam LD_4TO4_GAIN = 8'b1101_1011;
localparam LD_5TO4_GAIN = 8'b1101_1100;
localparam LD_6TO4_GAIN = 8'b1101_1101;
localparam LD_7TO4_GAIN = 8'b1101_1110;
localparam LD_8TO4_GAIN = 8'b1101_1111;

localparam LD_1TO5_GAIN = 8'b1110_0000;
localparam LD_2TO5_GAIN = 8'b1110_0001;
localparam LD_3TO5_GAIN = 8'b1110_0010;
localparam LD_4TO5_GAIN = 8'b1110_0011;
localparam LD_5TO5_GAIN = 8'b1110_0100;
localparam LD_6TO5_GAIN = 8'b1110_0101;
localparam LD_7TO5_GAIN = 8'b1110_0110;
localparam LD_8TO5_GAIN = 8'b1110_0111;

localparam LD_1TO6_GAIN = 8'b1110_1000;
localparam LD_2TO6_GAIN = 8'b1110_1001;
localparam LD_3TO6_GAIN = 8'b1110_1010;
localparam LD_4TO6_GAIN = 8'b1110_1011;
localparam LD_5TO6_GAIN = 8'b1110_1100;
localparam LD_6TO6_GAIN = 8'b1110_1101;
localparam LD_7TO6_GAIN = 8'b1110_1110;
localparam LD_8TO6_GAIN = 8'b1110_1111;

localparam LD_1TO7_GAIN = 8'b1111_0000;
localparam LD_2TO7_GAIN = 8'b1111_0001;
localparam LD_3TO7_GAIN = 8'b1111_0010;
localparam LD_4TO7_GAIN = 8'b1111_0011;
localparam LD_5TO7_GAIN = 8'b1111_0100;
localparam LD_6TO7_GAIN = 8'b1111_0101;
localparam LD_7TO7_GAIN = 8'b1111_0110;
localparam LD_8TO7_GAIN = 8'b1111_0111;

localparam LD_1TO8_GAIN = 8'b1111_1000;
localparam LD_2TO8_GAIN = 8'b1111_1001;
localparam LD_3TO8_GAIN = 8'b1111_1010;
localparam LD_4TO8_GAIN = 8'b1111_1011;
localparam LD_5TO8_GAIN = 8'b1111_1100;
localparam LD_6TO8_GAIN = 8'b1111_1101;
localparam LD_7TO8_GAIN = 8'b1111_1110;
localparam LD_8TO8_GAIN = 8'b1111_1111;

//Net
wire	[15:0]	adder11,adder12,adder13,adder14,adder15,adder16,adder17,adder18;
wire	[15:0]	adder21,adder22,adder23,adder24,adder25,adder26,adder27,adder28;
wire	[15:0]	adder31,adder32,adder33,adder34,adder35,adder36,adder37,adder38;
wire	[15:0]	adder41,adder42,adder43,adder44,adder45,adder46,adder47,adder48;
wire	[15:0]	adder51,adder52,adder53,adder54,adder55,adder56,adder57,adder58;
wire	[15:0]	adder61,adder62,adder63,adder64,adder65,adder66,adder67,adder68;
wire	[15:0]	adder71,adder72,adder73,adder74,adder75,adder76,adder77,adder78;
wire	[15:0]	adder81,adder82,adder83,adder84,adder85,adder86,adder87,adder88;
wire	[15:0]	codec1_mixer,codec2_mixer,codec3_mixer,codec4_mixer,codec5_mixer,codec6_mixer,codec7_mixer,codec8_mixer;

always@(posedge clk or negedge nRst)
begin
	if(!nRst)
		ws_reg	<= 1'b0;
	else
		ws_reg <= ws;
end

always@(posedge clk or negedge nRst)
begin
	if(!nRst)
	begin
		codec1_out	<= 16'h0000;
		codec2_out	<= 16'h0000;
		codec3_out	<= 16'h0000;
		codec4_out	<= 16'h0000;	
	end
 	else if({ws_reg,ws} == 2'b01)		//load the right track
	begin
		codec1_out <= codec2_mixer;
		codec2_out <= codec4_mixer;
		codec3_out <= codec6_mixer;
		codec4_out <= codec8_mixer;
	end
	else if({ws_reg,ws} == 2'b10)		//load the left track
	begin
		codec1_out <= codec1_mixer;
		codec2_out <= codec3_mixer;
		codec3_out <= codec5_mixer;
		codec4_out <= codec7_mixer;
	end
end

always@(posedge clk or negedge nRst)
begin
	if(!nRst)
	begin
		cmd_r <= 8'h00;
		data_r <= 8'h00;
	end
	else if(wr_en)
	begin
		if(cmd_data_sw)			//command
			cmd_r <= data;
		else					//data
			data_r <= data;
	end		
end					
					

always@(posedge clk or negedge nRst)
begin
	if(!nRst)
	begin
		codec_1to1_gain <= 8'h80;
		codec_2to1_gain <= 8'h80;
		codec_3to1_gain <= 8'h80;
		codec_4to1_gain <= 8'h80;
		codec_5to1_gain <= 8'h80;
		codec_6to1_gain <= 8'h80;
		codec_7to1_gain <= 8'h80;
		codec_8to1_gain <= 8'h80;
		
		codec_1to2_gain <= 8'h80;
		codec_2to2_gain <= 8'h80;
		codec_3to2_gain <= 8'h80;
		codec_4to2_gain <= 8'h80;
		codec_5to2_gain <= 8'h80;
		codec_6to2_gain <= 8'h80;
		codec_7to2_gain <= 8'h80;
		codec_8to2_gain <= 8'h80;
		
		codec_1to3_gain <= 8'h80;
		codec_2to3_gain <= 8'h80;
		codec_3to3_gain <= 8'h80;
		codec_4to3_gain <= 8'h80;
		codec_5to3_gain <= 8'h80;
		codec_6to3_gain <= 8'h80;
		codec_7to3_gain <= 8'h80;
		codec_8to3_gain <= 8'h80;
		
		codec_1to4_gain <= 8'h80;
		codec_2to4_gain <= 8'h80;
		codec_3to4_gain <= 8'h80;
		codec_4to4_gain <= 8'h80;
		codec_5to4_gain <= 8'h80;
		codec_6to4_gain <= 8'h80;
		codec_7to4_gain <= 8'h80;
		codec_8to4_gain <= 8'h80;
		
		codec_1to5_gain <= 8'h80;
		codec_2to5_gain <= 8'h80;
		codec_3to5_gain <= 8'h80;
		codec_4to5_gain <= 8'h80;
		codec_5to5_gain <= 8'h80;
		codec_6to5_gain <= 8'h80;
		codec_7to5_gain <= 8'h80;
		codec_8to5_gain <= 8'h80;
		
		codec_1to6_gain <= 8'h80;
		codec_2to6_gain <= 8'h80;
		codec_3to6_gain <= 8'h80;
		codec_4to6_gain <= 8'h80;
		codec_5to6_gain <= 8'h80;
		codec_6to6_gain <= 8'h80;
		codec_7to6_gain <= 8'h80;
		codec_8to6_gain <= 8'h80;
		
		codec_1to7_gain <= 8'h80;
		codec_2to7_gain <= 8'h80;
		codec_3to7_gain <= 8'h80;
		codec_4to7_gain <= 8'h80;
		codec_5to7_gain <= 8'h80;
		codec_6to7_gain <= 8'h80;
		codec_7to7_gain <= 8'h80;
		codec_8to7_gain <= 8'h80;
		
		codec_1to8_gain <= 8'h80;
		codec_2to8_gain <= 8'h80;
		codec_3to8_gain <= 8'h80;
		codec_4to8_gain <= 8'h80;
		codec_5to8_gain <= 8'h80;
		codec_6to8_gain <= 8'h80;
		codec_7to8_gain <= 8'h80;
		codec_8to8_gain <= 8'h80;
		
	end
	else if(wr_en&(!cmd_data_sw))	//data-write operation
	begin
		case(cmd_r)
		//ch01
		LD_1TO1_GAIN:
				 codec_1to1_gain <= data;
		LD_2TO1_GAIN:
				codec_2to1_gain <= data;
		LD_3TO1_GAIN:
				codec_3to1_gain <= data;
		LD_4TO1_GAIN:
				codec_4to1_gain <= data;
		LD_5TO1_GAIN:
				codec_5to1_gain <= data;
		LD_6TO1_GAIN:
				codec_6to1_gain <= data;
		LD_7TO1_GAIN:
				codec_7to1_gain <= data;
		LD_8TO1_GAIN:
				codec_8to1_gain <= data;
		//ch02
		LD_1TO2_GAIN:
				 codec_1to2_gain <= data;
		LD_2TO2_GAIN:
				codec_2to2_gain <= data;
		LD_3TO2_GAIN:
				codec_3to2_gain <= data;
		LD_4TO2_GAIN:
				codec_4to2_gain <= data;
		LD_5TO2_GAIN:
				codec_5to2_gain <= data;
		LD_6TO2_GAIN:
				codec_6to2_gain <= data;
		LD_7TO2_GAIN:
				codec_7to2_gain <= data;
		LD_8TO2_GAIN:
				codec_8to2_gain <= data;
		//ch03
		LD_1TO3_GAIN:
				 codec_1to3_gain <= data;
		LD_2TO3_GAIN:
				codec_2to3_gain <= data;
		LD_3TO3_GAIN:
				codec_3to3_gain <= data;
		LD_4TO3_GAIN:
				codec_4to3_gain <= data;
		LD_5TO3_GAIN:
				codec_5to3_gain <= data;
		LD_6TO3_GAIN:
				codec_6to3_gain <= data;
		LD_7TO3_GAIN:
				codec_7to3_gain <= data;
		LD_8TO3_GAIN:
				codec_8to3_gain <= data;
		//ch04
		LD_1TO4_GAIN:
				 codec_1to4_gain <= data;
		LD_2TO4_GAIN:
				codec_2to4_gain <= data;
		LD_3TO4_GAIN:
				codec_3to4_gain <= data;
		LD_4TO4_GAIN:
				codec_4to4_gain <= data;
		LD_5TO4_GAIN:
				codec_5to4_gain <= data;
		LD_6TO4_GAIN:
				codec_6to4_gain <= data;
		LD_7TO4_GAIN:
				codec_7to4_gain <= data;
		LD_8TO4_GAIN:
				codec_8to4_gain <= data;
		//ch05
		LD_1TO5_GAIN:
				 codec_1to5_gain <= data;
		LD_2TO5_GAIN:
				codec_2to5_gain <= data;
		LD_3TO5_GAIN:
				codec_3to5_gain <= data;
		LD_4TO5_GAIN:
				codec_4to5_gain <= data;
		LD_5TO5_GAIN:
				codec_5to5_gain <= data;
		LD_6TO5_GAIN:
				codec_6to5_gain <= data;
		LD_7TO5_GAIN:
				codec_7to5_gain <= data;
		LD_8TO5_GAIN:
				codec_8to5_gain <= data;
		//ch06
		LD_1TO6_GAIN:
				 codec_1to6_gain <= data;
		LD_2TO6_GAIN:
				codec_2to6_gain <= data;
		LD_3TO6_GAIN:
				codec_3to6_gain <= data;
		LD_4TO6_GAIN:
				codec_4to6_gain <= data;
		LD_5TO6_GAIN:
				codec_5to6_gain <= data;
		LD_6TO6_GAIN:
				codec_6to6_gain <= data;
		LD_7TO6_GAIN:
				codec_7to6_gain <= data;
		LD_8TO6_GAIN:
				codec_8to6_gain <= data;
		//ch07
		LD_1TO7_GAIN:
				 codec_1to7_gain <= data;
		LD_2TO7_GAIN:
				codec_2to7_gain <= data;
		LD_3TO7_GAIN:
				codec_3to7_gain <= data;
		LD_4TO7_GAIN:
				codec_4to7_gain <= data;
		LD_5TO7_GAIN:
				codec_5to7_gain <= data;
		LD_6TO7_GAIN:
				codec_6to7_gain <= data;
		LD_7TO7_GAIN:
				codec_7to7_gain <= data;
		LD_8TO7_GAIN:
				codec_8to7_gain <= data;
		//ch08
		LD_1TO8_GAIN:
				 codec_1to8_gain <= data;
		LD_2TO8_GAIN:
				codec_2to8_gain <= data;
		LD_3TO8_GAIN:
				codec_3to8_gain <= data;
		LD_4TO8_GAIN:
				codec_4to8_gain <= data;
		LD_5TO8_GAIN:
				codec_5to8_gain <= data;
		LD_6TO8_GAIN:
				codec_6to8_gain <= data;
		LD_7TO8_GAIN:
				codec_7to8_gain <= data;
		LD_8TO8_GAIN:
				codec_8to8_gain <= data;
		
		default:;
		endcase
	end
end		
//	ch1*gain_1to1		
/* assign	adder11 = codec_1to1_gain[7]?({4'b0000,codec1_in[15:4]}):16'h0000;
assign	adder12 = codec_1to1_gain[6]?({5'b00000,codec1_in[15:5]}):16'h0000;
assign	adder13 = codec_1to1_gain[5]?({6'b000000,codec1_in[15:6]}):16'h0000;
assign	adder14 = codec_1to1_gain[4]?({7'b0000000,codec1_in[15:7]}):16'h0000;
assign	adder15 = codec_1to1_gain[3]?({8'b00000000,codec1_in[15:8]}):16'h0000;
assign	adder16 = codec_1to1_gain[2]?({9'b000000000,codec1_in[15:9]}):16'h0000;
assign	adder17 = codec_1to1_gain[1]?({10'b0000000000,codec1_in[15:10]}):16'h0000;
assign	adder18 = codec_1to1_gain[0]?({11'b00000000000,codec1_in[15:11]}):16'h0000; */

//ch1_mixout = ch1_in*gain11 + ch2_in*gain21 +...+ch8_in*gain81
mul_gain	mul_gain_ins11(.IN(codec1_in),.GAIN(codec_1to1_gain),.OUT(adder11));
mul_gain	mul_gain_ins12(.IN(codec2_in),.GAIN(codec_2to1_gain),.OUT(adder12));
mul_gain	mul_gain_ins13(.IN(codec3_in),.GAIN(codec_3to1_gain),.OUT(adder13));
mul_gain	mul_gain_ins14(.IN(codec4_in),.GAIN(codec_4to1_gain),.OUT(adder14));
mul_gain	mul_gain_ins15(.IN(codec5_in),.GAIN(codec_5to1_gain),.OUT(adder15));
mul_gain	mul_gain_ins16(.IN(codec6_in),.GAIN(codec_6to1_gain),.OUT(adder16));
mul_gain	mul_gain_ins17(.IN(codec7_in),.GAIN(codec_7to1_gain),.OUT(adder17));
mul_gain	mul_gain_ins18(.IN(codec8_in),.GAIN(codec_8to1_gain),.OUT(adder18));

mul_gain	mul_gain_ins21(.IN(codec1_in),.GAIN(codec_1to2_gain),.OUT(adder21));
mul_gain	mul_gain_ins22(.IN(codec2_in),.GAIN(codec_2to2_gain),.OUT(adder22));
mul_gain	mul_gain_ins23(.IN(codec3_in),.GAIN(codec_3to2_gain),.OUT(adder23));
mul_gain	mul_gain_ins24(.IN(codec4_in),.GAIN(codec_4to2_gain),.OUT(adder24));
mul_gain	mul_gain_ins25(.IN(codec5_in),.GAIN(codec_5to2_gain),.OUT(adder25));
mul_gain	mul_gain_ins26(.IN(codec6_in),.GAIN(codec_6to2_gain),.OUT(adder26));
mul_gain	mul_gain_ins27(.IN(codec7_in),.GAIN(codec_7to2_gain),.OUT(adder27));
mul_gain	mul_gain_ins28(.IN(codec8_in),.GAIN(codec_8to2_gain),.OUT(adder28));

mul_gain	mul_gain_ins31(.IN(codec1_in),.GAIN(codec_1to3_gain),.OUT(adder31));
mul_gain	mul_gain_ins32(.IN(codec2_in),.GAIN(codec_2to3_gain),.OUT(adder32));
mul_gain	mul_gain_ins33(.IN(codec3_in),.GAIN(codec_3to3_gain),.OUT(adder33));
mul_gain	mul_gain_ins34(.IN(codec4_in),.GAIN(codec_4to3_gain),.OUT(adder34));
mul_gain	mul_gain_ins35(.IN(codec5_in),.GAIN(codec_5to3_gain),.OUT(adder35));
mul_gain	mul_gain_ins36(.IN(codec6_in),.GAIN(codec_6to3_gain),.OUT(adder36));
mul_gain	mul_gain_ins37(.IN(codec7_in),.GAIN(codec_7to3_gain),.OUT(adder37));
mul_gain	mul_gain_ins38(.IN(codec8_in),.GAIN(codec_8to3_gain),.OUT(adder38));

mul_gain	mul_gain_ins41(.IN(codec1_in),.GAIN(codec_1to4_gain),.OUT(adder41));
mul_gain	mul_gain_ins42(.IN(codec2_in),.GAIN(codec_2to4_gain),.OUT(adder42));
mul_gain	mul_gain_ins43(.IN(codec3_in),.GAIN(codec_3to4_gain),.OUT(adder43));
mul_gain	mul_gain_ins44(.IN(codec4_in),.GAIN(codec_4to4_gain),.OUT(adder44));
mul_gain	mul_gain_ins45(.IN(codec5_in),.GAIN(codec_5to4_gain),.OUT(adder45));
mul_gain	mul_gain_ins46(.IN(codec6_in),.GAIN(codec_6to4_gain),.OUT(adder46));
mul_gain	mul_gain_ins47(.IN(codec7_in),.GAIN(codec_7to4_gain),.OUT(adder47));
mul_gain	mul_gain_ins48(.IN(codec8_in),.GAIN(codec_8to4_gain),.OUT(adder48));

mul_gain	mul_gain_ins51(.IN(codec1_in),.GAIN(codec_1to5_gain),.OUT(adder51));
mul_gain	mul_gain_ins52(.IN(codec2_in),.GAIN(codec_2to5_gain),.OUT(adder52));
mul_gain	mul_gain_ins53(.IN(codec3_in),.GAIN(codec_3to5_gain),.OUT(adder53));
mul_gain	mul_gain_ins54(.IN(codec4_in),.GAIN(codec_4to5_gain),.OUT(adder54));
mul_gain	mul_gain_ins55(.IN(codec5_in),.GAIN(codec_5to5_gain),.OUT(adder55));
mul_gain	mul_gain_ins56(.IN(codec6_in),.GAIN(codec_6to5_gain),.OUT(adder56));
mul_gain	mul_gain_ins57(.IN(codec7_in),.GAIN(codec_7to5_gain),.OUT(adder57));
mul_gain	mul_gain_ins58(.IN(codec8_in),.GAIN(codec_8to5_gain),.OUT(adder58));

mul_gain	mul_gain_ins61(.IN(codec1_in),.GAIN(codec_1to6_gain),.OUT(adder61));
mul_gain	mul_gain_ins62(.IN(codec2_in),.GAIN(codec_2to6_gain),.OUT(adder62));
mul_gain	mul_gain_ins63(.IN(codec3_in),.GAIN(codec_3to6_gain),.OUT(adder63));
mul_gain	mul_gain_ins64(.IN(codec4_in),.GAIN(codec_4to6_gain),.OUT(adder64));
mul_gain	mul_gain_ins65(.IN(codec5_in),.GAIN(codec_5to6_gain),.OUT(adder65));
mul_gain	mul_gain_ins66(.IN(codec6_in),.GAIN(codec_6to6_gain),.OUT(adder66));
mul_gain	mul_gain_ins67(.IN(codec7_in),.GAIN(codec_7to6_gain),.OUT(adder67));
mul_gain	mul_gain_ins68(.IN(codec8_in),.GAIN(codec_8to6_gain),.OUT(adder68));

mul_gain	mul_gain_ins71(.IN(codec1_in),.GAIN(codec_1to7_gain),.OUT(adder71));
mul_gain	mul_gain_ins72(.IN(codec2_in),.GAIN(codec_2to7_gain),.OUT(adder72));
mul_gain	mul_gain_ins73(.IN(codec3_in),.GAIN(codec_3to7_gain),.OUT(adder73));
mul_gain	mul_gain_ins74(.IN(codec4_in),.GAIN(codec_4to7_gain),.OUT(adder74));
mul_gain	mul_gain_ins75(.IN(codec5_in),.GAIN(codec_5to7_gain),.OUT(adder75));
mul_gain	mul_gain_ins76(.IN(codec6_in),.GAIN(codec_6to7_gain),.OUT(adder76));
mul_gain	mul_gain_ins77(.IN(codec7_in),.GAIN(codec_7to7_gain),.OUT(adder77));
mul_gain	mul_gain_ins78(.IN(codec8_in),.GAIN(codec_8to7_gain),.OUT(adder78));

mul_gain	mul_gain_ins81(.IN(codec1_in),.GAIN(codec_1to8_gain),.OUT(adder81));
mul_gain	mul_gain_ins82(.IN(codec2_in),.GAIN(codec_2to8_gain),.OUT(adder82));
mul_gain	mul_gain_ins83(.IN(codec3_in),.GAIN(codec_3to8_gain),.OUT(adder83));
mul_gain	mul_gain_ins84(.IN(codec4_in),.GAIN(codec_4to8_gain),.OUT(adder84));
mul_gain	mul_gain_ins85(.IN(codec5_in),.GAIN(codec_5to8_gain),.OUT(adder85));
mul_gain	mul_gain_ins86(.IN(codec6_in),.GAIN(codec_6to8_gain),.OUT(adder86));
mul_gain	mul_gain_ins87(.IN(codec7_in),.GAIN(codec_7to8_gain),.OUT(adder87));
mul_gain	mul_gain_ins88(.IN(codec8_in),.GAIN(codec_8to8_gain),.OUT(adder88));

adder16_8	adder16_8_ins1(.A1(adder11),.A2(adder12),.A3(adder13),.A4(adder14),.A5(adder15),.A6(adder16),.A7(adder17),.A8(adder18),.S(codec1_mixer));
adder16_8	adder16_8_ins2(.A1(adder21),.A2(adder22),.A3(adder23),.A4(adder24),.A5(adder25),.A6(adder26),.A7(adder27),.A8(adder28),.S(codec2_mixer));
adder16_8	adder16_8_ins3(.A1(adder31),.A2(adder32),.A3(adder33),.A4(adder34),.A5(adder35),.A6(adder36),.A7(adder37),.A8(adder38),.S(codec3_mixer));
adder16_8	adder16_8_ins4(.A1(adder41),.A2(adder42),.A3(adder43),.A4(adder44),.A5(adder45),.A6(adder46),.A7(adder47),.A8(adder48),.S(codec4_mixer));
adder16_8	adder16_8_ins5(.A1(adder51),.A2(adder52),.A3(adder53),.A4(adder54),.A5(adder55),.A6(adder56),.A7(adder57),.A8(adder58),.S(codec5_mixer));
adder16_8	adder16_8_ins6(.A1(adder61),.A2(adder62),.A3(adder63),.A4(adder64),.A5(adder65),.A6(adder66),.A7(adder67),.A8(adder68),.S(codec6_mixer));
adder16_8	adder16_8_ins7(.A1(adder71),.A2(adder72),.A3(adder73),.A4(adder74),.A5(adder75),.A6(adder76),.A7(adder77),.A8(adder78),.S(codec7_mixer));
adder16_8	adder16_8_ins8(.A1(adder81),.A2(adder82),.A3(adder83),.A4(adder84),.A5(adder85),.A6(adder86),.A7(adder87),.A8(adder88),.S(codec8_mixer));

			
endmodule

 
