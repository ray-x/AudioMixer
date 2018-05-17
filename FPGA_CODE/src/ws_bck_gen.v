// --------------------------------------------------------------------
// Copyright (c) 2009 by *****  
// --------------------------------------------------------------------
// Module name: ws_bck_gen
// Description:
// this module can receive two channels of mono audio signal
// IC:UDA1341TS
// --------------------------------------------------------------------
//	 	Project name:	audio_mixer
// 		File name: ws_bck_gen.v
// 		Create date: 2009-11-19
// 		Author: Binjie Tu
// --------------------------------------------------------------------
// Revision History:
// --------------------------------------------------------------------

module ws_bck_gen(
					clk,
					nRst,
					ws,		//word select 1:left 0:right
					bck		//serial bit clock			
					);

		
//port					
input	clk;
input	nRst;
output	ws;
output	bck;

parameter	CNT_WS_WIDTH = 6;				
parameter	CNT_BCK_WIDTH = 11;
parameter	DIV_VALUE = 12;//48000/44/2; //48MHz divided by 48000/44,got the bck 44kHz 
	
	
//inside signal or reg
reg	[4:0] cnt_ws;
reg	[3:0]	cnt_bck;
reg	bck_reg;
reg	ws_reg;

	
//generate bck and ws
always@(posedge clk or negedge nRst)
begin
	if(!nRst)
	begin
		cnt_bck <= 4'b0000;
		cnt_ws  <= 5'b0_0000;
		bck_reg	<= 1'b0;
		ws_reg <= 1'b1;
	end
	else 
	begin
		if(cnt_bck < 4'b0111)//12
			cnt_bck[3:0] <= cnt_bck[3:0] + 4'b0001;
		else
		begin
			cnt_bck <= 4'b0000;
			bck_reg <= ~bck_reg;
			if(bck_reg == 1'b1)
			begin
				if(cnt_ws < 5'b0_1111)	//N-1 = 24 - 1 = 23
					cnt_ws[4:0] <= cnt_ws[4:0] + 5'b0_0001;
				else
				begin
					cnt_ws <= 5'b0_0000;
					ws_reg <= ~ws_reg;
				end
			end
		end
			
	end
	
	
end


/* always@(posedge clk or negedge nRst)
begin
	if(!nRst)
	begin
		cnt_ws <= 6'b0;
	end
	else if({bck_reg2,bck_reg} == 2'b10)
	begin
		if(cnt_ws < 6'h17)	//N-1 = 24 - 1 = 23
			cnt_ws[CNT_WS_WIDTH-1:0] <= cnt_ws[CNT_WS_WIDTH-1:0] + 6'b1;
		else
		begin
			cnt_ws <= 6'b0;
			ws_reg <= ~ws_reg;
			//cnt_ws[CNT_WS_WIDTH-1:0] <= cnt_ws[CNT_WS_WIDTH-1:0] + 7'b1; 
		end
			
	end	
end
 */
assign ws = ws_reg;		//f_ws = f_bck/32
assign bck = bck_reg;	

endmodule
