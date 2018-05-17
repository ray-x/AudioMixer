

module top(
			clk,	//48MHz 
			nRst,
		
			//Audio
			ad_xclk,
			ad_ws,
			ad_bck,
			ad_di,
			ad_do,
			ad_l3md,
			ad_l3clk,
			ad_l3data,
			
			mcu_data,
			mcu_wr,
			mcu_cmd_data_sw,
			mcu_l3md,
			mcu_l3clk,
			mcu_l3data,
			mcu_cs1,
			mcu_cs0,		
			led	
			);
//port
input	clk;
input	nRst;
output	[3:0] ad_xclk;
output	[3:0] ad_ws;
output	[3:0] ad_bck;		
input	[3:0] ad_di;		
output	[3:0] ad_do;
output	[3:0] ad_l3md;
output	[3:0] ad_l3clk;
output	[3:0] ad_l3data;
input	[7:0]	mcu_data;
input 	mcu_wr;
input	mcu_cmd_data_sw;
input	mcu_l3md;
input	mcu_l3clk;
input	mcu_l3data;
input	mcu_cs1;
input	mcu_cs0;		
output	[2:0]	led;			
//inside
wire			CLK_48MHZ;
wire			CLK_24MHZ;
wire			ws;
wire			bck;
wire	[15:0]	data_out0;
wire	[15:0]	data_out1;
wire	[15:0]	data_out2;
wire	[15:0]	data_out3;

reg		[15:0]	data_left0;	
reg		[15:0]	data_left1;	
reg		[15:0]	data_left2;	
reg		[15:0]	data_left3;	
	
reg		[15:0]	data_right0;
reg		[15:0]	data_right1;
reg		[15:0]	data_right2;
reg		[15:0]	data_right3;

reg		[15:0]	data_left_buf0;	
reg		[15:0]	data_left_buf1;	
reg		[15:0]	data_left_buf2;	
reg		[15:0]	data_left_buf3;
	
reg		[15:0]	data_right_buf0;
reg		[15:0]	data_right_buf1;
reg		[15:0]	data_right_buf2;
reg		[15:0]	data_right_buf3;

wire	[15:0]	mixer_data0;		
wire	[15:0]	mixer_data1;		
wire	[15:0]	mixer_data2;		
wire	[15:0]	mixer_data3;		
reg				bck_reg;
reg				ws_reg;			
reg		[7:0] 	mcu_data_r;

clk_gen		clk_gen_ins(
					   .CLKIN_IN(clk), 
					   .RST_IN(~nRst), 
					   .CLKDV_OUT(CLK_24MHZ), 
					   //.CLKIN_IBUFG_OUT, 
					   .CLK0_OUT(CLK_48MHZ) 
					   //.LOCKED_OUT
					   );
ws_bck_gen	ws_bck_gen_ins(
							.clk(CLK_24MHZ),
							.nRst(nRst),
							.ws(ws),					//word select 1:left 0:right
							.bck(bck)					//serial bit clock			
							);					   
		
channel_rx	channel_rx_ins0(		
						.clk(CLK_24MHZ),
						.nRst(nRst),			
						//.ws(ws),						//word select
						.bck(bck),						//bit clock
						.data_out(data_out0),			//16bit data out
						.data_bit_rx(ad_di[0])			//serial data in
						);
						
channel_tx	channel_tx_ins0(
						.clk(CLK_24MHZ),
						.nRst(nRst),
						.ws(ws),
						.bck(bck),
						.data_in(mixer_data0),
						.data_bit_tx(ad_do[0])
						);
						
channel_rx	channel_rx_ins1(		
						.clk(CLK_24MHZ),
						.nRst(nRst),			
						//.ws(ws),						//word select
						.bck(bck),						//bit clock
						.data_out(data_out1),			//16bit data out
						.data_bit_rx(ad_di[1])			//serial data in
						);
						
channel_tx	channel_tx_ins1(
						.clk(CLK_24MHZ),
						.nRst(nRst),
						.ws(ws),
						.bck(bck),
						.data_in(mixer_data1),
						.data_bit_tx(ad_do[1])
						);

channel_rx	channel_rx_ins2(		
						.clk(CLK_24MHZ),
						.nRst(nRst),			
						//.ws(ws),						//word select
						.bck(bck),						//bit clock
						.data_out(data_out2),			//16bit data out
						.data_bit_rx(ad_di[2])			//serial data in
						);
						
channel_tx	channel_tx_ins2(
						.clk(CLK_24MHZ),
						.nRst(nRst),
						.ws(ws),
						.bck(bck),
						.data_in(mixer_data2),
						.data_bit_tx(ad_do[2])
						);
						
channel_rx	channel_rx_ins3(		
						.clk(CLK_24MHZ),
						.nRst(nRst),			
						//.ws(ws),						//word select
						.bck(bck),						//bit clock
						.data_out(data_out3),			//16bit data out
						.data_bit_rx(ad_di[3])			//serial data in
						);
						
channel_tx	channel_tx_ins3(
						.clk(CLK_24MHZ),
						.nRst(nRst),
						.ws(ws),
						.bck(bck),
						.data_in(mixer_data3),
						.data_bit_tx(ad_do[3])
						);
						
cmd_decoder	cmd_decoder_ins		
						(
						.clk(CLK_24MHZ),
						.nRst(nRst),
						.wr_en(mcu_wr),					//1:write enable 0:write disable
						.cmd_data_sw(mcu_cmd_data_sw),	//1:command 0:data
						.data(mcu_data),
						.ws(ws),
						.codec1_in(data_left_buf0),
						.codec2_in(data_right_buf0),
						.codec3_in(data_left_buf1),
						.codec4_in(data_right_buf1),
						.codec5_in(data_left_buf2),
						.codec6_in(data_right_buf2),
						.codec7_in(data_left_buf3),
						.codec8_in(data_right_buf3),
						.codec1_out(mixer_data0),
						.codec2_out(mixer_data1),
						.codec3_out(mixer_data2),
						.codec4_out(mixer_data3)
						);

always@(posedge CLK_24MHZ or negedge nRst)
begin
	if(!nRst)
		ws_reg <= 1'b0;
	else
		ws_reg <= ws;
end

always@(posedge CLK_24MHZ or negedge nRst)
begin
	if(!nRst)
		bck_reg <= 1'b0;
	else
		bck_reg <= bck;
end

//left track data buffer 						
always@(posedge CLK_24MHZ or negedge nRst)
begin
	if(!nRst)
	begin
		data_left0 <= 16'h0000;
		data_left1 <= 16'h0000;
		data_left2 <= 16'h0000;
		data_left3 <= 16'h0000;
	end
	else if({ws_reg,ws} == 2'b01)
	begin
		data_left0 <= data_out0;
		data_left1 <= data_out1;
		data_left2 <= data_out2;
		data_left3 <= data_out3;
	end	
end

//right track data buffer 	
always@(posedge CLK_24MHZ or negedge nRst)
begin
	if(!nRst)
	begin
		data_right0 <= 16'h0000;
		data_right1 <= 16'h0000;
		data_right2 <= 16'h0000;
		data_right3 <= 16'h0000;
	end
	else if({ws_reg,ws} == 2'b10)
	begin
		data_right0 <= data_out0;
		data_right1 <= data_out1;
		data_right2 <= data_out2;
		data_right3 <= data_out3;
	end	
end

//mixer data buffer
always@(posedge CLK_24MHZ or negedge nRst)
begin
	if(!nRst)
	begin	
		data_left_buf0 <= 16'h0000;
		data_right_buf0 <= 16'h0000;
		data_left_buf1 <= 16'h0000;
		data_right_buf1 <= 16'h0000;
		data_left_buf2 <= 16'h0000;
		data_right_buf2 <= 16'h0000;
		data_left_buf3 <= 16'h0000;
		data_right_buf3 <= 16'h0000;
	end
	else if({bck_reg,bck} == 2'b01)
	begin
		data_left_buf0 <= data_left0;
		data_right_buf0 <= data_right0;
		data_left_buf1 <= data_left1;
		data_right_buf1 <= data_right1;
		data_left_buf2 <= data_left2;
		data_right_buf2 <= data_right2;
		data_left_buf3 <= data_left3;
		data_right_buf3 <= data_right3;
	end	
end

always@(posedge CLK_48MHZ)
	mcu_data_r <= mcu_data;

/* always@(*)
begin
	case(mcu_data_r)
	8'h00: led = 3'b000;
	8'h01: led = 3'b001;
	8'h02: led = 3'b010;
	8'h04: led = 3'b011;
	8'h08: led = 3'b100;
	8'h10: led = 3'b101;
	8'h20: led = 3'b110;
	8'h40: led = 3'b111;
	default: led = 3'b000;
	endcase
end */
assign led[0] = ad_di;
assign led[1] = ad_di;
assign led[2] = ad_di;


// L3-interface 
assign ad_l3md[0] = ({mcu_cs1,mcu_cs0} == 2'b00)?mcu_l3md:2'b11;
assign ad_l3clk[0] = ({mcu_cs1,mcu_cs0} == 2'b00)?mcu_l3clk:2'b11;
assign ad_l3data[0] = ({mcu_cs1,mcu_cs0} == 2'b00)?mcu_l3data:2'b11;

assign ad_l3md[1] = ({mcu_cs1,mcu_cs0} == 2'b01)?mcu_l3md:2'b11;
assign ad_l3clk[1] = ({mcu_cs1,mcu_cs0} == 2'b01)?mcu_l3clk:2'b11;
assign ad_l3data[1] = ({mcu_cs1,mcu_cs0} == 2'b01)?mcu_l3data:2'b11;

assign ad_l3md[2] = ({mcu_cs1,mcu_cs0} == 2'b10)?mcu_l3md:2'b11;
assign ad_l3clk[2] = ({mcu_cs1,mcu_cs0} == 2'b10)?mcu_l3clk:2'b11;
assign ad_l3data[2] = ({mcu_cs1,mcu_cs0} == 2'b10)?mcu_l3data:2'b11;

assign ad_l3md[3] = ({mcu_cs1,mcu_cs0} == 2'b11)?mcu_l3md:2'b11;
assign ad_l3clk[3] = ({mcu_cs1,mcu_cs0} == 2'b11)?mcu_l3clk:2'b11;
assign ad_l3data[3] = ({mcu_cs1,mcu_cs0} == 2'b11)?mcu_l3data:2'b11;

// digital interface
assign ad_xclk[0] = CLK_24MHZ;
assign ad_ws[0] = ws;
assign ad_bck[0] = bck;

assign ad_xclk[1] = CLK_24MHZ;
assign ad_ws[1] = ws;
assign ad_bck[1] = bck;

assign ad_xclk[2] = CLK_24MHZ;
assign ad_ws[2] = ws;
assign ad_bck[2] = bck;

assign ad_xclk[3] = CLK_24MHZ;
assign ad_ws[3] = ws;
assign ad_bck[3] = bck;		
			
endmodule 
