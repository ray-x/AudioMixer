// --------------------------------------------------------------------
// Copyright (c) 2009 by *****  
// --------------------------------------------------------------------
// Module name: channel_rx
// Description:
// this module can receive two channels of mono audio signal
// IC:UDA1341TS
// --------------------------------------------------------------------
//	 	Project name:	audio_mixer
// 		File name: channel_rx.v
// 		Create date: 2009-11-19
// 		Author: Binjie Tu
// --------------------------------------------------------------------
// Revision History:
// --------------------------------------------------------------------

module channel_rx(
					//system
					clk,
					nRst,		//
					//UDA1341 digital signal interface
					//ws,					//word select
					bck,				//bit clock
					data_out,			//16bit data out
					data_bit_rx			//serial data in
				);
input	clk;
input	nRst;
//input	ws;
input	bck;
output	[15:0]	data_out;
input	data_bit_rx;

//reg		ws_reg;
//reg		ws_posedge;		//left channel 
//reg		ws_negedge;		//right channel
reg		bck_reg;
//reg		bck_posedge;	//bck posedge
reg		[15:0]	data_reg;
/*
always@(posedge clk or negedge nRst)
begin
	if(!nRst)
		ws_reg <= 1'b0;
	else
		ws_reg <= ws;
end
*/

always@(posedge clk or negedge nRst)
begin
	if(!nRst)
		bck_reg <= 1'b0;
	else
		bck_reg <= bck;
end

//assign ws_posedge = (ws_reg == 1'b0) && (ws == 1'b1);
//assign ws_negedge = (ws_reg == 1'b1) && (ws == 1'b0);	
//assign bck_posedge = (bck_reg == 1'b0) && (bck == 1'b1);
assign	data_out = data_reg[15:0];
always@(posedge clk or negedge nRst)
begin
	if(!nRst)
		data_reg <= 16'h0000;		
	else if((bck_reg == 1'b0) && (bck == 1'b1))		//bck posedge latch the bit
		data_reg <= {data_reg[14:0],data_bit_rx};		
end

endmodule
