// --------------------------------------------------------------------
// Copyright (c) 2009 by *****  
// --------------------------------------------------------------------
// Module name: channel_rx
// Description:
// this module can tx two channels of mono audio signal
// IC:UDA1341TS
// --------------------------------------------------------------------
//	 	Project name:	audio_mixer
// 		File name: channel_tx.v
// 		Create date: 2009-12-29
// 		Author: Binjie Tu
// --------------------------------------------------------------------
// Revision History:
// --------------------------------------------------------------------

module channel_tx(
			clk,
			nRst,			
			ws,
			bck,
			data_in,
			data_bit_tx
		);
//Port		
input	clk;
input	nRst;		
input	ws;
input	bck;
input	[15:0] data_in;
output	data_bit_tx;

//inside signal
reg		[3:0] bck_reg;
reg		ws_reg;
reg		[16:0] data_reg;

always@(posedge clk or negedge nRst)
begin
	if(!nRst)
		bck_reg <= 4'b0000;
	else
		bck_reg <= {bck_reg[2:0],bck};
end

always@(posedge clk or negedge nRst)
begin
	if(!nRst)
		ws_reg <= 1'b0;
	else
		ws_reg <= ws;
end
/*
always@(posedge clk or negedge nRst)
begin
	if(!nRst)
		data_reg <= 25'h0000000;
	else if({ws_reg,ws} == 2'b01 || {ws_reg,ws} == 2'b10)
		data_reg <= {9'b0_0000_0000,data_in};						//At the ws edge load the data for output. 
	else if(bck_reg == 4'b1000)						//detect bck negedge and then delay 3 clk.
													//since bck negedge and ws edge at the same time.  
		data_reg <= {data_reg[23:0],1'b0};			//setup the data
		
end

assign	data_bit_tx = data_reg[24];
*/
always@(posedge clk or negedge nRst)
begin
	if(!nRst)
		data_reg <= 17'h0000000;
	else if({ws_reg,ws} == 2'b01 || {ws_reg,ws} == 2'b10)
		data_reg <= {1'b0,data_in};						//At the ws edge load the data for output. 
	else if(bck_reg == 4'b1000)						//detect bck negedge and then delay 3 clk.
													//since bck negedge and ws edge at the same time.  
		data_reg <= {data_reg[15:0],1'b0};			//setup the data
		
end

assign	data_bit_tx = data_reg[16];


endmodule 

