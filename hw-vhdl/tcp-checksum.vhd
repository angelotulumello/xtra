-- ----------------------------------------------------------------------------
--                             Entity declaration
-- ----------------------------------------------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all; 

Library UNISIM;
use UNISIM.vcomponents.all;


entity tcp_checksum is 
    generic (
-- Parameters of AxiStream Slave Bus Interface S00_AXIS
                C_S00_AXIS_DATA_WIDTH  : integer   := 256;
                C_S00_AXIS_TUSER_WIDTH : integer   := 128;
-- Parameters of AxiStream Master Bus Interface M00_AXIS
                C_M00_AXIS_DATA_WIDTH  : integer   := 256;
                C_M00_AXIS_TUSER_WIDTH : integer   := 128;
-- Parameters of Axi Slave Bus Interface S00_AXIS
                C_S00_AXI_DATA_WIDTH  : integer   := 32;
                C_S00_AXI_ADDR_WIDTH  : integer   := 12;
                C_BASEADDR  : std_logic_vector(31 downto 0)   := x"90000000";
                C_HIGHADDR  : std_logic_vector(31 downto 0)   := x"90000FFF"
            );
    port (
-- Ports of Axi Master Bus Interface M00_AXIS

-- Global ports
             M0_AXIS_ACLK : in std_logic;
             M0_AXIS_ARESETN  : in std_logic;

-- Master Stream Ports.
             M0_AXIS_TVALID   : out std_logic;
             M0_AXIS_TDATA    : out std_logic_vector(C_M00_AXIS_DATA_WIDTH-1 downto 0);
             M0_AXIS_TKEEP    : out std_logic_vector((C_M00_AXIS_DATA_WIDTH/8)-1 downto 0);
             M0_AXIS_TUSER    : out std_logic_vector(C_M00_AXIS_TUSER_WIDTH-1 downto 0);
             M0_AXIS_TLAST    : out std_logic;
             M0_AXIS_TREADY   : in std_logic;
             
-- Ports of Axi Stream Slave Bus Interface S00_AXIS

--             S0_AXIS_ACLK    : in std_logic;
--             S0_AXIS_ARESETN : in std_logic;
             S0_AXIS_TVALID  : in std_logic;
             S0_AXIS_TDATA   : in std_logic_vector(C_S00_AXIS_DATA_WIDTH-1 downto 0);
             S0_AXIS_TKEEP   : in std_logic_vector((C_S00_AXIS_DATA_WIDTH/8)-1 downto 0);
             S0_AXIS_TUSER   : in std_logic_vector(C_S00_AXIS_TUSER_WIDTH-1 downto 0);
             S0_AXIS_TLAST   : in std_logic;
             S0_AXIS_TREADY  : out std_logic

-- Ports of Axi Slave Bus Interface S_AXI
--             S_AXI_ACLK	    : in std_logic;  
--             S_AXI_ARESETN	: in std_logic;                                     
--             S_AXI_AWADDR	: in std_logic_vector(C_S00_AXI_ADDR_WIDTH-1 downto 0);     
--             S_AXI_AWVALID	: in std_logic; 
--             S_AXI_WDATA 	: in std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0); 
--             S_AXI_WSTRB 	: in std_logic_vector(C_S00_AXI_DATA_WIDTH/8-1 downto 0);   
--             S_AXI_WVALID	: in std_logic;                                    
--             S_AXI_BREADY	: in std_logic;                                    
--             S_AXI_ARADDR	: in std_logic_vector(C_S00_AXI_ADDR_WIDTH-1 downto 0);
--             S_AXI_ARVALID	: in std_logic;                                     
--             S_AXI_RREADY	: in std_logic;                                     
--             S_AXI_ARREADY	: out std_logic;             
--             S_AXI_RDATA	: out std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
--             S_AXI_RRESP	: out std_logic_vector(1 downto 0);
--             S_AXI_RVALID	: out std_logic;                                   
--             S_AXI_WREADY	: out std_logic; 
--             S_AXI_BRESP	: out std_logic_vector(1 downto 0);                         
--             S_AXI_BVALID	: out std_logic;                                    
--             S_AXI_AWREADY  : out std_logic 
         );
         
	 attribute max_fanout:integer;
	 attribute max_fanout of S0_AXIS_TDATA : signal is 5;           
         
         attribute core_generation_info : string;
         attribute core_generation_info of tcp_checksum : entity is "tcp_checksum{x_ipProduct=Vivado 2016.4,x_ipVendor=cnit.it}";
end entity;

architecture full of tcp_checksum is

  component  fallthrough_small_fifo is
          generic (
                       WIDTH : integer;
                       MAX_DEPTH_BITS : integer
               );
          PORT (
               din: in std_logic_vector(WIDTH-1 downto 0);     -- Data in
               wr_en: in std_logic;   --Write enable
               rd_en: in std_logic;   --Read the next word
               dout: out std_logic_vector(WIDTH-1 downto 0);    -- Data out
               full: out std_logic;
               nearly_full: out std_logic;               
               prog_full: out std_logic;               
               empty: out std_logic;               
               reset:in std_logic;
               clk:in std_logic
            );
          end component;


                                 
function ntoh16 (I: std_logic_vector(15 downto 0)) return std_logic_vector is
            begin
            return (I(7 downto 0) & I(15 downto 8));
            end function ntoh16;



-- calcola il checksum di un vettore di 16*n bits
function compute_chk(A: std_logic_vector) return  std_logic_vector is
variable B : std_logic_vector(A'length-1 downto 0);
variable temp1 : std_logic_vector(31 downto 0):=x"00000000";
variable temp2 : std_logic_vector(31 downto 0);
variable x,y : std_logic_vector(31 downto 0);
variable i: integer:=0;
begin 

--A'ascending TRUE se To, FALSE se DOWNTO 
--    if (A'ascending) then
--	   B:=A;
--    else
--	   B:=A;
--    end if;
    B:=A;    
    for i in 0 to (B'length/16-1) loop
	temp1:= temp1 + (x"0000"& ntoh16(B(16*i+15 downto 16*i))); 
    end loop;
    x := temp1(15 downto 0) & temp1(31 downto 16);
    y := temp1;
    temp2  := x+y;
    return temp2(31 downto 16);
end function compute_chk;

 function reduce_chk(A: std_logic_vector) return  std_logic_vector is
    variable result : std_logic_vector(15 downto 0);
    begin    
    result:=A;
	if (A /=x"0000") then
	    result := not(A);
	end if;
	return result;
end function reduce_chk;


    signal RESETN,RESET   : std_logic;

-- ----------------------------------------------------------------------------
-- signals for AXI Lite
-- ----------------------------------------------------------------------------

--type axi_states is (addr_wait, read_state, write_state, response_state);
    signal axi_state :std_logic_vector(2 downto 0);
    signal address : std_logic_vector(C_S00_AXI_ADDR_WIDTH-1 downto 0);
    signal write_enable: std_logic;
    signal read_enable: std_logic;
    signal int_S_AXI_BVALID: std_logic;

-- ----------------------------------------------------------------------------
-- signals for ETH decoding 
-- ----------------------------------------------------------------------------

    type pkt_states is (IDLE, PKT1,PKT2);

    signal curr_state, next_state : pkt_states;
    signal out_curr_state, out_next_state : pkt_states;
    signal step: boolean; 
    
-- ----------------------------------------------------------------------------
-- signals for packet classification 
-- ----------------------------------------------------------------------------
    signal is_IP,is_UDP,is_TCP: boolean;
    signal out_is_IP,out_is_UDP,out_is_TCP: boolean;
    signal src_if: std_logic_vector(7 downto 0);
    signal dst_if: std_logic_vector(7 downto 0);
    signal pkt_len: std_logic_vector(15 downto 0);
    signal src_mac,dst_mac: std_logic_vector(47 downto 0);
    signal src_ip,dst_ip,seq_num,ack_num: std_logic_vector(31 downto 0);
    signal src_port,dst_port: std_logic_vector(15 downto 0);
    
-- ----------------------------------------------------------------------------
-- stats  
-- ----------------------------------------------------------------------------
    signal ip_count,tcp_count,udp_count:std_logic_vector(31 downto 0);
    signal  release_number,release_date: std_logic_vector(31 downto 0);
    signal  reg_test0: std_logic_vector(31 downto 0);
    
    signal fifo_din,fifo_dout   : std_logic_vector(416 downto 0); 
    signal pkt   : std_logic_vector(7 downto 0); 
    signal srd_en,sstamp,TFIRST,rd_en,empty,nearly_full: std_logic;

    


-- ----------------------------------------------------------------------------
-- checksum signals 
-- ----------------------------------------------------------------------------
    signal chk_in,chk_in1,chk_in2,out_tcp_checksum,ip_len,new_tcp_checksum: std_logic_vector(15 downto 0);
    signal new_tcp_checksum1,new_tcp_checksum2: std_logic_vector(15 downto 0);
    signal out_tcp_checksum32  : std_logic_vector(31 downto 0);
    
    
    
    signal empty_checksum,stamp: std_logic;
    signal sflush,flush: std_logic;
    
    signal masked_TDATA: std_logic_vector(C_M00_AXIS_DATA_WIDTH-1 downto 0);
    signal int_M0_AXIS_TDATA: std_logic_vector(C_M00_AXIS_DATA_WIDTH-1 downto 0);
    signal int_M0_AXIS_TLAST: std_logic;
    

    attribute X_INTERFACE_INFO: string;
    attribute X_INTERFACE_INFO of M0_AXIS_ACLK: signal is "xilinx.com:signal:clock:1.0 M0_AXIS_ACLK CLK";
    attribute X_INTERFACE_PARAMETER: string;
    attribute X_INTERFACE_PARAMETER of M0_AXIS_ACLK: signal is "ASSOCIATED_BUSIF M0_AXIS:S0_AXIS";                     
    --attribute max_fanout:integer;

    attribute mark_debug: string;
    attribute keep:string;
    attribute max_fanout of new_tcp_checksum: signal is 2;           
    attribute max_fanout of new_tcp_checksum1: signal is 2;           
    attribute max_fanout of new_tcp_checksum2: signal is 2;           




-- ----------------------------------------------------------------------------
--                             Architecture body
-- ----------------------------------------------------------------------------

begin


    RESETN<=M0_AXIS_ARESETN;
    RESET<=not (M0_AXIS_ARESETN);

-- -------------------------------------------------------------------------
--
-- process to detect if the next word is available
--
-- -------------------------------------------------------------------------

    step_assign: step<= true when (S0_AXIS_TVALID='1') and (nearly_full = '0') else false;

-- -------------------------------------------------------------------------
--
-- extract Header ETH-IP-TCP/UDP
--
-- -------------------------------------------------------------------------

    process(M0_AXIS_ACLK, M0_AXIS_ARESETN)
    --variable tcp_header_len: std_logic_vector(7 downto 0);
    --variable ip_len: std_logic_vector(15 downto 0);
    --variable tcp_len: std_logic_vector(15 downto 0);
    variable old_tcp_checksum :  std_logic_vector(15 downto 0);
    variable first_tcp_checksum :  std_logic_vector(15 downto 0);
    begin
        if (M0_AXIS_ACLK'event and M0_AXIS_ACLK = '1') then
            if (M0_AXIS_ARESETN = '0') then
                curr_state <= IDLE;
                ip_count<= (others => '0');
                udp_count<= (others => '0');
                tcp_count<= (others => '0');
                src_if <= (others => '0');
		ip_len <= (others => '0');
                
            else              
                src_if<= (others => '0');
                case curr_state is
                    when IDLE =>
                    --out_tcp_checksum<=new_tcp_checksum;
                        if (step) then
                            curr_state <= PKT1;
                            pkt_len <= S0_AXIS_TUSER(15 downto 0);
			    dst_if <= S0_AXIS_TUSER(31 downto 24);
                            src_if <= S0_AXIS_TUSER(23 downto 16);
                            dst_mac <= S0_AXIS_TDATA(47 downto 0);
                            src_mac <= S0_AXIS_TDATA(95 downto 48);
                            src_ip   <= (others => '0');
                            dst_ip   <= (others => '0');
                            is_ip<=false;
                            is_UDP<=false;
                            is_TCP<=false;
                            if (S0_AXIS_TDATA(111 downto 96)=x"0008") then --> 0x0800 reversed
                                ip_count <= ip_count+1;
				is_ip<=true;
                                if (S0_AXIS_TDATA(191 downto 184)=x"11") then --FIXME does not check eth header length
                                    udp_count <= udp_count+1;
                                    is_UDP<=true;
                                end if;
                                if (S0_AXIS_TDATA(191 downto 184)=x"06") then --FIXME does not check eth header lenght
                                    tcp_count <= tcp_count+1;
                                    is_TCP<=true;
                                end if;
                                ip_len <= ntoh16(S0_AXIS_TDATA(143 downto 128));
                                src_ip <= S0_AXIS_TDATA(239 downto 208);
                                dst_ip(15 downto 0) <=S0_AXIS_TDATA(255 downto 240);
                                --                                                                    (x"0006" -x"0014") & ip_len
                                first_tcp_checksum :=ntoh16(compute_chk(S0_AXIS_TDATA(255 downto 208) & x"F1FF" & S0_AXIS_TDATA(143 downto 128)));
                            end if; --IS_IP 
                        end if;
                    when PKT1 =>
                        if (step) then
                            dst_ip(31 downto 16) <=S0_AXIS_TDATA(15 downto 0);
			    old_tcp_checksum := not S0_AXIS_TDATA(159 downto 144);
                            --tcp_header_len:= "00" & S0_AXIS_TDATA(119 downto 116) & "00";
                            --tcp_len:=(ip_len + x"0006" -x"0014");
                            --new_tcp_checksum <=ntoh16(compute_chk(ntoh16(tcp_len) & new_tcp_checksum & S0_AXIS_TDATA ));
                            --new_tcp_checksum <=ntoh16(compute_chk(first_tcp_checksum &  S0_AXIS_TDATA(255 downto 160) & S0_AXIS_TDATA(143 downto 0)));
                            new_tcp_checksum  <=ntoh16(compute_chk(first_tcp_checksum &  S0_AXIS_TDATA & old_tcp_checksum ));
                            new_tcp_checksum1 <=ntoh16(compute_chk(S0_AXIS_TDATA(127 downto 0)));
                            new_tcp_checksum2 <=ntoh16(compute_chk(S0_AXIS_TDATA(255 downto 160) & S0_AXIS_TDATA(143 downto 128) & first_tcp_checksum ));
                                                        
                            if (is_TCP or is_UDP) then
                                src_port <= S0_AXIS_TDATA(31 downto 16);
                                dst_port <= S0_AXIS_TDATA(47 downto 32);
                            end if;
			    curr_state <= PKT2;
                        end if;
                    when PKT2 =>
                        if (step) then
                            new_tcp_checksum <=ntoh16(compute_chk(new_tcp_checksum & S0_AXIS_TDATA ));
                            new_tcp_checksum1 <=ntoh16(compute_chk(new_tcp_checksum1 & S0_AXIS_TDATA(127 downto 0)   ));
                            new_tcp_checksum2 <=ntoh16(compute_chk(new_tcp_checksum2 & S0_AXIS_TDATA(255 downto 128) ));
                            if ( S0_AXIS_TLAST='1') then
                                    curr_state <= IDLE;
                            end if;
                        end if;
                end case;
            end if;
        end if;
    end process;


    
process(M0_AXIS_ACLK, M0_AXIS_ARESETN)
    begin
        if (M0_AXIS_ACLK'event and M0_AXIS_ACLK = '1') then
            if (M0_AXIS_ARESETN = '0') then
                out_curr_state <= IDLE;
                --flush <='0';
                sflush<='0';
                --stamp <='0';
                sstamp<='0';
                --rd_en<='0';
                srd_en<='0';
            else
                --stamp<='0'; 
                sstamp<=stamp;
                --flush<='0';
		sflush<=flush;
                
                srd_en<= rd_en;
                
                 
                case out_curr_state is
                    when IDLE =>
                        if ((M0_AXIS_TREADY = '1') and (empty_checksum = '0')) then
                            out_curr_state <= PKT1;
                            out_is_ip<=false;
                            out_is_UDP<=false;
                            out_is_TCP<=false;
                            if (int_M0_AXIS_TDATA(111 downto 96)=x"0008") then --> 0x0800 reversed
                                out_is_IP<=true;
                                if (int_M0_AXIS_TDATA(191 downto 184)=x"11") then --FIXME does not check eth header lenght
                                    out_is_UDP<=true;
                                end if;
                                if (int_M0_AXIS_TDATA(191 downto 184)=x"06") then --FIXME does not check eth header lenght
                                    out_is_TCP<=true;
                                end if;
                            end if; --IS_IP 
                        end if;
                    when PKT1 =>
                        if (M0_AXIS_TREADY = '1') then
			    --flush <='1'; 
			    --if (out_is_TCP) then
				--stamp<='1';
			    --end if;

			    if ( int_M0_AXIS_TLAST='1') then
				out_curr_state <= IDLE;
			    else
				out_curr_state <= PKT2;
			    end if;
			end if;
                    when PKT2 =>
			if ( (M0_AXIS_TREADY = '1') and (int_M0_AXIS_TLAST='1')) then
			    out_curr_state <= IDLE;
			end if;
                end case;
            end if;
        end if;
    end process;



--M0_AXIS_TDATA <= fifo_dout(255 downto 160) & ntoh16(reduce_chk(compute_chk(out_tcp_checksum32))) & fifo_dout(143 downto 0) when sstamp = '1' else
M0_AXIS_TDATA <= fifo_dout(255 downto 160) & ntoh16(reduce_chk(compute_chk(out_tcp_checksum32))) & fifo_dout(143 downto 0) when stamp = '1' else
                 fifo_dout(255 downto 0);
--M0_AXIS_TDATA <= fifo_dout(255 downto 160) & out_tcp_checksum & fifo_dout(143 downto 0) when sstamp = '1' else
--                 fifo_dout(255 downto 0);

M0_AXIS_TLAST <= int_M0_AXIS_TLAST;  


rd_en <= '1' when ((M0_AXIS_TREADY = '1') and (empty_checksum = '0') and (out_curr_state = IDLE)) else
         '1' when ((M0_AXIS_TREADY = '1') and (out_curr_state /= IDLE)) else
         '0';

stamp <= '1' when ((M0_AXIS_TREADY = '1') and out_is_TCP and (out_curr_state = PKT1)) else
         '0';

flush <= '1' when ((M0_AXIS_TREADY = '1') and (out_curr_state = PKT1)) else
         '0';
fifo_din          <= S0_AXIS_TLAST & S0_AXIS_TUSER & S0_AXIS_TKEEP & S0_AXIS_TDATA;
int_M0_AXIS_TLAST <= fifo_dout(416);
M0_AXIS_TUSER     <= fifo_dout(415 downto 288);
M0_AXIS_TKEEP     <= fifo_dout(287 downto 256);
int_M0_AXIS_TDATA <= fifo_dout(255 downto 0);
         
fallthrough_small_fifo_i: fallthrough_small_fifo generic map ( WIDTH => 417, MAX_DEPTH_BITS =>8)
        port map(
         reset       => not(M0_AXIS_ARESETN),
         clk         => M0_AXIS_ACLK,
         -- Outputs
         dout        => fifo_dout,
         nearly_full => nearly_full,
	 empty       =>  empty,
         --Inputs
         din         => fifo_din,
         wr_en       => (S0_AXIS_TVALID and not(nearly_full)),
         rd_en       => rd_en
         );


---
--TODO: dovrei mascherate S0_AXIS_TDATA con S0_AXIS_TKEEP!!!!
---

masking_f: for i in 0 to 31 generate 
begin
    masked_TDATA((8*i +7) downto 8*i) <= S0_AXIS_TDATA((8*i +7) downto 8*i) when S0_AXIS_TKEEP(i)='1' else x"00";
end generate;

--chk_in1<= ntoh16(compute_chk(new_tcp_checksum & masked_TDATA(255 downto 128)));
--chk_in2<= ntoh16(compute_chk(masked_TDATA(127 downto 0)));
chk_in1<= ntoh16(compute_chk(new_tcp_checksum1 & masked_TDATA(127 downto 0)));
chk_in2<= ntoh16(compute_chk(new_tcp_checksum2 & masked_TDATA(255 downto 128)));

fallthrough_small_fifo_chk_i: fallthrough_small_fifo generic map ( WIDTH => 32, MAX_DEPTH_BITS =>8)
        port map(
  reset       => not(M0_AXIS_ARESETN),
  clk         => M0_AXIS_ACLK,
  -- Outputs
  dout        => out_tcp_checksum32,
  nearly_full => open,
  empty       => empty_checksum,
  --Inputs
  din         => --chk_in, 
  chk_in1 & chk_in2,
  wr_en       => (S0_AXIS_TLAST and S0_AXIS_TVALID and not(nearly_full)),
  rd_en       => flush
  );

--out_tcp_checksum <= chk_in;
S0_AXIS_TREADY<= not(nearly_full);
--M0_AXIS_TVALID <= '1' when ((empty='0') and (srd_en='1')) else '0';
M0_AXIS_TVALID <= rd_en;

process(M0_AXIS_ACLK, M0_AXIS_ARESETN)
    begin
        if (M0_AXIS_ACLK'event and M0_AXIS_ACLK = '1') then
            if (M0_AXIS_ARESETN = '0') then
                pkt <= x"00";
            else
                if ((nearly_full='0') and (S0_AXIS_TVALID = '1') and (S0_AXIS_TLAST='1')) then
                       pkt<=pkt+1;
                end if;
                --if ((empty='0')  and (M0_AXIS_TREADY = '1') and (int_M0_AXIS_TLAST='1')) then
                if ((empty='0')  and (srd_en='1') and (M0_AXIS_TREADY = '1') and (int_M0_AXIS_TLAST='1')) then
                       pkt<=pkt-1;
                end if;
                if ((empty='0')  and (srd_en='1') and (M0_AXIS_TREADY = '1') and (int_M0_AXIS_TLAST='1')) and ((nearly_full='0') and (S0_AXIS_TVALID = '1') and (S0_AXIS_TLAST='1')) then
                    pkt<=pkt;  -- pkt<=pkt +1 -1 ;
                end if;  
            end if;    
        end if;
end process;      
 
   


end architecture full;


