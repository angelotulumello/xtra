-- ----------------------------------------------------------------------------
--                             Entity declaration
-- ----------------------------------------------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;



Library UNISIM;
use UNISIM.vcomponents.all;

Library work;
use work.salutil.all;

Library cam;

entity xtra is
generic (
		-- Parameters of AxiStream Slave Bus Interface S00_AXIS
		C_S00_AXIS_DATA_WIDTH  : integer   := 256;
C_S00_AXIS_TUSER_WIDTH : integer   := 128;
-- Parameters of AxiStream Master Bus Interface M00_AXIS
C_M00_AXIS_DATA_WIDTH  : integer   := 256;
C_M00_AXIS_TUSER_WIDTH : integer   := 128;
-- Parameters of Axi Slave Bus Interface S00_AXIS
C_S00_AXI_DATA_WIDTH  : integer   := 32;
C_S00_AXI_ADDR_WIDTH  : integer   := 32;
C_BASEADDR  : std_logic_vector(31 downto 0)   := x"80000000"
--                C_HIGHADDR  : std_logic_vector(31 downto 0)   := x"9FFFFFFF"
);
port (
		-- Ports of Axi Master Bus Interface M00_AXIS

		-- Global ports
		M0_AXIS_ACLK : in std_logic;
M0_AXIS_ARESETN  : in std_logic;

-- Master Stream Ports.
--M0_AXIS_TVALID   : out std_logic;
--M0_AXIS_TDATA    : out std_logic_vector(C_M00_AXIS_DATA_WIDTH-1 downto 0);
--M0_AXIS_TKEEP    : out std_logic_vector((C_M00_AXIS_DATA_WIDTH/8)-1 downto 0);
--M0_AXIS_TUSER    : out std_logic_vector(C_M00_AXIS_TUSER_WIDTH-1 downto 0);
--M0_AXIS_TLAST    : out std_logic;
--M0_AXIS_TREADY   : in std_logic;


-- Master Stream Ports.
M_TIMER_AXIS_TVALID   : out std_logic;
M_TIMER_AXIS_TDATA    : out std_logic_vector(255 downto 0);
M_TIMER_AXIS_TKEEP    : out std_logic_vector(31 downto 0);
M_TIMER_AXIS_TUSER    : out std_logic_vector(127 downto 0);
M_TIMER_AXIS_TLAST    : out std_logic;
M_TIMER_AXIS_TREADY   : in std_logic;

--             -- Master Stream Ports.
M_PKTGEN_AXIS_TVALID   : out std_logic;
M_PKTGEN_AXIS_TDATA    : out std_logic_vector(255 downto 0);
M_PKTGEN_AXIS_TKEEP    : out std_logic_vector(31 downto 0);
M_PKTGEN_AXIS_TUSER    : out std_logic_vector(127 downto 0);
M_PKTGEN_AXIS_TLAST    : out std_logic;
M_PKTGEN_AXIS_TREADY   : in std_logic;


-- Master Stream Ports.
M_DEBUG_TVALID   : out std_logic;
M_DEBUG_TDATA    : out std_logic_vector(255 downto 0);
M_DEBUG_TKEEP    : out std_logic_vector(31 downto 0);
M_DEBUG_TUSER    : out std_logic_vector(127 downto 0);
M_DEBUG_TLAST    : out std_logic;
M_DEBUG_TREADY   : in std_logic;



-- Ports of Axi Stream Slave Bus Interface S00_AXIS

S0_AXIS_ACLK    : in std_logic;
S0_AXIS_ARESETN : in std_logic;
S0_AXIS_TVALID  : in std_logic;
S0_AXIS_TDATA   : in std_logic_vector(C_S00_AXIS_DATA_WIDTH-1 downto 0);
S0_AXIS_TKEEP   : in std_logic_vector((C_S00_AXIS_DATA_WIDTH/8)-1 downto 0);
S0_AXIS_TUSER   : in std_logic_vector(C_S00_AXIS_TUSER_WIDTH-1 downto 0);
S0_AXIS_TLAST   : in std_logic;
S0_AXIS_TREADY  : out std_logic;

-- Ports of Axi Slave Bus Interface S_AXI
S_AXI_ACLK	    : in std_logic;
S_AXI_ARESETN	: in std_logic;
S_AXI_AWADDR	: in std_logic_vector(C_S00_AXI_ADDR_WIDTH-1 downto 0);
S_AXI_AWVALID	: in std_logic;
S_AXI_WDATA 	: in std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
S_AXI_WSTRB 	: in std_logic_vector(C_S00_AXI_DATA_WIDTH/8-1 downto 0);
S_AXI_WVALID	: in std_logic;
S_AXI_BREADY	: in std_logic;
S_AXI_ARADDR	: in std_logic_vector(C_S00_AXI_ADDR_WIDTH-1 downto 0);
S_AXI_ARVALID	: in std_logic;
S_AXI_RREADY	: in std_logic;
S_AXI_ARREADY	: out std_logic;
S_AXI_RDATA	: out std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
S_AXI_RRESP	: out std_logic_vector(1 downto 0);
S_AXI_RVALID	: out std_logic;
S_AXI_WREADY	: out std_logic;
S_AXI_BRESP	: out std_logic_vector(1 downto 0);
S_AXI_BVALID	: out std_logic;
S_AXI_AWREADY  : out std_logic;


count_timer_item: in std_logic_vector(31 downto 0)
);


attribute core_generation_info : string;
attribute core_generation_info of xtra : entity is "xtra{x_ipProduct=Vivado 2016.4,x_ipVendor=cnit.it}";
end entity;

architecture full of xtra is

type tabella_inv_type is array (0 to 255) of std_logic_vector(9 downto 0);

function ntoh32 (I: std_logic_vector(31 downto 0)) return std_logic_vector is
  begin
  return (I(7 downto 0) & I(15 downto 8) & I(23 downto 16) & I(31 downto 24));
  end function ntoh32;
  
-- T(x)=1024/x
signal inv_tab: tabella_inv_type := (
		conv_std_logic_vector(1023,10), -- 0
		conv_std_logic_vector(1023,10), -- 1
		conv_std_logic_vector( 512,10), -- 2
		conv_std_logic_vector( 341,10), -- 3
		conv_std_logic_vector( 256,10), -- 4
		conv_std_logic_vector( 204,10), -- 5
		conv_std_logic_vector( 170,10), -- 6
		conv_std_logic_vector( 146,10), -- 7
		conv_std_logic_vector( 128,10), -- 8
		conv_std_logic_vector( 113,10), -- 9
		conv_std_logic_vector( 102,10), -- 10
		conv_std_logic_vector( 93,10),  -- 11
		conv_std_logic_vector( 85,10),  -- 12
		conv_std_logic_vector( 78,10),  -- 13
		conv_std_logic_vector( 73,10),  -- 14
		conv_std_logic_vector( 68,10),  -- 15
		conv_std_logic_vector( 64,10),  -- 16
		conv_std_logic_vector( 60,10),  -- 17
		conv_std_logic_vector( 56,10),  -- 18
		conv_std_logic_vector( 53,10),  -- 19
		conv_std_logic_vector( 51,10),  -- 20
		conv_std_logic_vector( 48,10),  -- 21
		conv_std_logic_vector( 46,10),  -- 22
		conv_std_logic_vector( 44,10),  -- 23
		conv_std_logic_vector( 42,10),  -- 24
		conv_std_logic_vector( 40,10),  -- 25
		conv_std_logic_vector( 39,10),  -- 26
		conv_std_logic_vector( 37,10),  -- 27
		conv_std_logic_vector( 36,10),  -- 28
		conv_std_logic_vector( 35,10),  -- 29
		conv_std_logic_vector( 34,10),  -- 30
		conv_std_logic_vector( 33,10),  -- 31
		conv_std_logic_vector( 32,10),  -- 32
		conv_std_logic_vector( 31,10),  -- 33
		conv_std_logic_vector( 30,10),  -- 34
		conv_std_logic_vector( 29,10),  -- 35
		conv_std_logic_vector( 28,10),  -- 36
		conv_std_logic_vector( 27,10),  -- 37
		conv_std_logic_vector( 26,10),  -- 38
		conv_std_logic_vector( 26,10),  -- 39
		conv_std_logic_vector( 25,10),  -- 40
		conv_std_logic_vector( 24,10),  -- 41
		conv_std_logic_vector( 24,10),  -- 42
		conv_std_logic_vector( 23,10),  -- 43
		conv_std_logic_vector( 23,10),  -- 44
		conv_std_logic_vector( 22,10),  -- 45
		conv_std_logic_vector( 22,10),  -- 46
		conv_std_logic_vector( 21,10),  -- 47
		conv_std_logic_vector( 21,10),  -- 48
		conv_std_logic_vector( 20,10),  -- 49
		conv_std_logic_vector( 20,10),  -- 50
		conv_std_logic_vector( 20,10),  -- 51
		conv_std_logic_vector( 19,10),  -- 52
		conv_std_logic_vector( 19,10),  -- 53
		conv_std_logic_vector( 18,10),  -- 54
		conv_std_logic_vector( 18,10),  -- 55
		conv_std_logic_vector( 18,10),  -- 56
		conv_std_logic_vector( 17,10),  -- 57
		conv_std_logic_vector( 17,10),  -- 58
		conv_std_logic_vector( 17,10),  -- 59
		conv_std_logic_vector( 17,10),  -- 60
		conv_std_logic_vector( 16,10),  -- 61
		conv_std_logic_vector( 16,10),  -- 62
		conv_std_logic_vector( 16,10),  -- 63
		conv_std_logic_vector( 16,10),  -- 64
		conv_std_logic_vector( 15,10),  -- 65
		conv_std_logic_vector( 15,10),  -- 66
		conv_std_logic_vector( 15,10),  -- 67
		conv_std_logic_vector( 15,10),  -- 68
		conv_std_logic_vector( 14,10),  -- 69
		conv_std_logic_vector( 14,10),  -- 70
		conv_std_logic_vector( 14,10),  -- 71
		conv_std_logic_vector( 14,10),  -- 72
		conv_std_logic_vector( 14,10),  -- 73
		conv_std_logic_vector( 13,10),  -- 74
		conv_std_logic_vector( 13,10),  -- 75
		conv_std_logic_vector( 13,10),  -- 76
		conv_std_logic_vector( 13,10),  -- 77
		conv_std_logic_vector( 13,10),  -- 78
		conv_std_logic_vector( 12,10),  -- 79
		conv_std_logic_vector( 12,10),  -- 80
		conv_std_logic_vector( 12,10),  -- 81
		conv_std_logic_vector( 12,10),  -- 82
		conv_std_logic_vector( 12,10),  -- 83
		conv_std_logic_vector( 12,10),  -- 84
		conv_std_logic_vector( 12,10),  -- 85
		conv_std_logic_vector( 11,10),  -- 86
		conv_std_logic_vector( 11,10),  -- 87
		conv_std_logic_vector( 11,10),  -- 88
		conv_std_logic_vector( 11,10),  -- 89
		conv_std_logic_vector( 11,10),  -- 90
		conv_std_logic_vector( 11,10),  -- 91
		conv_std_logic_vector( 11,10),  -- 92
		conv_std_logic_vector( 11,10),  -- 93
		conv_std_logic_vector( 10,10),  -- 94
		conv_std_logic_vector( 10,10),  -- 95
		conv_std_logic_vector( 10,10),  -- 96
		conv_std_logic_vector( 10,10),  -- 97
		conv_std_logic_vector( 10,10),  -- 98
		conv_std_logic_vector( 10,10),  -- 99
		conv_std_logic_vector( 10,10),  -- 100
		conv_std_logic_vector( 10,10),  -- 101
		conv_std_logic_vector( 10,10),  -- 102
		conv_std_logic_vector(  9,10),  -- 103
		conv_std_logic_vector(  9,10),  -- 104
		conv_std_logic_vector(  9,10),  -- 105
		conv_std_logic_vector(  9,10),  -- 106
		conv_std_logic_vector(  9,10),  -- 107
		conv_std_logic_vector(  9,10),  -- 108
		conv_std_logic_vector(  9,10),  -- 109
		conv_std_logic_vector(  9,10),  -- 110
		conv_std_logic_vector(  9,10),  -- 111
		conv_std_logic_vector(  9,10),  -- 112
		conv_std_logic_vector(  9,10),  -- 113
		conv_std_logic_vector(  8,10),  -- 114
		conv_std_logic_vector(  8,10),  -- 115
		conv_std_logic_vector(  8,10),  -- 116
		conv_std_logic_vector(  8,10),  -- 117
		conv_std_logic_vector(  8,10),  -- 118
		conv_std_logic_vector(  8,10),  -- 119
		conv_std_logic_vector(  8,10),  -- 120
		conv_std_logic_vector(  8,10),  -- 121
		conv_std_logic_vector(  8,10),  -- 122
		conv_std_logic_vector(  8,10),  -- 123
		conv_std_logic_vector(  8,10),  -- 124
		conv_std_logic_vector(  8,10),  -- 125
		conv_std_logic_vector(  8,10),  -- 126
		conv_std_logic_vector(  8,10),  -- 127
		conv_std_logic_vector(  8,10),  -- 128
		conv_std_logic_vector(  7,10),  -- 129
		conv_std_logic_vector(  7,10),  -- 130
		conv_std_logic_vector(  7,10),  -- 131
		conv_std_logic_vector(  7,10),  -- 132
		conv_std_logic_vector(  7,10),  -- 133
		conv_std_logic_vector(  7,10),  -- 134
		conv_std_logic_vector(  7,10),  -- 135
		conv_std_logic_vector(  7,10),  -- 136
		conv_std_logic_vector(  7,10),  -- 137
		conv_std_logic_vector(  7,10),  -- 138
		conv_std_logic_vector(  7,10),  -- 139
		conv_std_logic_vector(  7,10),  -- 140
		conv_std_logic_vector(  7,10),  -- 141
		conv_std_logic_vector(  7,10),  -- 142
		conv_std_logic_vector(  7,10),  -- 143
		conv_std_logic_vector(  7,10),  -- 144
		conv_std_logic_vector(  7,10),  -- 145
		conv_std_logic_vector(  7,10),  -- 146
		conv_std_logic_vector(  6,10),  -- 147
		conv_std_logic_vector(  6,10),  -- 148
		conv_std_logic_vector(  6,10),  -- 149
		conv_std_logic_vector(  6,10),  -- 150
		conv_std_logic_vector(  6,10),  -- 151
		conv_std_logic_vector(  6,10),  -- 152
		conv_std_logic_vector(  6,10),  -- 153
		conv_std_logic_vector(  6,10),  -- 154
		conv_std_logic_vector(  6,10),  -- 155
		conv_std_logic_vector(  6,10),  -- 156
		conv_std_logic_vector(  6,10),  -- 157
		conv_std_logic_vector(  6,10),  -- 158
		conv_std_logic_vector(  6,10),  -- 159
		conv_std_logic_vector(  6,10),  -- 160
		conv_std_logic_vector(  6,10),  -- 161
		conv_std_logic_vector(  6,10),  -- 162
		conv_std_logic_vector(  6,10),  -- 163
		conv_std_logic_vector(  6,10),  -- 164
		conv_std_logic_vector(  6,10),  -- 165
		conv_std_logic_vector(  6,10),  -- 166
		conv_std_logic_vector(  6,10),  -- 167
		conv_std_logic_vector(  6,10),  -- 168
		conv_std_logic_vector(  6,10),  -- 169
		conv_std_logic_vector(  6,10),  -- 170
		conv_std_logic_vector(  5,10),  -- 171
		conv_std_logic_vector(  5,10),  -- 172
		conv_std_logic_vector(  5,10),  -- 173
		conv_std_logic_vector(  5,10),  -- 174
		conv_std_logic_vector(  5,10),  -- 175
		conv_std_logic_vector(  5,10),  -- 176
		conv_std_logic_vector(  5,10),  -- 177
		conv_std_logic_vector(  5,10),  -- 178
		conv_std_logic_vector(  5,10),  -- 179
		conv_std_logic_vector(  5,10),  -- 180
		conv_std_logic_vector(  5,10),  -- 181
		conv_std_logic_vector(  5,10),  -- 182
		conv_std_logic_vector(  5,10),  -- 183
		conv_std_logic_vector(  5,10),  -- 184
		conv_std_logic_vector(  5,10),  -- 185
		conv_std_logic_vector(  5,10),  -- 186
		conv_std_logic_vector(  5,10),  -- 187
		conv_std_logic_vector(  5,10),  -- 188
		conv_std_logic_vector(  5,10),  -- 189
		conv_std_logic_vector(  5,10),  -- 190
		conv_std_logic_vector(  5,10),  -- 191
		conv_std_logic_vector(  5,10),  -- 192
		conv_std_logic_vector(  5,10),  -- 193
		conv_std_logic_vector(  5,10),  -- 194
		conv_std_logic_vector(  5,10),  -- 195
		conv_std_logic_vector(  5,10),  -- 196
		conv_std_logic_vector(  5,10),  -- 197
		conv_std_logic_vector(  5,10),  -- 198
		conv_std_logic_vector(  5,10),  -- 199
		conv_std_logic_vector(  5,10),  -- 200
		conv_std_logic_vector(  5,10),  -- 201
		conv_std_logic_vector(  5,10),  -- 202
		conv_std_logic_vector(  5,10),  -- 203
		conv_std_logic_vector(  5,10),  -- 204
		conv_std_logic_vector(  4,10),  -- 205
		conv_std_logic_vector(  4,10),  -- 206
		conv_std_logic_vector(  4,10),  -- 207
		conv_std_logic_vector(  4,10),  -- 208
		conv_std_logic_vector(  4,10),  -- 209
		conv_std_logic_vector(  4,10),  -- 210
		conv_std_logic_vector(  4,10),  -- 211
		conv_std_logic_vector(  4,10),  -- 212
		conv_std_logic_vector(  4,10),  -- 213
		conv_std_logic_vector(  4,10),  -- 214
		conv_std_logic_vector(  4,10),  -- 215
		conv_std_logic_vector(  4,10),  -- 216
		conv_std_logic_vector(  4,10),  -- 217
		conv_std_logic_vector(  4,10),  -- 218
		conv_std_logic_vector(  4,10),  -- 219
		conv_std_logic_vector(  4,10),  -- 220
		conv_std_logic_vector(  4,10),  -- 221
		conv_std_logic_vector(  4,10),  -- 222
		conv_std_logic_vector(  4,10),  -- 223
		conv_std_logic_vector(  4,10),  -- 224
		conv_std_logic_vector(  4,10),  -- 225
		conv_std_logic_vector(  4,10),  -- 226
		conv_std_logic_vector(  4,10),  -- 227
		conv_std_logic_vector(  4,10),  -- 228
		conv_std_logic_vector(  4,10),  -- 229
		conv_std_logic_vector(  4,10),  -- 230
		conv_std_logic_vector(  4,10),  -- 231
		conv_std_logic_vector(  4,10),  -- 232
		conv_std_logic_vector(  4,10),  -- 233
		conv_std_logic_vector(  4,10),  -- 234
		conv_std_logic_vector(  4,10),  -- 235
		conv_std_logic_vector(  4,10),  -- 236
		conv_std_logic_vector(  4,10),  -- 237
		conv_std_logic_vector(  4,10),  -- 238
		conv_std_logic_vector(  4,10),  -- 239
		conv_std_logic_vector(  4,10),  -- 240
		conv_std_logic_vector(  4,10),  -- 241
		conv_std_logic_vector(  4,10),  -- 242
		conv_std_logic_vector(  4,10),  -- 243
		conv_std_logic_vector(  4,10),  -- 244
		conv_std_logic_vector(  4,10),  -- 245
		conv_std_logic_vector(  4,10),  -- 246
		conv_std_logic_vector(  4,10),  -- 247
		conv_std_logic_vector(  4,10),  -- 248
		conv_std_logic_vector(  4,10),  -- 249
		conv_std_logic_vector(  4,10),  -- 250
		conv_std_logic_vector(  4,10),  -- 251
		conv_std_logic_vector(  4,10),  -- 252
		conv_std_logic_vector(  4,10),  -- 253
		conv_std_logic_vector(  4,10),  -- 254
		conv_std_logic_vector(  4,10)   -- 255
);



function myror (L: std_logic_vector; R: integer) return std_logic_vector is
begin
return to_stdlogicvector(to_bitvector(L) ror R);
end function myror;


function ntoh (I: std_logic_vector(31 downto 0)) return std_logic_vector is
begin
return (I(7 downto 0) & I(15 downto 8) & I(23 downto 16) & I(31 downto 24));
end function ntoh;

function ntoh16 (I: std_logic_vector(15 downto 0)) return std_logic_vector is
begin
return (I(7 downto 0) & I(15 downto 8));
end function ntoh16;


function max(a, b: std_logic_vector(31 downto 0)) return std_logic_vector is
variable max: std_logic_vector(31 downto 0) := (others => '0');
variable temp: std_logic_vector(31 downto 0) := (others => '0');
begin

if (IN_SYNTHESIS) then
temp:=b;
else
temp:="000" & b(31 downto 3);
end if;

if (temp>x"0040_0000") then
return a;
end if;

if (a > temp) then
return a;
else
return temp;
end if;

return a;

end function max;


-- delta(timer,TimeEcho);
function delta(a,b: std_logic_vector) return std_logic_vector is
variable result:std_logic_vector(31 downto 0) := (others => '0');
begin

if (IN_SYNTHESIS) then
result:= a(31 downto 0)-(b(21 downto 0) & "0000000000");
else
result:= a(31 downto 0)-(b(24 downto 0) & "0000000");
end if;

return result;
end;



signal RESETN,RESET   : std_logic;
signal MY_BASEADDR  : std_logic_vector(31 downto 0)   := x"80000000";
signal send: std_logic;
signal  reg_test0: std_logic_vector(31 downto 0);
signal  count_send_pkt,count_ack,count_timer1,count_timer2,count_pkt,count_timer, end_count  : std_logic_vector(31 downto 0);


-- ----------------------------------------------------------------------------
-- signals for ETH decoding
-- ----------------------------------------------------------------------------

type pkt_states is (START,reset_all, IDLE, PKT1,PKT1_wait,PKT2,PKT3,PKT3_wait,PKT4,PKT4_wait,PKT5, START_PKT1,START_PKT2,START_PKT3,START_PKT4);

signal curr_state, next_state : pkt_states;
signal FSMnowait,step: boolean; -- FSMnowait evita pacchetti back to back

-- ----------------------------------------------------------------------------
-- signals for packet classification
-- ----------------------------------------------------------------------------
signal is_IP,is_UDP,is_TCP: boolean;
signal src_if: std_logic_vector(7 downto 0);
signal dst_if: std_logic_vector(7 downto 0);
signal pkt_len: std_logic_vector(15 downto 0);
signal random: std_logic_vector(31 downto 0);
--signal timer: std_logic_vector(63 downto 0);
signal nanotimer: std_logic_vector(7 downto 0);
signal threshold: std_logic_vector(31 downto 0);
signal src_mac,dst_mac: std_logic_vector(47 downto 0);
signal src_ip,dst_ip,seq_num,ack_num: std_logic_vector(31 downto 0);
signal src_port,dst_port: std_logic_vector(15 downto 0);

signal OptionKind   : std_logic_vector(7 downto 0);
signal OptionLength : std_logic_vector(7 downto 0);
signal TimeSender   : std_logic_vector(31 downto 0);
signal count,temposyn,TimeEcho     : std_logic_vector(31 downto 0);
signal TIMER_WAIT   : std_logic_vector(31 downto 0);


signal ack,ackdm1: std_logic;
signal temp_id,conn_id:  std_logic_vector(9 downto 0);

signal int_M0_AXIS_TUSER: std_logic_vector(C_M00_AXIS_TUSER_WIDTH-1 downto 0);
signal int_S0_AXIS_TREADY: std_logic;



-- ----------------------------------------------------------------------------
-- signals for AXI Lite
-- ----------------------------------------------------------------------------

--type axi_states is (addr_wait, read_state, write_state, response_state);
--signal axi_state : axi_states;
signal axi_state :std_logic_vector(2 downto 0);
signal cam1_we,write_enable: std_logic;
signal address : std_logic_vector(C_S00_AXI_ADDR_WIDTH-1 downto 0);
signal read_enable: std_logic;
signal match,int_S_AXI_BVALID: std_logic;


signal match_addr_tcam,match_addr_tcam_t: std_logic_vector(7 downto 0);
-- ----------------------------------------------------------------------------
-- signals for XTRA Registers
-- ----------------------------------------------------------------------------
signal R0,R1,R2,R3,R4,R5,R6,R7,R8,R9,R10,R11,R12,R13,R14,R15: std_logic_vector(31 downto 0);
signal tcam_mask: std_logic_vector(31 downto 0);
signal R0_in,R1_in,R2_in,R3_in,R4_in,R5_in,R6_in,R7_in,R8_in,R9_in,R10_in,R11_in,R12_in,R13_in,R14_in,R15_in: std_logic_vector(31 downto 0);

signal field0,field1,field2,field3,field4,field5,field6,field7: std_logic_vector(31 downto 0);

signal HOUT4,HOUT1,HOUT2,HOUT3: std_logic_vector(31 downto 0);

--  mux output for select action inputs
signal  SOUTA1a,SOUTA1b,SOUTA2a,SOUTA2b,SOUTA3a,SOUTA3b,SOUTA4a,SOUTA4b,SOUTA5a,SOUTA5b,SOUTA6a,SOUTA6b,SOUTA7a,SOUTA7b,SOUTA8a,SOUTA8b:std_logic_vector(31 downto 0);


-- registered mux output for actions
signal RHOUT4,RHOUT1,RHOUT2,RHOUT3: std_logic_vector(31 downto 0);



signal timer_expired_value, data_cam:  std_logic_vector(31 downto 0);
signal execute,pkt,timer,timer_c,flag_clk: std_logic;

signal present_state,future_state: std_logic_vector(31 downto 0);
signal conditions,conditions_t, conditions_reg,conditions_reg_1: std_logic_vector(31 downto 0);
signal header: std_logic_vector(1023 downto 0);
signal ConfM1,ConfM2,ConfM3,ConfM4: std_logic_vector(31 downto 0);
signal ConfH1_offset,ConfH2_offset,ConfH3_offset,ConfH4_offset: std_logic_vector(31 downto 0);
signal ConfH1_size,ConfH2_size,ConfH3_size,ConfH4_size: std_logic_vector(31 downto 0);

type action_array is array (8 downto 0) of std_logic_vector(31 downto 0);
signal action_s0,action_s1,action_s2: action_array;
signal read_from_ram_s0,read_from_ram_s1,read_from_ram_s2: action_array;
signal read_from_ram0: std_logic_vector(31 downto 0);
signal we_rdp_s0,we_rdp_s1,we_rdp_s2:  std_logic_vector(8 downto 0);
signal we_r0: std_logic;

signal action1comb,action2comb,action3comb,action4comb,action5comb,action6comb,action7comb,action8comb: std_logic_vector(31 downto 0);
signal action0,action1,action2,action3,action4,action5,action6,action7,action8: std_logic_vector(31 downto 0);

-- register configuration condition
signal CR_1,CR_2, CR_3, CR_4, CR_5, CR_6, CR_7, CR_8, CR_9, CR_10, CR_11: std_logic_vector(31 downto 0);

signal count_timestamp: std_logic_vector(31 downto 0);
signal R15_timestamp: std_logic_vector(31 downto 0);
signal start_register:std_logic_vector(31 downto 0);

signal count_66:std_logic_vector(31 downto 0);
signal count_1024:std_logic_vector(31 downto 0);

attribute max_fanout:integer;
attribute max_fanout of SOUTA1a: signal is 4;
attribute max_fanout of SOUTA1b: signal is 4;
attribute max_fanout of SOUTA2a: signal is 4;
attribute max_fanout of SOUTA2b: signal is 4;
attribute max_fanout of curr_state: signal is 4;


-- ----------------------------------------------------------------------------
--                             Architecture body
-- ----------------------------------------------------------------------------

 begin


 MY_BASEADDR<=x"80000000";
 RESETN<=S0_AXIS_ARESETN;
 RESET<=not (S0_AXIS_ARESETN);


 -- -------------------------------------------------------------------------
 --
 -- process to detect if the next word is available
 --
 -- -------------------------------------------------------------------------

 step_assign: step<= true when (S0_AXIS_TVALID='1') and (int_S0_AXIS_TREADY = '1') else false;

 -- -------------------------------------------------------------------------
 --
 -- extract Header ETH-IP-TCP/UDP
 --
 -- -------------------------------------------------------------------------

process(S0_AXIS_ACLK, S0_AXIS_ARESETN)
	begin
	if (S0_AXIS_ACLK'event and S0_AXIS_ACLK = '1') then
		if (S0_AXIS_ARESETN = '0') then
			FSMnowait<=true;
			int_S0_AXIS_TREADY <= '1';
			curr_state <= START;
			timer<='0';
			pkt<='0';
			end_count<=(others => '0');
			src_if<= (others => '0');
			header<= (others => '0');
			timer_expired_value <= (others => '0');
			flag_clk<='0';
			M_DEBUG_TVALID<='0';
			M_DEBUG_TLAST  <= '0';
			M_DEBUG_TDATA <=  (others =>'0');
			M_DEBUG_TUSER  <= (others =>'0');
			M_DEBUG_TKEEP  <= (others =>'0');
			execute<='0';

		else
			M_DEBUG_TVALID<='0';
			M_DEBUG_TDATA <=  (others =>'0');
			M_DEBUG_TUSER(127 downto 32)  <= (others =>'0');
			M_DEBUG_TUSER(31 downto 24) <= reg_test0(7 downto 0);
			M_DEBUG_TUSER(23 downto 0) <= x"010040";
			M_DEBUG_TKEEP  <= (others =>'1');
			M_DEBUG_TLAST  <= '1';

			case curr_state is

			------------------------------------------------------------------------
			------------          START_FSM ---------------------------
			------------------------------------------------------------------------

			when START =>
				if (start_register=x"00000001") then
					curr_state <= START_PKT1;
				end if;
			when START_PKT1 =>
				curr_state <= START_PKT2;
			when START_PKT2 =>
				curr_state <= START_PKT3;
			when START_PKT3 =>
				curr_state <= START_PKT4;
			when START_PKT4 =>
				curr_state <= IDLE;


			when IDLE =>
				src_if<= (others => '0');
				execute<='0';
				timer<='0';
				pkt<='0';
				FSMnowait<=true;
				S0_AXIS_TREADY <= '1';
				if (start_register=x"00000000") then
				    curr_state<=reset_all;
				else
				if (S0_AXIS_TVALID='1') then
					M_DEBUG_TVALID<= '1';
					M_DEBUG_TDATA <= x"01" & x"12341234" & x"fefe" & x"70a30634401b9f" & x"3200" & x"0045" & x"0008" & x"010c42363996" & x"3633159196B4";
					curr_state <= PKT1;
					pkt_len <= S0_AXIS_TUSER(15 downto 0);
					src_if  <= S0_AXIS_TUSER(23 downto 16);
					header(255 downto 0)<=S0_AXIS_TDATA;
					timer_expired_value <= S0_AXIS_TDATA(31 downto 0);-- non ci interessa: dati che si porta dietro il timer
					--timer_retransmittion_count <= S0_AXIS_TDATA(39 downto 32);
					--timer_expired              <= S0_AXIS_TDATA(79 downto 40);
				end if;
				if (S0_AXIS_TVALID='1' and S0_AXIS_TLAST='1') then
					FSMnowait<=false;
					S0_AXIS_TREADY <= '0';
				end if;
			end if;
			
			when reset_all=>
			curr_state<=START;
			when PKT1 =>
				if (S0_AXIS_TVALID='1') then
					header(511 downto 256) <=S0_AXIS_TDATA;
					if (src_if=x"ff") then
						timer<='1';
						end_count<=end_count+1;
					end if;
					if (src_if=x"01") then
						pkt<='1';
					end if;
					curr_state <= PKT1_wait;
				end if;
				if (S0_AXIS_TVALID='1' and S0_AXIS_TLAST='1') then
					flag_clk<='1';
					FSMnowait<=false;
					S0_AXIS_TREADY <= '0';
				end if;
				
				
				
				when PKT1_wait =>
                if (S0_AXIS_TVALID='1') then
                    header(511 downto 256) <=S0_AXIS_TDATA;
                    --execute<='1';
                    curr_state <= PKT2;
                end if;
                if(flag_clk ='1' and S0_AXIS_TVALID='0') then
                    curr_state <= PKT2;
                    --execute<='1';
                end if;
                if (S0_AXIS_TVALID='1' and S0_AXIS_TLAST='1') then
                    FSMnowait<=false;
                    S0_AXIS_TREADY <= '0';
                    flag_clk<='1';
                end if;
				
				
				
				
				
				
				
				
				
				when PKT2 =>
				if (S0_AXIS_TVALID='1') then
					--header(511 downto 256) <=S0_AXIS_TDATA;
					--execute<='1';
					curr_state <= PKT3;
				end if;
				if(flag_clk ='1' and S0_AXIS_TVALID='0') then
					curr_state <= PKT3;
					--execute<='1';
				end if;
				if (S0_AXIS_TVALID='1' and S0_AXIS_TLAST='1') then
					FSMnowait<=false;
					S0_AXIS_TREADY <= '0';
					flag_clk<='1';
				end if;
			when PKT3 =>
				execute<='0';
				--execute<='1';
				if (S0_AXIS_TVALID='1') then
					curr_state <=PKT3_wait;   --PKT4;
				end if;
				if(flag_clk ='1' and S0_AXIS_TVALID='0') then
					curr_state <= PKT3_wait;
					--execute<='1';
				end if;
				if (S0_AXIS_TVALID='1' and S0_AXIS_TLAST='1') then
					FSMnowait<=false;
					S0_AXIS_TREADY <= '0';
					flag_clk<='1';
				end if;


			when PKT3_wait =>
				execute<='1';
				if (S0_AXIS_TVALID='1') then
					curr_state <= PKT4;
				end if;
				if(flag_clk ='1' and S0_AXIS_TVALID='0') then
					curr_state <= PKT4;
					--execute<='1';
				end if;
				if (S0_AXIS_TVALID='1' and S0_AXIS_TLAST='1') then
					FSMnowait<=false;
					S0_AXIS_TREADY <= '0';
					flag_clk<='1';
				end if;







			when PKT4 =>
				execute<='1';
				if (S0_AXIS_TVALID='1') then
					curr_state <= PKT4_wait;
				end if;
				if(flag_clk ='1' and S0_AXIS_TVALID='0') then
					curr_state <= PKT4_wait;
					--execute<='1';
				end if;
				if (S0_AXIS_TVALID='1' and S0_AXIS_TLAST='1') then
					FSMnowait<=false;
					S0_AXIS_TREADY <= '0';
					flag_clk<='1';
				end if;
				
				when PKT4_wait =>
                   execute<='1';
                   if (S0_AXIS_TVALID='1') then
                       curr_state <= PKT5;
                   end if;
                   if(flag_clk ='1' and S0_AXIS_TVALID='0') then
                       curr_state <= PKT5;
                       --execute<='1';
                   end if;
                   if (S0_AXIS_TVALID='1' and S0_AXIS_TLAST='1') then
                       FSMnowait<=false;
                       S0_AXIS_TREADY <= '0';
                       flag_clk<='1';
                   end if;
				



			when PKT5 =>
				S0_AXIS_TREADY <= '1';
				execute<='0';
				if ( not(FSMnowait) or  (S0_AXIS_TVALID='1' and S0_AXIS_TLAST='1')) then
					curr_state <= IDLE;
					flag_clk<='0';
				end if;
			end case;
		end if;
	end if;
	end process;


--header:
H1:  entity work.sam32 port map(
			header=>header,
			offset=>ConfH1_offset,
			size=>ConfH1_size,
			field=>HOUT1);
H2:  entity work.sam32 port map(
			header=>header,
			offset=>ConfH2_offset,
			size=>ConfH2_size,
			field=>HOUT2);
H3:  entity work.sam32 port map(
			header=>header,
			offset=>ConfH3_offset,
			size=>ConfH3_size,
			field=>HOUT3);
H4:  entity work.sam32 port map(
			header=>header,
			offset=>ConfH4_offset,
			size=>ConfH4_size,
			field=>HOUT4);

condition:process(S0_AXIS_ACLK, S0_AXIS_ARESETN)
variable cR1: std_logic_vector(31 downto 0);
variable cR2: std_logic_vector(31 downto 0);
variable cR3: std_logic_vector(31 downto 0);
variable cR4: std_logic_vector(31 downto 0);
variable cR5: std_logic_vector(31 downto 0);
variable cR6: std_logic_vector(31 downto 0);
variable cR7: std_logic_vector(31 downto 0);
variable cR8: std_logic_vector(31 downto 0);
variable cR9: std_logic_vector(31 downto 0);
variable cR10: std_logic_vector(31 downto 0);
variable cR11: std_logic_vector(31 downto 0);
variable cR12: std_logic_vector(31 downto 0);
variable cR13: std_logic_vector(31 downto 0);
variable cR14: std_logic_vector(31 downto 0);
variable cR15: std_logic_vector(31 downto 0);
variable cR16: std_logic_vector(31 downto 0);
	begin
		if(S0_AXIS_ACLK' event and S0_AXIS_ACLK ='1') then
			if(S0_AXIS_ARESETN = '0') then
--			cR1:=x"00000000";
--			cR2:=x"00000000";
--			cR3:=x"00000000";
--			cR4:=x"00000000";
--			cR5:=x"00000000";
--			cR6:=x"00000000";
--			cR7:=x"00000000";
--			cR8:=x"00000000";
--			cR9:=x"00000000";
--			cR10:=x"00000000";
--			cR11:=x"00000000";
--			cR12:=x"00000000";
--			cR13:=x"00000000";
--			cR14:=x"00000000";
--			cR15:=x"00000000";
--			cR16:=x"00000000";
			
			else
			 cR1:=x"00000000";
             cR2:=x"00000000";
             cR3:=x"00000000";
             cR4:=x"00000000";
             cR5:=x"00000000";
                          cR6:=x"00000000";
                          cR7:=x"00000000";
                          cR8:=x"00000000";
                          cR9:=x"00000000";                                             --<----------- CAZZO NON TOCCATE LA SCALETTA
                                       cR10:=x"00000000";
                                       cR11:=x"00000000";
                                       cR12:=x"00000000";
                                       cR13:=x"00000000";
                                                    cR14:=x"00000000";
                                                    cR15:=x"00000000";
                                                    cR16:=x"00000000";
--			if(curr_state=reset_all) then
--			   CR_1<=X"00000000";
--			   CR_2<=X"00000000";
--			   CR_3<=X"00000000";
--			   CR_4<=X"00000000";
--			   CR_5<=X"00000000";
--			   CR_6<=X"00000000";
--			   CR_7<=X"00000000";
--			   CR_8<=X"00000000";
--			   CR_9<=X"00000000";
--			   CR_10<=X"00000000";
--			   CR_11<=X"00000000";
--			   conditions<=x"00000000";
--			   end if;
--			if(curr_state=reset_all) then
--			  conditions<=(others=>'0'); 
			   
			
			if (curr_state=PKT1_wait) THEN
			    
			     conditions(31 downto 24) <= present_state(7 downto 0);
                 conditions(23) <=timer;
                 conditions(22)<= pkt;
                 conditions(21 downto 8)<=(others=>'0');
				if CR_1(8 downto 4)="00000" then
				cR1:=R0;
				end if;
				if CR_1(8 downto 4)="00001" then 
				cR1:=R1;
				end if;
				if CR_1(8 downto 4)="00010" then   
				cR1:=R2;
				end if;
				if CR_1(8 downto 4)="00011" then  
				cR1:=R3;
				end if;
				if CR_1(8 downto 4)="00100" then
				cR1:=R4;
				end if;
				if CR_1(8 downto 4)="00101" then
				cR1:=R5;
				end if;
				if CR_1(8 downto 4)="00110" then
				cR1:=R6;
				end if;
				if CR_1(8 downto 4)="00111" then      
				cR1:=R7;
				end if;
				if CR_1(8 downto 4)="01000" then
				cR1:=R8;
				end if;
				if CR_1(8 downto 4)="01001" then
				cR1:=R9;
				end if;
				if CR_1(8 downto 4)="01010" then
				cR1:=R10;
				end if;     
				if CR_1(8 downto 4)="01011" then
				cR1:=R11;
				end if;      
				if CR_1(8 downto 4)="01100" then      
				cR1:=R12;
				end if;
				if CR_1(8 downto 4)="01101" then
				cR1:=R13;
				end if;
				if CR_1(8 downto 4)="01110" then
				cR1:=R14;
				end if;
				if CR_1(8 downto 4)="01111" then
				cR1:=R15;
				end if;
				if CR_1(8 downto 4)="10000" then
				cR1:=HOUT1;
				end if;
				if CR_1(8 downto 4)="10001" then
				cR1:=HOUT2;
				end if;
				if CR_1(8 downto 4)="10010" then
				cR1:=HOUT3;
				end if;
				if CR_1(8 downto 4)="10011" then
				cR1:=HOUT4;
				end if;
				if CR_1(13 downto 9)="00000" then
				cR2:=R0;
				end if;
				if CR_1(13 downto 9)="00001" then 
				cR2:=R1;
				end if;
				if CR_1(13 downto 9)="00010" then   
				cR2:=R2;
				end if;
				if CR_1(13 downto 9)="00011" then  
				cR2:=R3;
				end if;
				if CR_1(13 downto 9)="00100" then
				cR2:=R4;
				end if;
				if CR_1(13 downto 9)="00101" then
				cR2:=R5;
				end if;
				if CR_1(13 downto 9)="00110" then
				cR2:=R6;
				end if;
				if CR_1(13 downto 9)="00111" then      
				cR2:=R7;
				end if;
				if CR_1(13 downto 9)="01000" then
				cR2:=R8;
				end if;
				if CR_1(13 downto 9)="01001" then
				cR2:=R9;
				end if;
				if CR_1(13 downto 9)="01010" then
				cR2:=R10;
				end if;     
				if CR_1(13 downto 9)="01011" then
				cR2:=R11;
				end if;      
				if CR_1(13 downto 9)="01100" then      
				cR2:=R12;
				end if;
				if CR_1(13 downto 9)="01101" then
				cR2:=R13;
				end if;
				if CR_1(13 downto 9)="01110" then
				cR2:=R14;
				end if;
				if CR_1(13 downto 9)="01111" then
				cR2:=R15;
				end if;
				if CR_1(13 downto 9)="10000" then
				cR2:=HOUT1;
				end if;
				if CR_1(13 downto 9)="10001" then
				cR2:=HOUT2;
				end if;
				if CR_1(13 downto 9)="10010" then
				cR2:=HOUT3;
				end if;
				if CR_1(13 downto 9)="10011" then
				cR2:=HOUT4;
				end if;
				
				if CR_1(3)='0' then
				if CR_1(2 downto 0)="000" and cR1>cR2 then
				conditions(0)<='1';
				
    		   		 elsif CR_1(2 downto 0)="001" and  cR1=cR2 then
			  	conditions(0)<='1';
				
				elsif CR_1(2 downto 0)="010" and (not(cR1>cR2))and (not(cR1=cR2 )) then
				conditions(0)<='1';
				
				elsif CR_1(2 downto 0)="100" and cR1>=cR2 then
				conditions(0)<='1';
				
				elsif CR_1(2 downto 0)="101" and cR1/=cR2 then
				conditions(0)<='1';
				       
				
				elsif CR_1(2 downto 0)="110" and cR1<=cR2 then
				conditions(0)<='1';
				
				else 
				conditions(0)<='0';
				end if;	    
                      		
				else
				if CR_1(2 downto 0)="000" and cR1>"000000000000000"&CR_1(31 downto 14) then
				conditions(0)<='1';
				
    		   		 elsif CR_1(2 downto 0)="001" and  cR1="000000000000000"&CR_1(31 downto 14) then
			  	conditions(0)<='1';
				
				elsif CR_1(2 downto 0)="010" and (not(cR1>"000000000000000"&CR_1(31 downto 14)))and (not(cR1="0000000000000000"&CR_1(31 downto 14) )) then
				conditions(0)<='1';
				
				elsif CR_1(2 downto 0)="100" and cR1>="0000000000000000"&CR_1(31 downto 14) then
				conditions(0)<='1';
				
				elsif CR_1(2 downto 0)="101" and cR1/="000000000000000"&CR_1(31 downto 14) then
				conditions(0)<='1';
				       
				
				elsif CR_1(2 downto 0)="110" and cR1<="000000000000000"&CR_1(31 downto 14) then
				conditions(0)<='1';
				
				else 
				conditions(0)<='0';
				end if;
				end if;	    

				
			
				
			   
				if CR_2(8 downto 4)="00000" then
				cR3:=R0;
				end if;
				if CR_2(8 downto 4)="00001" then 
				cR3:=R1;
				end if;
				if CR_2(8 downto 4)="00010" then   
				cR3:=R2;
				end if;
				if CR_2(8 downto 4)="00011" then  
				cR3:=R3;
				end if;
				if CR_2(8 downto 4)="00100" then
				cR3:=R4;
				end if;
				if CR_2(8 downto 4)="00101" then
				cR3:=R5;
				end if;
				if CR_2(8 downto 4)="00110" then
				cR3:=R6;
				end if;
				if CR_2(8 downto 4)="00111" then      
				cR3:=R7;
				end if;
				if CR_2(8 downto 4)="01000" then
				cR3:=R8;
				end if;
				if CR_2(8 downto 4)="01001" then
				cR3:=R9;
				end if;
				if CR_2(8 downto 4)="01010" then
				cR3:=R10;
				end if;     
				if CR_2(8 downto 4)="01011" then
				cR3:=R11;
				end if;      
				if CR_2(8 downto 4)="01100" then      
				cR3:=R12;
				end if;
				if CR_2(8 downto 4)="01101" then
				cR3:=R13;
				end if;
				if CR_2(8 downto 4)="01110" then
				cR3:=R14;
				end if;
				if CR_2(8 downto 4)="01111" then
				cR3:=R15;
				end if;
				if CR_2(8 downto 4)="10000" then
				cR3:=HOUT1;
				end if;
				if CR_2(8 downto 4)="10001" then
				cR3:=HOUT2;
				end if;
				if CR_2(8 downto 4)="10010" then
				cR3:=HOUT3;
				end if;
				if CR_2(8 downto 4)="10011" then
				cR3:=HOUT4;
				end if;
				if CR_2(13 downto 9)="00000" then
				cR4:=R0;
				end if;
				if CR_2(13 downto 9)="00001" then 
				cR4:=R1;
				end if;
				if CR_2(13 downto 9)="00010" then   
				cR4:=R2;
				end if;
				if CR_2(13 downto 9)="00011" then  
				cR4:=R3;
				end if;
				if CR_2(13 downto 9)="00100" then
				cR4:=R4;
				end if;
				if CR_2(13 downto 9)="00101" then
				cR4:=R5;
				end if;
				if CR_2(13 downto 9)="00110" then
				cR4:=R6;
				end if;
				if CR_2(13 downto 9)="00111" then      
				cR4:=R7;
				end if;
				if CR_2(13 downto 9)="01000" then
				cR4:=R8;
				end if;
				if CR_2(13 downto 9)="01001" then
				cR4:=R9;
				end if;
				if CR_2(13 downto 9)="01010" then
				cR4:=R10;
				end if;     
				if CR_2(13 downto 9)="01011" then
				cR4:=R11;
				end if;      
				if CR_2(13 downto 9)="01100" then      
				cR4:=R12;
				end if;
				if CR_2(13 downto 9)="01101" then
				cR4:=R13;
				end if;
				if CR_2(13 downto 9)="01110" then
				cR4:=R14;
				end if;
				if CR_2(13 downto 9)="01111" then
				cR4:=R15;
				end if;
				if CR_2(13 downto 9)="10000" then
				cR4:=HOUT1;
				end if;
				if CR_2(13 downto 9)="10001" then
				cR4:=HOUT2;
				end if;
				if CR_2(13 downto 9)="10010" then
				cR4:=HOUT3;
				end if;
				if CR_2(13 downto 9)="10011" then
				cR4:=HOUT4;
				end if;
				
				if CR_2(3)='0' then
				if CR_2(2 downto 0)="000" and cR3>cR4 then
				conditions(1)<='1';
				
				
				elsif CR_2(2 downto 0)="001" and cR3=cR4 then
			  	conditions(1)<='1';
				
				elsif CR_2(2 downto 0)="010" and (not( cR3>cR4))and(not( cR3=cR4 )) then
				conditions(1)<='1';
				
				elsif CR_2(2 downto 0)="100" and cR3>=cR4 then
				conditions(1)<='1';
					
				elsif CR_2(2 downto 0)="101" and cR3/=cR4 then
				conditions(1)<='1';
				        
				
				elsif CR_2(2 downto 0)="110" and cR3<=cR4 then
				conditions(1)<='1';
				else 
				conditions(1)<='0';
				end if;

				else	    
				if CR_2(2 downto 0)="000" and cR3>"000000000000000"& CR_2(31 downto 14) then
				conditions(1)<='1';
				
				
				elsif CR_2(2 downto 0)="001" and cR3="000000000000000"& CR_2(31 downto 14) then
			  	conditions(1)<='1';
				
				elsif CR_2(2 downto 0)="010" and (not( cR3>"000000000000000"& CR_2(31 downto 14)))and(not( cR3="000000000000000"& CR_2(31 downto 14) )) then
				conditions(1)<='1';
				
				elsif CR_2(2 downto 0)="100" and cR3>="000000000000000"& CR_2(31 downto 14) then
				conditions(1)<='1';
					
				elsif CR_2(2 downto 0)="101" and cR3/="000000000000000"& CR_2(31 downto 14) then
				conditions(1)<='1';
				        
				
				elsif CR_2(2 downto 0)="110" and cR3<="000000000000000"& CR_2(31 downto 14) then
				conditions(1)<='1';
				else 
				conditions(1)<='0';
				end if;
                      end if;
				
			
			    
				if CR_3(8 downto 4)="00000" then
				cR5:=R0;
				end if;
				if CR_3(8 downto 4)="00001" then 
				cR5:=R1;
				end if;
				if CR_3(8 downto 4)="00010" then   
				cR5:=R2;
				end if;
				if CR_3(8 downto 4)="00011" then  
				cR5:=R3;
				end if;
				if CR_3(8 downto 4)="00100" then
				cR5:=R4;
				end if;
				if CR_3(8 downto 4)="00101" then
				cR5:=R5;
				end if;
				if CR_3(8 downto 4)="00110" then
				cR5:=R6;
				end if;
				if CR_3(8 downto 4)="00111" then      
				cR5:=R7;
				end if;
				if CR_3(8 downto 4)="01000" then
				cR5:=R8;
				end if;
				if CR_3(8 downto 4)="01001" then
				cR5:=R9;
				end if;
				if CR_3(8 downto 4)="01010" then
				cR5:=R10;
				end if;     
				if CR_3(8 downto 4)="01011" then
				cR5:=R11;
				end if;      
				if CR_3(8 downto 4)="01100" then      
				cR5:=R12;
				end if;
				if CR_3(8 downto 4)="01101" then
				cR5:=R13;
				end if;
				if CR_3(8 downto 4)="01110" then
				cR5:=R14;
				end if;
				if CR_3(8 downto 4)="01111" then
				cR5:=R15;
				end if;
				if CR_3(8 downto 4)="10000" then
				cR5:=HOUT1;
				end if;
				if CR_3(8 downto 4)="10001" then
				cR5:=HOUT2;
				end if;
				if CR_3(8 downto 4)="10010" then
				cR5:=HOUT3;
				end if;
				if CR_3(8 downto 4)="10011" then
				cR5:=HOUT4;
				end if;
				if CR_3(13 downto 9)="00000" then
				cR6:=R0;
				end if;
				if CR_3(13 downto 9)="00001" then 
				cR6:=R1;
				end if;
				if CR_3(13 downto 9)="00010" then   
				cR6:=R2;
				end if;
				if CR_3(13 downto 9)="00011" then  
				cR6:=R3;
				end if;
				if CR_3(13 downto 9)="00100" then
				cR6:=R4;
				end if;
				if CR_3(13 downto 9)="00101" then
				cR6:=R5;
				end if;
				if CR_3(13 downto 9)="00110" then
				cR6:=R6;
				end if;
				if CR_3(13 downto 9)="00111" then      
				cR6:=R7;
				end if;
				if CR_3(13 downto 9)="01000" then
				cR6:=R8;
				end if;
				if CR_3(13 downto 9)="01001" then
				cR6:=R9;
				end if;
				if CR_3(13 downto 9)="01010" then
				cR6:=R10;
				end if;     
				if CR_3(13 downto 9)="01011" then
				cR6:=R11;
				end if;      
				if CR_3(13 downto 9)="01100" then      
				cR6:=R12;
				end if;
				if CR_3(13 downto 9)="01101" then
				cR6:=R13;
				end if;
				if CR_3(13 downto 9)="01110" then
				cR6:=R14;
				end if;
				if CR_3(13 downto 9)="01111" then
				cR6:=R15;
				end if;
				if CR_3(13 downto 9)="10000" then
				cR6:=HOUT1;
				end if;
				if CR_3(13 downto 9)="10001" then
				cR6:=HOUT2;
				end if;
				if CR_3(13 downto 9)="10010" then
				cR6:=HOUT3;
				end if;
				if CR_3(13 downto 9)="10011" then
				cR6:=HOUT4;
				end if;
				if CR_3(3)='0' then
				if CR_3(2 downto 0)="000" and cR5>cR6 then
				conditions(2)<='1';
				
				
				elsif CR_3(2 downto 0)="001" and cR5=cR6 then
			  	conditions(2)<='1';
				
				elsif CR_3(2 downto 0)="010" and (not( cR5>cR6))and(not( cR5=cR6 )) then
				conditions(2)<='1';
				
				elsif CR_3(2 downto 0)="100" and cR5>=cR6 then
				conditions(2)<='1';
					
				elsif CR_3(2 downto 0)="101" and cR5/=cR6 then
				conditions(2)<='1';
				        
				
				elsif CR_3(2 downto 0)="110" and cR5<=cR6 then
				conditions(2)<='1';
				else 
				conditions(2)<='0';
				end if;

				else	    
				if CR_3(2 downto 0)="000" and cR5>"000000000000000"& CR_3(31 downto 14) then
				conditions(2)<='1';
				
				
				elsif CR_3(2 downto 0)="001" and cR5="000000000000000"& CR_3(31 downto 14) then
			  	conditions(2)<='1';
				
				elsif CR_3(2 downto 0)="010" and (not(cR5> "000000000000000"& CR_3(31 downto 14)))and(not(cR5= "000000000000000"& CR_3(31 downto 14) )) then
				conditions(2)<='1';
				
				elsif CR_3(2 downto 0)="100" and cR5>="000000000000000"& CR_3(31 downto 14) then
				conditions(2)<='1';
					
				elsif CR_3(2 downto 0)="101" and cR5/="000000000000000"& CR_3(31 downto 14) then
				conditions(2)<='1';
				        
				
				elsif CR_3(2 downto 0)="110" and cR5<="000000000000000"& CR_3(31 downto 14) then
				conditions(2)<='1';
				else 
				conditions(2)<='0';
				end if;
                      end if;
                      
				
				
			     
				if CR_4(8 downto 4)="00000" then
				cR7:=R0;
				end if;
				if CR_4(8 downto 4)="00001" then 
				cR7:=R1;
				end if;
				if CR_4(8 downto 4)="00010" then   
				cR7:=R2;
				end if;
				if CR_4(8 downto 4)="00011" then  
				cR7:=R3;
				end if;
				if CR_4(8 downto 4)="00100" then
				cR7:=R4;
				end if;
				if CR_4(8 downto 4)="00101" then
				cR7:=R5;
				end if;
				if CR_4(8 downto 4)="00110" then
				cR7:=R6;
				end if;
				if CR_4(8 downto 4)="00111" then      
				cR7:=R7;
				end if;
				if CR_4(8 downto 4)="01000" then
				cR7:=R8;
				end if;
				if CR_4(8 downto 4)="01001" then
				cR7:=R9;
				end if;
				if CR_4(8 downto 4)="01010" then
				cR7:=R10;
				end if;     
				if CR_4(8 downto 4)="01011" then
				cR7:=R11;
				end if;      
				if CR_4(8 downto 4)="01100" then      
				cR7:=R12;
				end if;
				if CR_4(8 downto 4)="01101" then
				cR7:=R13;
				end if;
				if CR_4(8 downto 4)="01110" then
				cR7:=R14;
				end if;
				if CR_4(8 downto 4)="01111" then
				cR7:=R15;
				end if;
				if CR_4(8 downto 4)="10000" then
				cR7:=HOUT1;
				end if;
				if CR_4(8 downto 4)="10001" then
				cR7:=HOUT2;
				end if;
				if CR_4(8 downto 4)="10010" then
				cR7:=HOUT3;
				end if;
				if CR_4(8 downto 4)="10011" then
				cR7:=HOUT4;
				end if;
				if CR_4(12 downto 8)="00000" then
				cR8:=R0;
				end if;
				if CR_4(12 downto 8)="00001" then 
				cR8:=R1;
				end if;
				if CR_4(12 downto 8)="00010" then   
				cR8:=R2;
				end if;
				if CR_4(13 downto 9)="00011" then  
				cR8:=R3;
				end if;
				if CR_4(13 downto 9)="00100" then
				cR8:=R4;
				end if;
				if CR_4(13 downto 9)="00101" then
				cR8:=R5;
				end if;
				if CR_4(13 downto 9)="00110" then
				cR8:=R6;
				end if;
				if CR_4(13 downto 9)="00111" then      
				cR8:=R7;
				end if;
				if CR_4(13 downto 9)="01000" then
				cR8:=R8;
				end if;
				if CR_4(13 downto 9)="01001" then
				cR8:=R9;
				end if;
				if CR_4(13 downto 9)="01010" then
				cR8:=R10;
				end if;     
				if CR_4(13 downto 9)="01011" then
				cR8:=R11;
				end if;      
				if CR_4(13 downto 9)="01100" then      
				cR8:=R12;
				end if;
				if CR_4(13 downto 9)="01101" then
				cR8:=R13;
				end if;
				if CR_4(13 downto 9)="01110" then
				cR8:=R14;
				end if;
				if CR_4(13 downto 9)="01111" then
				cR8:=R15;
				end if;
				if CR_4(13 downto 9)="10000" then
				cR8:=HOUT1;
				end if;
				if CR_4(13 downto 9)="10001" then
				cR8:=HOUT2;
				end if;
				if CR_4(13 downto 9)="10010" then
				cR8:=HOUT3;
				end if;
				if CR_4(13 downto 9)="10011" then
				cR8:=HOUT4;
				end if;
				if CR_4(3)='0' then
				if CR_4(2 downto 0)="000" and cR7>cR8 then
				conditions(3)<='1';
				
				
				elsif CR_4(2 downto 0)="001" and cR7=cR8 then
			  	conditions(3)<='1';
				
				elsif CR_4(2 downto 0)="010" and (not( cR7>cR8))and(not( cR7=cR8 )) then
				conditions(3)<='1';
				
				elsif CR_4(2 downto 0)="100" and cR7>=cR8 then
				conditions(3)<='1';
					
				elsif CR_4(2 downto 0)="101" and cR7/=cR8 then
				conditions(3)<='1';
				        
				
				elsif CR_4(2 downto 0)="110" and cR7<=cR8 then
				conditions(3)<='1';
				else 
				conditions(3)<='0';
				end if;

				else	    
				if CR_4(2 downto 0)="000" and cR7>"000000000000000"& CR_4(31 downto 14) then
				conditions(3)<='1';
				
				
				elsif CR_4(2 downto 0)="001" and cR7="000000000000000"& CR_4(31 downto 14) then
			  	conditions(3)<='1';
				
				elsif CR_4(2 downto 0)="010" and (not(cR7> "000000000000000"& CR_4(31 downto 14)))and(not(cR7= "000000000000000"& CR_4(31 downto 14) )) then
				conditions(3)<='1';
				
				elsif CR_4(2 downto 0)="100" and cR7>="000000000000000"& CR_4(31 downto 14) then
				conditions(3)<='1';
					
				elsif CR_4(2 downto 0)="101" and cR7/="000000000000000"& CR_4(31 downto 14) then
				conditions(3)<='1';
				        
				
				elsif CR_4(2 downto 0)="110" and cR7<="000000000000000"& CR_4(31 downto 14) then
				conditions(3)<='1';
				else 
				conditions(3)<='0';
				end if;
                      end if;
				if CR_5(8 downto 4)="00000" then
				cR9:=R0;
				end if;
				if CR_5(8 downto 4)="00001" then 
				cR9:=R1;
				end if;
				if CR_5(8 downto 4)="00010" then   
				cR9:=R2;
				end if;
				if CR_5(8 downto 4)="00011" then  
				cR9:=R3;
				end if;
				if CR_5(8 downto 4)="00100" then
				cR9:=R4;
				end if;
				if CR_5(8 downto 4)="00101" then
				cR9:=R5;
				end if;
				if CR_5(8 downto 4)="00110" then
				cR9:=R6;
				end if;
				if CR_5(8 downto 4)="00111" then      
				cR9:=R7;
				end if;
				if CR_5(8 downto 4)="01000" then
				cR9:=R8;
				end if;
				if CR_5(8 downto 4)="01001" then
				cR9:=R9;
				end if;
				if CR_5(8 downto 4)="01010" then
				cR9:=R10;
				end if;     
				if CR_5(8 downto 4)="01011" then
				cR9:=R11;
				end if;      
				if CR_5(8 downto 4)="01100" then      
				cR9:=R12;
				end if;
				if CR_5(8 downto 4)="01101" then
				cR9:=R13;
				end if;
				if CR_5(8 downto 4)="01110" then
				cR9:=R14;
				end if;
				if CR_5(8 downto 4)="01111" then
				cR9:=R15;
				end if;
				if CR_5(8 downto 4)="10000" then
				cR9:=HOUT1;
				end if;
				if CR_5(8 downto 4)="10001" then
				cR9:=HOUT2;
				end if;
				if CR_5(8 downto 4)="10010" then
				cR9:=HOUT3;
				end if;
				if CR_5(8 downto 4)="10011" then
				cR9:=HOUT4;
				end if;
				if CR_5(12 downto 8)="00000" then
				cR10:=R0;
				end if;
				if CR_5(12 downto 8)="00001" then 
				cR10:=R1;
				end if;
				if CR_5(12 downto 8)="00010" then   
				cR10:=R2;
				end if;
				if CR_5(13 downto 9)="00011" then  
				cR10:=R3;
				end if;
				if CR_5(13 downto 9)="00100" then
				cR10:=R4;
				end if;
				if CR_5(13 downto 9)="00101" then
				cR10:=R5;
				end if;
				if CR_5(13 downto 9)="00110" then
				cR10:=R6;
				end if;
				if CR_5(13 downto 9)="00111" then      
				cR10:=R7;
				end if;
				if CR_5(13 downto 9)="01000" then
				cR10:=R8;
				end if;
				if CR_5(13 downto 9)="01001" then
				cR10:=R9;
				end if;
				if CR_5(13 downto 9)="01010" then
				cR10:=R10;
				end if;     
				if CR_5(13 downto 9)="01011" then
				cR10:=R11;
				end if;      
				if CR_5(13 downto 9)="01100" then      
				cR10:=R12;
				end if;
				if CR_5(13 downto 9)="01101" then
				cR10:=R13;
				end if;
				if CR_5(13 downto 9)="01110" then
				cR10:=R14;
				end if;
				if CR_5(13 downto 9)="01111" then
				cR10:=R15;
				end if;
				if CR_5(13 downto 9)="10000" then
				cR10:=HOUT1;
				end if;
				if CR_5(13 downto 9)="10001" then
				cR10:=HOUT2;
				end if;
				if CR_5(13 downto 9)="10010" then
				cR10:=HOUT3;
				end if;
				if CR_5(13 downto 9)="10011" then
				cR10:=HOUT4;
				end if;
				if CR_5(3)='0' then
				if CR_5(2 downto 0)="000" and cR9>cR10 then
				conditions(4)<='1';
				
				
				elsif CR_5(2 downto 0)="001" and cR9=cR10 then
			  	conditions(4)<='1';
				
				elsif CR_5(2 downto 0)="010" and (not( cR9>cR10))and(not( cR9=cR10 )) then
				conditions(4)<='1';
				
				elsif CR_5(2 downto 0)="100" and cR9>=cR10 then
				conditions(4)<='1';
					
				elsif CR_5(2 downto 0)="101" and cR9/=cR10 then
				conditions(4)<='1';
				        
				
				elsif CR_5(2 downto 0)="110" and cR9<=cR10 then
				conditions(4)<='1';
				else 
				conditions(4)<='0';
				end if;

				else	    
				if CR_5(2 downto 0)="000" and cR9>"000000000000000"& CR_5(31 downto 14) then
				conditions(4)<='1';
				
				
				elsif CR_5(2 downto 0)="001" and cR9="000000000000000"& CR_5(31 downto 14) then
			  	conditions(4)<='1';
				
				elsif CR_5(2 downto 0)="010" and (not(cR9> "000000000000000"& CR_5(31 downto 14)))and(not(cR9= "000000000000000"& CR_5(31 downto 14) )) then
				conditions(4)<='1';
				
				elsif CR_5(2 downto 0)="100" and cR9>="000000000000000"& CR_5(31 downto 14) then
				conditions(4)<='1';
					
				elsif CR_5(2 downto 0)="101" and cR9/="000000000000000"& CR_5(31 downto 14) then
				conditions(4)<='1';
				        
				
				elsif CR_5(2 downto 0)="110" and cR9<="000000000000000"& CR_5(31 downto 14) then
				conditions(4)<='1';
				else 
				conditions(4)<='0';
				end if;
                      end if;
                      
				if CR_6(8 downto 4)="00000" then
				cR11:=R0;
				end if;
				if CR_6(8 downto 4)="00001" then 
				cR11:=R1;
				end if;
				if CR_6(8 downto 4)="00010" then   
				cR11:=R2;
				end if;
				if CR_6(8 downto 4)="00011" then  
				cR11:=R3;
				end if;
				if CR_6(8 downto 4)="00100" then
				cR11:=R4;
				end if;
				if CR_6(8 downto 4)="00101" then
				cR11:=R5;
				end if;
				if CR_6(8 downto 4)="00110" then
				cR11:=R6;
				end if;
				if CR_6(8 downto 4)="00111" then      
				cR11:=R7;
				end if;
				if CR_6(8 downto 4)="01000" then
				cR11:=R8;
				end if;
				if CR_6(8 downto 4)="01001" then
				cR11:=R9;
				end if;
				if CR_6(8 downto 4)="01010" then
				cR11:=R10;
				end if;     
				if CR_6(8 downto 4)="01011" then
				cR11:=R11;
				end if;      
				if CR_6(8 downto 4)="01100" then      
				cR11:=R12;
				end if;
				if CR_6(8 downto 4)="01101" then
				cR11:=R13;
				end if;
				if CR_6(8 downto 4)="01110" then
				cR11:=R14;
				end if;
				if CR_6(8 downto 4)="01111" then
				cR11:=R15;
				end if;
				if CR_6(8 downto 4)="10000" then
				cR11:=HOUT1;
				end if;
				if CR_6(8 downto 4)="10001" then
				cR11:=HOUT2;
				end if;
				if CR_6(8 downto 4)="10010" then
				cR11:=HOUT3;
				end if;
				if CR_6(8 downto 4)="10011" then
				cR11:=HOUT4;
				end if;
				if CR_6(12 downto 8)="00000" then
				cR12:=R0;
				end if;
				if CR_6(12 downto 8)="00001" then 
				cR12:=R1;
				end if;
				if CR_6(12 downto 8)="00010" then   
				cR12:=R2;
				end if;
				if CR_6(13 downto 9)="00011" then  
				cR12:=R3;
				end if;
				if CR_6(13 downto 9)="00100" then
				cR12:=R4;
				end if;
				if CR_6(13 downto 9)="00101" then
				cR12:=R5;
				end if;
				if CR_6(13 downto 9)="00110" then
				cR12:=R6;
				end if;
				if CR_6(13 downto 9)="00111" then      
				cR12:=R7;
				end if;
				if CR_6(13 downto 9)="01000" then
				cR12:=R8;
				end if;
				if CR_6(13 downto 9)="01001" then
				cR12:=R9;
				end if;
				if CR_6(13 downto 9)="01010" then
				cR12:=R10;
				end if;     
				if CR_6(13 downto 9)="01011" then
				cR12:=R11;
				end if;      
				if CR_6(13 downto 9)="01100" then      
				cR12:=R12;
				end if;
				if CR_6(13 downto 9)="01101" then
				cR12:=R13;
				end if;
				if CR_6(13 downto 9)="01110" then
				cR12:=R14;
				end if;
				if CR_6(13 downto 9)="01111" then
				cR12:=R15;
				end if;
				if CR_6(13 downto 9)="10000" then
				cR12:=HOUT1;
				end if;
				if CR_6(13 downto 9)="10001" then
				cR12:=HOUT2;
				end if;
				if CR_6(13 downto 9)="10010" then
				cR12:=HOUT3;
				end if;
				if CR_6(13 downto 9)="10011" then
				cR12:=HOUT4;
				end if;
				if CR_6(3)='0' then
				if CR_6(2 downto 0)="000" and cR11>cR12 then
				conditions(5)<='1';
				
				
				elsif CR_6(2 downto 0)="001" and cR11=cR12 then
			  	conditions(5)<='1';
				
				elsif CR_6(2 downto 0)="010" and (not( cR11>cR12))and(not( cR11=cR12 )) then
				conditions(5)<='1';
				
				elsif CR_6(2 downto 0)="100" and cR11>=cR12 then
				conditions(5)<='1';
					
				elsif CR_6(2 downto 0)="101" and cR11/=cR12 then
				conditions(5)<='1';
				        
				
				elsif CR_6(2 downto 0)="110" and cR11<=cR12 then
				conditions(5)<='1';
				else 
				conditions(5)<='0';
				end if;

				else	    
				if CR_6(2 downto 0)="000" and cR11>"000000000000000"& CR_6(31 downto 14) then
				conditions(5)<='1';
				
				
				elsif CR_6(2 downto 0)="001" and cR11= "000000000000000"& CR_6(31 downto 14) then
			  	conditions(5)<='1';
				
				elsif CR_6(2 downto 0)="010" and (not(cR11> "000000000000000"& CR_6(31 downto 14)))and(not(cR11= "000000000000000"& CR_6(31 downto 14) )) then
				conditions(5)<='1';
				
				elsif CR_6(2 downto 0)="100" and cR11>= "000000000000000"& CR_6(31 downto 14) then
				conditions(5)<='1';
					
				elsif CR_6(2 downto 0)="101" and cR11/="000000000000000"& CR_6(31 downto 14) then
				conditions(5)<='1';
				        
				
				elsif CR_6(2 downto 0)="110" and cR11<="000000000000000"& CR_6(31 downto 14) then
				conditions(5)<='1';
				else 
				conditions(5)<='0';
				end if;
                      end if;
				
				if CR_7(8 downto 4)="00000" then
				cR13:=R0;
				end if;
				if CR_7(8 downto 4)="00001" then 
				cR13:=R1;
				end if;
				if CR_7(8 downto 4)="00010" then   
				cR13:=R2;
				end if;
				if CR_7(8 downto 4)="00011" then  
				cR13:=R3;
				end if;
				if CR_7(8 downto 4)="00100" then
				cR13:=R4;
				end if;
				if CR_7(8 downto 4)="00101" then
				cR13:=R5;
				end if;
				if CR_7(8 downto 4)="00110" then
				cR13:=R6;
				end if;
				if CR_7(8 downto 4)="00111" then      
				cR13:=R7;
				end if;
				if CR_7(8 downto 4)="01000" then
				cR13:=R8;
				end if;
				if CR_7(8 downto 4)="01001" then
				cR13:=R9;
				end if;
				if CR_7(8 downto 4)="01010" then
				cR13:=R10;
				end if;     
				if CR_7(8 downto 4)="01011" then
				cR13:=R11;
				end if;      
				if CR_7(8 downto 4)="01100" then      
				cR13:=R12;
				end if;
				if CR_7(8 downto 4)="01101" then
				cR13:=R13;
				end if;
				if CR_7(8 downto 4)="01110" then
				cR13:=R14;
				end if;
				if CR_7(8 downto 4)="01111" then
				cR13:=R15;
				end if;
				if CR_7(8 downto 4)="10000" then
				cR13:=HOUT1;
				end if;
				if CR_7(8 downto 4)="10001" then
				cR13:=HOUT2;
				end if;
				if CR_7(8 downto 4)="10010" then
				cR13:=HOUT3;
				end if;
				if CR_7(8 downto 4)="10011" then
				cR13:=HOUT4;
				end if;
				if CR_7(12 downto 8)="00000" then
				cR14:=R0;
				end if;
				if CR_7(12 downto 8)="00001" then 
				cR14:=R1;
				end if;
				if CR_7(12 downto 8)="00010" then   
				cR14:=R2;
				end if;
				if CR_7(13 downto 9)="00011" then  
				cR14:=R3;
				end if;
				if CR_7(13 downto 9)="00100" then
				cR14:=R4;
				end if;
				if CR_7(13 downto 9)="00101" then
				cR14:=R5;
				end if;
				if CR_7(13 downto 9)="00110" then
				cR14:=R6;
				end if;
				if CR_7(13 downto 9)="00111" then      
				cR14:=R7;
				end if;
				if CR_7(13 downto 9)="01000" then
				cR14:=R8;
				end if;
				if CR_7(13 downto 9)="01001" then
				cR14:=R9;
				end if;
				if CR_7(13 downto 9)="01010" then
				cR14:=R10;
				end if;     
				if CR_7(13 downto 9)="01011" then
				cR14:=R11;
				end if;      
				if CR_7(13 downto 9)="01100" then      
				cR14:=R12;
				end if;
				if CR_7(13 downto 9)="01101" then
				cR14:=R13;
				end if;
				if CR_7(13 downto 9)="01110" then
				cR14:=R14;
				end if;
				if CR_7(13 downto 9)="01111" then
				cR14:=R15;
				end if;
				if CR_7(13 downto 9)="10000" then
				cR14:=HOUT1;
				end if;
				if CR_7(13 downto 9)="10001" then
				cR14:=HOUT2;
				end if;
				if CR_7(13 downto 9)="10010" then
				cR14:=HOUT3;
				end if;
				if CR_7(13 downto 9)="10011" then
				cR14:=HOUT4;
				end if;
				if CR_7(3)='0' then
				if CR_7(2 downto 0)="000" and cR13>cR14 then
				conditions(6)<='1';
				
				
				elsif CR_7(2 downto 0)="001" and cR13=cR14 then
			  	conditions(6)<='1';
				
				elsif CR_7(2 downto 0)="010" and (not( cR13>cR14))and(not( cR13=cR14 )) then
				conditions(6)<='1';
				
				elsif CR_7(2 downto 0)="100" and cR13>=cR14 then
				conditions(6)<='1';
					
				elsif CR_7(2 downto 0)="101" and cR13/=cR14 then
				conditions(6)<='1';
				        
				
				elsif CR_7(2 downto 0)="110" and cR13<=cR14 then
				conditions(6)<='1';
				else 
				conditions(6)<='0';
				end if;

				else	    
				if CR_7(2 downto 0)="000" and cR13>"000000000000000"& CR_7(31 downto 14) then
				conditions(6)<='1';
				
				
				elsif CR_7(2 downto 0)="001" and cR13="000000000000000"& CR_7(31 downto 14) then
			  	conditions(6)<='1';
				
				elsif CR_7(2 downto 0)="010" and (not( cR13>"000000000000000"& CR_7(31 downto 14)))and(not(cR13= "000000000000000"& CR_7(31 downto 14) )) then
				conditions(6)<='1';
				
				elsif CR_7(2 downto 0)="100" and cR13>="000000000000000"& CR_7(31 downto 14) then
				conditions(6)<='1';
					
				elsif CR_7(2 downto 0)="101" and cR13/="000000000000000"& CR_7(31 downto 14) then
				conditions(6)<='1';
				        
				
				elsif CR_7(2 downto 0)="110" and cR13<="000000000000000"& CR_7(31 downto 14) then
				conditions(6)<='1';
				else 
				conditions(6)<='0';
				end if;
                      end if;
			
				if CR_8(8 downto 4)="00000" then
				cR15:=R0;
				end if;
				if CR_8(8 downto 4)="00001" then 
				cR15:=R1;
				end if;
				if CR_8(8 downto 4)="00010" then   
				cR15:=R2;
				end if;
				if CR_8(8 downto 4)="00011" then  
				cR15:=R3;
				end if;
				if CR_8(8 downto 4)="00100" then
				cR15:=R4;
				end if;
				if CR_8(8 downto 4)="00101" then
				cR15:=R5;
				end if;
				if CR_8(8 downto 4)="00110" then
				cR15:=R6;
				end if;
				if CR_8(8 downto 4)="00111" then      
				cR15:=R7;
				end if;
				if CR_8(8 downto 4)="01000" then
				cR15:=R8;
				end if;
				if CR_8(8 downto 4)="01001" then
				cR15:=R9;
				end if;
				if CR_8(8 downto 4)="01010" then
				cR15:=R10;
				end if;     
				if CR_8(8 downto 4)="01011" then
				cR15:=R11;
				end if;      
				if CR_8(8 downto 4)="01100" then      
				cR15:=R12;
				end if;
				if CR_8(8 downto 4)="01101" then
				cR15:=R13;
				end if;
				if CR_8(8 downto 4)="01110" then
				cR15:=R14;
				end if;
				if CR_8(8 downto 4)="01111" then
				cR15:=R15;
				end if;
				if CR_8(8 downto 4)="10000" then
				cR15:=HOUT1;
				end if;
				if CR_8(8 downto 4)="10001" then
				cR15:=HOUT2;
				end if;
				if CR_8(8 downto 4)="10010" then
				cR15:=HOUT3;
				end if;
				if CR_8(8 downto 4)="10011" then
				cR15:=HOUT4;
				end if;
				if CR_8(12 downto 8)="00000" then
				cR16:=R0;
				end if;
				if CR_8(12 downto 8)="00001" then 
				cR16:=R1;
				end if;
				if CR_8(12 downto 8)="00010" then   
				cR16:=R2;
				end if;
				if CR_8(13 downto 9)="00011" then  
				cR16:=R3;
				end if;
				if CR_8(13 downto 9)="00100" then
				cR16:=R4;
				end if;
				if CR_8(13 downto 9)="00101" then
				cR16:=R5;
				end if;
				if CR_8(13 downto 9)="00110" then
				cR16:=R6;
				end if;
				if CR_8(13 downto 9)="00111" then      
				cR16:=R7;
				end if;
				if CR_8(13 downto 9)="01000" then
				cR16:=R8;
				end if;
				if CR_8(13 downto 9)="01001" then
				cR16:=R9;
				end if;
				if CR_8(13 downto 9)="01010" then
				cR16:=R10;
				end if;     
				if CR_8(13 downto 9)="01011" then
				cR16:=R11;
				end if;      
				if CR_8(13 downto 9)="01100" then      
				cR16:=R12;
				end if;
				if CR_8(13 downto 9)="01101" then
				cR16:=R13;
				end if;
				if CR_8(13 downto 9)="01110" then
				cR16:=R14;
				end if;
				if CR_8(13 downto 9)="01111" then
				cR16:=R15;
				end if;
				if CR_8(13 downto 9)="10000" then
				cR16:=HOUT1;
				end if;
				if CR_8(13 downto 9)="10001" then
				cR16:=HOUT2;
				end if;
				if CR_8(13 downto 9)="10010" then
				cR16:=HOUT3;
				end if;
				if CR_8(13 downto 9)="10011" then
				cR16:=HOUT4;
				end if;
				if CR_8(3)='0' then
				if CR_8(2 downto 0)="000" and cR15>cR16 then
				conditions(7)<='1';
				
				
				elsif CR_8(2 downto 0)="001" and cR15=cR16 then
			  	conditions(7)<='1';
				
				elsif CR_8(2 downto 0)="010" and (not( cR15>cR16))and(not( cR15=cR16 )) then
				conditions(7)<='1';
				
				elsif CR_8(2 downto 0)="100" and cR15>=cR16 then
				conditions(7)<='1';
					
				elsif CR_8(2 downto 0)="101" and cR15/=cR16 then
				conditions(7)<='1';
				        
				
				elsif CR_8(2 downto 0)="110" and cR15<=cR16 then
				conditions(7)<='1';
				else 
				conditions(7)<='0';
				end if;

				else	    
				if CR_8(2 downto 0)="000" and cR15>"000000000000000"& CR_8(31 downto 14) then
				conditions(7)<='1';
				
				
				elsif CR_8(2 downto 0)="001" and cR15="000000000000000"& CR_8(31 downto 14) then
			  	conditions(7)<='1';
				
				elsif CR_8(2 downto 0)="010" and (not( cR15>"000000000000000"& CR_8(31 downto 14)))and(not(cR15= "000000000000000"& CR_8(31 downto 14) )) then
				conditions(7)<='1';
				
				elsif CR_8(2 downto 0)="100" and cR15>="000000000000000"& CR_8(31 downto 14) then
				conditions(7)<='1';
					
				elsif CR_8(2 downto 0)="101" and cR15/="000000000000000"& CR_8(31 downto 14) then
				conditions(7)<='1';
				        
				
				elsif CR_8(2 downto 0)="110" and cR15<="000000000000000"& CR_8(31 downto 14) then
				conditions(7)<='1';
				else 
				conditions(7)<='0';
				end if;
                      end if;
		     end if;
		end if;
end if;
end process;




--enble_tcam<='0' when src_if=x"00" else '1';
--conditions(31 downto 24) <=present_state(7 downto 0);
--    conditions(23)<= timer; --'1' when src_if=x"ff" else '0';
--    conditions(22)<= pkt; --'1' when src_if=x"01" else '0';
--    conditions(21 downto 10)<= "000000000000";
--    conditions(9)<= '1' when (HOUT1>SOUT1) else '0';
--    conditions(8)<= '1' when (HOUT1=SOUT1) else '0';
--    conditions(7)<= '1' when (SOUT1=SOUT2) else '0';
--    conditions(6)<= '1' when (SOUT1>SOUT2) else '0';
--    conditions(5)<= '1' when (SOUT3=SOUT4) else '0';
--    conditions(4)<= '1' when (SOUT3>SOUT4) else '0';
--    conditions(3)<= '1' when (SOUT1=SOUT4) else '0';
--    conditions(2)<= '1' when (SOUT1>SOUT4) else '0';
--    conditions(1)<= '1' when (SOUT1=SOUT3) else '0';
--    conditions(0)<= '1' when (SOUT1>SOUT3) else '0';
--conditions(31 downto 24) <= present_state(7 downto 0);
--conditions(23) <=timer;
--conditions(22)<= pkt;
--conditions(21)<= '1' when (R1>R0) else '0';
--conditions(20)<= '1' when (R1=R0) else '0';
--conditions(19)<= '1' when (R1>HOUT1) else '0';
--conditions(18)<= '1' when (R1=HOUT1) else '0';
--conditions(17)<= '1' when (R2>HOUT2) else '0';
--conditions(16)<= '1' when (R2=HOUT2) else '0';
--conditions(15)<= '1' when (R3>HOUT3) else '0';
--conditions(14)<= '1' when (R3=HOUT3) else '0';
--conditions(13)<= '1' when (R4>HOUT4) else '0';
--conditions(12)<= '1' when (R4=HOUT4) else '0';
--conditions(11)<= '1' when (R2>R0) else '0';
--conditions(10)<= '1' when (R2=R0) else '0';
--conditions(9)<= '1' when (R2>R1) else '0';
--conditions(8)<= '1' when (R2=R1) else '0';
--conditions(7)<= '1' when (R3>R0) else '0';
--conditions(6)<= '1' when (R3=R0) else '0';
--conditions(5)<= '1' when (R3>R1) else '0';
--conditions(4)<= '1' when (R3=R1) else '0';
--conditions(3)<= '1' when (R3>R2) else '0';
--conditions(2)<= '1' when (R3=R2) else '0';
--conditions(1)<= '1' when (R4>R0) else '0';
--conditions(0)<= '1' when (R4=R0) else '0';
--conditions(21 downto 0)<=(others=>'0');

conditions_t<=conditions;


process(S0_AXIS_ACLK, S0_AXIS_ARESETN)
	begin
		if(S0_AXIS_ACLK' event and S0_AXIS_ACLK ='1') then
			if(S0_AXIS_ARESETN = '0') then
				conditions_reg<=x"00000000";
				conditions_reg_1<=x"00000000";
		
			if (curr_state=PKT2) THEN
				conditions_reg_1<=conditions;
			else
				conditions_reg<=conditions;
			end if;
		end if;
	end if;
end process;





	--- registri per mux azioni
process(S0_AXIS_ACLK, S0_AXIS_ARESETN)
	begin
		if (S0_AXIS_ACLK'event and S0_AXIS_ACLK = '1') then
			if (S0_AXIS_ARESETN = '0') then
				RHOUT1 <=x"00000000";
				RHOUT2 <=x"00000000";
				RHOUT3 <=x"00000000";
				RHOUT4 <=x"00000000";
			else
			  
			  if (curr_state=PKT1_wait) then
				RHOUT1 <=HOUT1;
				RHOUT2 <=HOUT2;
				RHOUT3 <=HOUT3;
				RHOUT4 <=HOUT4;
			 elsif(curr_state=reset_all) then
			      RHOUT1 <=x"00000000";
                  RHOUT2 <=x"00000000";
                  RHOUT3 <=x"00000000";
                  RHOUT4 <=x"00000000";
			end if;
		end if;
		end if;
end process;


Tcam1bis: entity cam.cam_top
		generic map (
				C_DEPTH =>32,
				C_WIDTH =>32,
				C_MEM_INIT_FILE  => "/home/aniellocamma/NetFPGA-SUME-live/projects/demo-opp/opp4.src/init_cam_2.mif"
			    )
		port map(
				CLK => S_AXI_ACLK,
				CMP_DIN =>  conditions_t,
				CMP_DATA_MASK => (others=>'0'),            --x"00000000000000000000000000000000",
				BUSY => open,
				MATCH  => match,
				MATCH_ADDR  => match_addr_tcam(4 downto 0),
				WE       =>   cam1_we,
				WR_ADDR  => address(6 downto 2),
				DATA_MASK => tcam_mask,
				DIN=> S_AXI_WDATA,
				EN  => '1'
			);




---future state

rdp_fs:  entity work.ram32x32dp
		generic map (init_file => "/home/aniellocamma/Documenti/opp-tcp_copia/opp-tcp.srcs/sources/init_ram_1.mif")
		port map
		(

		 --AXI interface
		 axi_clock => S_AXI_ACLK,
		 we =>we_r0,
		 axi_addr => address(6 downto 2),
		 axi_data_in => S_AXI_WDATA,
		 axi_data_out => read_from_ram0,

		 -- AXIS interface
		 clock => S0_AXIS_ACLK,
		 addr    => match_addr_tcam(4 downto 0),
		 data_out => future_state
		 );



-- Azioni slot 0

ram_generate_1: for i in 0 to 8 generate
begin
rdpa:  entity work.ram32x32dp
		generic map (init_file => "/home/aniellocamma/Documenti/opp-tcp_copia/opp-tcp.srcs/sources/init_ram_1.mif")
		port map
		(

		 --AXI interface
		 axi_clock => S_AXI_ACLK,
		 we => we_rdp_s0(i),
		 axi_addr => address(6 downto 2),
		 axi_data_in => S_AXI_WDATA,
		 axi_data_out => read_from_ram_s0(i),

		 -- AXIS interface
		 clock => S0_AXIS_ACLK,
		 addr    => match_addr_tcam(4 downto 0),
		 data_out => action_s0(i)
		 );
		 
end generate;


-- Azioni slot 1


ram_generate_2: for i in 0 to 8 generate
begin
rdpa:  entity work.ram32x32dp
		generic map (init_file => "/home/aniellocamma/Documenti/opp-tcp_copia/opp-tcp.srcs/sources/init_ram_1.mif")
		port map
		(

		 --AXI interface
		 axi_clock => S_AXI_ACLK,
		 we =>we_rdp_s1(i),
		 axi_addr => address(6 downto 2),
		 axi_data_in => S_AXI_WDATA,
		 axi_data_out => read_from_ram_s1(i),

		 -- AXIS interface
		 clock => S0_AXIS_ACLK,
		 addr    => match_addr_tcam(4 downto 0),
		 data_out => action_s1(i)
		 );
end generate;


-- Azioni slot 2


ram_generate_3: for i in 0 to 8 generate
begin
rdpa:  entity work.ram32x32dp
		generic map (init_file => "/home/aniellocamma/Documenti/opp-tcp_copia/opp-tcp.srcs/sources/init_ram_1.mif")
		port map
		(

		 --AXI interface
		 axi_clock => S_AXI_ACLK,
		 we =>we_rdp_s2(i),
		 axi_addr => address(6 downto 2),
		 axi_data_in => S_AXI_WDATA,
		 axi_data_out => read_from_ram_s2(i),

		 -- AXIS interface
		 clock => S0_AXIS_ACLK,
		 addr    => match_addr_tcam(4 downto 0),
		 data_out => action_s2(i)
		 );
end generate;


process(S0_AXIS_ACLK, S0_AXIS_ARESETN)
	begin
		if (S0_AXIS_ACLK'event and S0_AXIS_ACLK = '1') then
			if (S0_AXIS_ARESETN = '0') then
				match_addr_tcam_t<=x"00";
			else
				match_addr_tcam_t<=match_addr_tcam;
			end if;
		end if;

end process;




	--- logica next state
Next_State_PROCESS: process(S0_AXIS_ACLK, S0_AXIS_ARESETN)
	begin
	if (S0_AXIS_ACLK'event and S0_AXIS_ACLK = '1') then
		if (S0_AXIS_ARESETN = '0') then
			present_state<=x"000000FF";
		else
			if (execute='1' and curr_state=PKT5 and match='1') then
				present_state<=future_state;
	        elsif(curr_state=reset_all) then
	            present_state<=x"000000FF";
			end if;
		end if;
	end if;
end process;



	-- slot decoding                     PKT3                                  PKT4
	--action1<= action1s0 when (curr_state=PKT4) else action1s1 when (curr_state=PKT5) else x"00000000" ;
	--action2<= action2s0 when (curr_state=PKT4) else action2s1 when (curr_state=PKT5) else x"00000000" ;
	--action3<= action3s0 when (curr_state=PKT4) else action3s1 when (curr_state=PKT5) else x"00000000" ;

process(S0_AXIS_ACLK, S0_AXIS_ARESETN)
	begin
		if (S0_AXIS_ACLK'event and S0_AXIS_ACLK = '1') then
			if (S0_AXIS_ARESETN = '0') then
				action0<=x"00000000";
				action1<=x"00000000";
				action2<=x"00000000";
				action3<=x"00000000";
				action4<=x"00000000";
				action5<=x"00000000";
				action6<=x"00000000";
				action7<=x"00000000";
				action8<=x"00000000";
		    elsif ( curr_state= reset_all) then
		        action0<=x"00000000";
                action1<=x"00000000";
                action2<=x"00000000";
                action3<=x"00000000";
                action4<=x"00000000";
                action5<=x"00000000";
                action6<=x"00000000";
                action7<=x"00000000";
                action8<=x"00000000";
	        
			elsif ( curr_state=PKT3_wait) then
				action0<= action_s0(0);
				action1<= action_s0(1);
				action2<= action_s0(2);
				action3<= action_s0(3);
				action4<= action_s0(4);
				action5<= action_s0(5);
				action6<= action_s0(6);
				action7<= action_s0(7);
				action8<= action_s0(8);
			elsif (curr_state=PKT4) then
				action0<= action_s1(0);
				action1<= action_s1(1);
				action2<= action_s1(2);
				action3<= action_s1(3);
				action4<= action_s1(4);
				action5<= action_s1(5);
				action6<= action_s1(6);
				action7<= action_s1(7);
				action8<= action_s1(8);
		   elsif (curr_state= PKT5 or curr_state=PKT4_wait ) then
				action0<= action_s2(0);
				action1<= action_s2(1);
				action2<= action_s2(2);
				action3<= action_s2(3);
				action4<= action_s2(4);
				action5<= action_s2(5);
				action6<= action_s2(6);
				action7<= action_s2(7);
				action8<= action_s2(8);
			end if;
		end if;
end process;



action1comb <= action_s0(1) when (curr_state=PKT3_wait) else 
               action_s1(1) when (curr_state=PKT4) else
               action_s2(1);
action2comb <= action_s0(2) when (curr_state=PKT3_wait) else 
               action_s1(2) when (curr_state=PKT4) else
               action_s2(2);
action3comb <= action_s0(3) when (curr_state=PKT3_wait) else 
               action_s1(3) when (curr_state=PKT4) else
               action_s2(3);
action4comb <= action_s0(4) when (curr_state=PKT3_wait) else 
               action_s1(4) when (curr_state=PKT4) else
               action_s2(4);
action5comb <= action_s0(5) when (curr_state=PKT3_wait) else 
               action_s1(5) when (curr_state=PKT4) else
               action_s2(5);
action6comb <= action_s0(6) when (curr_state=PKT3_wait) else 
               action_s1(6) when (curr_state=PKT4) else
               action_s2(6);
action7comb <= action_s0(7) when (curr_state=PKT3_wait) else 
               action_s1(7) when (curr_state=PKT4) else
               action_s2(7);
action8comb <= action_s0(8) when (curr_state=PKT3_wait) else 
               action_s1(8) when (curr_state=PKT4) else
               action_s2(8);




MA1:  entity work.mux32clk2 port map(
		       clk => S0_AXIS_ACLK,
		       resetn => S0_AXIS_ARESETN,
		       R0=>R0,
		       R1=>R1,
		       R2=>R2,
		       R3=>R3,
		       R4=>R4,
		       R5=>R5,
		       R6=>R6,
		       R7=>R7,
		       R8=>R8,
		       R9=>R9,
		       R10=>R10,
		       R11=>R11,
		       R12=>R12,
		       R13=>R13,
		       R14=>R14,
		       R15=>R15,
		       SOUTa=>SOUTA1a,
		       SOUTb=>SOUTA1b,
		       Conf=>action1comb(23 downto 16)
		       );
		       
MA2:  entity work.mux32clk2 port map(
		       clk => S0_AXIS_ACLK,
		       resetn => S0_AXIS_ARESETN,
		       R0=>R0,
		       R1=>R1,
		       R2=>R2,
		       R3=>R3,
		       R4=>R4,
		       R5=>R5,
		       R6=>R6,
		       R7=>R7,
		       R8=>R8,
		       R9=>R9,
		       R10=>R10,
		       R11=>R11,
		       R12=>R12,
		       R13=>R13,
		       R14=>R14,
		       R15=>R15,
		       SOUTa=>SOUTA2a,
		       SOUTb=>SOUTA2b,
		       Conf=>action2comb(23 downto 16)
		       );
MA3:  entity work.mux32clk2 port map(
		       clk => S0_AXIS_ACLK,
		       resetn => S0_AXIS_ARESETN,
		       R0=>R0,
		       R1=>R1,
		       R2=>R2,
		       R3=>R3,
		       R4=>R4,
		       R5=>R5,
		       R6=>R6,
		       R7=>R7,
		       R8=>R8,
		       R9=>R9,
		       R10=>R10,
		       R11=>R11,
		       R12=>R12,
		       R13=>R13,
		       R14=>R14,
		       R15=>R15,
		       SOUTa=>SOUTA3a,
		       SOUTb=>SOUTA3b,
		       Conf=>action3comb(23 downto 16)
		       );
MA4:  entity work.mux32clk2 port map(
		       clk => S0_AXIS_ACLK,
		       resetn => S0_AXIS_ARESETN,
		       R0=>R0,
		       R1=>R1,
		       R2=>R2,
		       R3=>R3,
		       R4=>R4,
		       R5=>R5,
		       R6=>R6,
		       R7=>R7,
		       R8=>R8,
		       R9=>R9,
		       R10=>R10,
		       R11=>R11,
		       R12=>R12,
		       R13=>R13,
		       R14=>R14,
		       R15=>R15,
		       SOUTa=>SOUTA4a,
		       SOUTb=>SOUTA4b,
		       Conf=>action4comb(23 downto 16)
		       );
MA5:  entity work.mux32clk2 port map(
		       clk => S0_AXIS_ACLK,
		       resetn => S0_AXIS_ARESETN,
		       R0=>R0,
		       R1=>R1,
		       R2=>R2,
		       R3=>R3,
		       R4=>R4,
		       R5=>R5,
		       R6=>R6,
		       R7=>R7,
		       R8=>R8,
		       R9=>R9,
		       R10=>R10,
		       R11=>R11,
		       R12=>R12,
		       R13=>R13,
		       R14=>R14,
		       R15=>R15,
		       SOUTa=>SOUTA5a,
		       SOUTb=>SOUTA5b,
		       Conf=>action5comb(23 downto 16)
		       );
MA6:  entity work.mux32clk2 port map(
		       clk => S0_AXIS_ACLK,
		       resetn => S0_AXIS_ARESETN,
		       R0=>R0,
		       R1=>R1,
		       R2=>R2,
		       R3=>R3,
		       R4=>R4,
		       R5=>R5,
		       R6=>R6,
		       R7=>R7,
		       R8=>R8,
		       R9=>R9,
		       R10=>R10,
		       R11=>R11,
		       R12=>R12,
		       R13=>R13,
		       R14=>R14,
		       R15=>R15,
		       SOUTa=>SOUTA6a,
		       SOUTb=>SOUTA6b,
		       Conf=>action6comb(23 downto 16)
		       );
MA7:  entity work.mux32clk2 port map(
		       clk => S0_AXIS_ACLK,
		       resetn => S0_AXIS_ARESETN,
		       R0=>R0,
		       R1=>R1,
		       R2=>R2,
		       R3=>R3,
		       R4=>R4,
		       R5=>R5,
		       R6=>R6,
		       R7=>R7,
		       R8=>R8,
		       R9=>R9,
		       R10=>R10,
		       R11=>R11,
		       R12=>R12,
		       R13=>R13,
		       R14=>R14,
		       R15=>R15,
		       SOUTa=>SOUTA7a,
		       SOUTb=>SOUTA7b,
		       Conf=>action7comb(23 downto 16)
		       );
MA8:  entity work.mux32clk2 port map(
		       clk => S0_AXIS_ACLK,
		       resetn => S0_AXIS_ARESETN,
		       R0=>R0,
		       R1=>R1,
		       R2=>R2,
		       R3=>R3,
		       R4=>R4,
		       R5=>R5,
		       R6=>R6,
		       R7=>R7,
		       R8=>R8,
		       R9=>R9,
		       R10=>R10,
		       R11=>R11,
		       R12=>R12,
		       R13=>R13,
		       R14=>R14,
		       R15=>R15,
		       SOUTa=>SOUTA8a,
		       SOUTb=>SOUTA8b,
		       Conf=>action8comb(23 downto 16)
		       );




--- Azioni per send packet
A_Send_Pkts: process(S0_AXIS_ACLK, S0_AXIS_ARESETN)
	begin
	if (S0_AXIS_ACLK'event and S0_AXIS_ACLK = '1') then
		if (S0_AXIS_ARESETN = '0') then
			M_PKTGEN_AXIS_TVALID <= '0';
			M_PKTGEN_AXIS_TLAST  <= '0';
			send<='0';
			count_66<=(others=>'0');
			count_1024<=(others=>'0');
		else
			send<='0';
			M_PKTGEN_AXIS_TVALID <= '0';
			M_PKTGEN_AXIS_TUSER(127 downto 32)  <= (others =>'0');
			M_PKTGEN_AXIS_TUSER(31 downto 0) <= x"00000020";
			M_PKTGEN_AXIS_TKEEP  <= (others =>'1');
			M_PKTGEN_AXIS_TLAST  <= '1';
			if (execute='1') then
				if (action0=x"00000100" and curr_state=PKT5) then
				        count_66<=count_66+1;
						send<='1';
						M_PKTGEN_AXIS_TVALID <= '1';
						M_PKTGEN_AXIS_TDATA(31 downto 0)<= SOUTA1a;
						M_PKTGEN_AXIS_TDATA(63 downto 32)<=SOUTA1b; --seq_num
						--M_PKTGEN_AXIS_TDATA(31 downto 0)  <= x"00" & action1(23 downto 0); --seq_num;
						--M_PKTGEN_AXIS_TDATA(63 downto 32)  <=  (others =>'0');                        --timer(41 downto 10);
						
						M_PKTGEN_AXIS_TDATA(66 downto 64)  <=(others=>'0') ;
                        M_PKTGEN_AXIS_TDATA(66)  <= '1' ;                        --timer(41 downto 10);
                        M_PKTGEN_AXIS_TDATA(98 downto 67)  <= SOUTA2a;
                        M_PKTGEN_AXIS_TDATA(130 downto 99)  <=SOUTA2b;
                        M_PKTGEN_AXIS_TDATA(255 downto 131) <= (others =>'0');

				end if;
				if (action0=x"00000200" and curr_state=PKT5) then
                        count_1024<=count_1024+1;
                        send<='1';
                        M_PKTGEN_AXIS_TVALID <= '1';
                        M_PKTGEN_AXIS_TDATA(31 downto 0)<= SOUTA1a;
                        M_PKTGEN_AXIS_TDATA(63 downto 32)<= SOUTA1b;   --seq_num
                        
                        --M_PKTGEN_AXIS_TDATA(31 downto 0)  <= x"00" & action1(23 downto 0); --seq_num;
                        M_PKTGEN_AXIS_TDATA(66 downto 64)  <=(others =>'0');
			            M_PKTGEN_AXIS_TDATA(98 downto 67)  <= SOUTA2a;
			            M_PKTGEN_AXIS_TDATA(130 downto 99)  <= SOUTA2b;
			            M_PKTGEN_AXIS_TDATA(255 downto 131) <= (others =>'0');


                end if;
			end if;
		end if;
	end if;
end process;

--- Azioni per set timer
A_Set_TIMER: process(S0_AXIS_ACLK, S0_AXIS_ARESETN)
	begin
		if (S0_AXIS_ACLK'event and S0_AXIS_ACLK = '1') then
			if (S0_AXIS_ARESETN = '0') then
				M_TIMER_AXIS_TVALID<='0';
				M_TIMER_AXIS_TLAST<='1';
				timer_c<='0';
			else
				M_TIMER_AXIS_TVALID<='0';
				M_TIMER_AXIS_TLAST<='1';
				M_TIMER_AXIS_TKEEP(31 downto 0)<=x"ffffffff";
				M_TIMER_AXIS_TUSER(31 downto 0)<=(OTHERS=>'0');
				M_TIMER_AXIS_TUSER(33 downto 32)<="01";
				M_TIMER_AXIS_TUSER(127 downto 34)<=(OTHERS=>'0');
				timer_c<='0';

				if (curr_state=START_PKT1) then
					timer_c<='1';
					M_TIMER_AXIS_TVALID<='1';
					M_TIMER_AXIS_TDATA(31 downto 0)   <= x"0000_0002"; --T1
					M_TIMER_AXIS_TDATA(63 downto 32)<=(OTHERS=>'0');
					M_TIMER_AXIS_TDATA(95 downto 64)  <= x"0000_0001"; --V1
					M_TIMER_AXIS_TDATA(103 downto 96) <= x"00"; --V1
					M_TIMER_AXIS_TDATA(255 downto 104)<=(OTHERS=>'0');
				end if;
				if (execute='1') then
					if (action0(31 downto 24)=x"AA"  ) then
--set timer
						timer_c<='1';
						M_TIMER_AXIS_TVALID<='1';
						M_TIMER_AXIS_TDATA(31 downto 0)   <= x"0000_0002"; --T1
						M_TIMER_AXIS_TDATA(63 downto 32)<=(OTHERS=>'0');
						M_TIMER_AXIS_TDATA(95 downto 64)  <= x"0000_0001"; --V1
						M_TIMER_AXIS_TDATA(103 downto 96) <= x"00"; --V1
						M_TIMER_AXIS_TDATA(255 downto 104)<=(OTHERS=>'0');
					end if;
					if (action0(31 downto 24)=x"AB") then
--set timer
						M_TIMER_AXIS_TVALID<='1';
						M_TIMER_AXIS_TDATA(31 downto 0)   <= SOUTA3a; --T1
						M_TIMER_AXIS_TDATA(63 downto 32)<=(OTHERS=>'0');
                                                
						M_TIMER_AXIS_TDATA(95 downto 64)  <= SOUTA3b; --V1
						M_TIMER_AXIS_TDATA(103 downto 96) <= x"00"; --V1
						M_TIMER_AXIS_TDATA(255 downto 104)<=(OTHERS=>'0');
					end if;
					if (action0(31 downto 24)=x"AC") then
--set timer
                        timer_c<='1';
                        M_TIMER_AXIS_TVALID<='1';
                        M_TIMER_AXIS_TDATA(31 downto 0) <= x"00" & action0(23 downto 0); --T1
                        M_TIMER_AXIS_TDATA(63 downto 32)<=(OTHERS=>'0');
                        M_TIMER_AXIS_TDATA(95 downto 64)  <= x"0000_0001"; --V1
                        M_TIMER_AXIS_TDATA(103 downto 96) <= x"00"; --V1
                        M_TIMER_AXIS_TDATA(255 downto 104)<=(OTHERS=>'0');					
					end if;
				end if;
			end if;
		end if;
end process;


--- Azioni per R0 & R8. Tramite action1(9 downto 0) seleziono il registro su cui operare
AR0eR8: process(S0_AXIS_ACLK, S0_AXIS_ARESETN)
		variable result: std_logic_vector(41 downto 0);
		variable result_1: std_logic_vector(41 downto 0);
		variable vR: std_logic_vector(31 downto 0);
		variable hout :std_logic_vector(31 downto 0);
		variable reg : std_logic_vector (31 downto 0);
		begin
			if (S0_AXIS_ACLK'event and S0_AXIS_ACLK = '1') then
				if (S0_AXIS_ARESETN = '0') then
					R0<=x"00000000";
					R8<=x"00000000";
                    --vR:=x"00000000";
				else
					vR:=x"00000000";
-- AZIONI SU R0
					if (curr_state=START ) then
						R0<=R0_in;
					end if;
					if (curr_state=START ) then
						R8<=R8_in;
					end if;
                    if (curr_state=reset_all) then
                        R0<=x"00000000";
                        R8<=x"00000000";
                    end if;
					if (execute='1') then --if ((execute='1') and (action1 /= NOP)) then  
						if ( action1(31 downto 24) = x"11") then
							vR:=R0_in;
						end if;
                        if ( action1(31 downto 24) = x"12") then
                            vR:=R8_in;
                        end if;
                        
                        if ( action1(31 downto 24) = x"1A") then 
                            vR:= x"000" & action1(23 downto 4);
                        end if;
                        
						if (action1(31 downto 24) = x"91" ) then
							vR:=SOUTA1a+1;
						end if;

						if (action1(31 downto 24) = x"A1") then
							vR:=SOUTA1a+SOUTA1b;
						end if;
						if (action1(31 downto 24) = x"B1" ) then
							if (SOUTA1a>SOUTA1b) then
							vR:=SOUTA1a-SOUTA1b;
							else
							vR:=(others=>'0');
							end if;
						end if;
                                                    
						if (action1(31 downto 24) = x"C1") then
							vR:=SOUTA1a(15 downto 0)*SOUTA1b(15 downto 0);
						end if;

						if (action1(31 downto 24) = x"D1" ) then
							result:=SOUTA1a*inv_tab(conv_integer(SOUTA1b(7 downto 0))); --SOUT1*(1024/SOUT2)
							vR:=result(41 downto 10); --SOUT1*(1024/SOUT2)*1/1024
						end if;
                       
						if (action1(31 downto 24) = x"E0") then
							vR:=RHOUT1; --SOUT1/SOUT2
						end if;
						if (action1(31 downto 24) = x"F0") then
							vR:=RHOUT1+1; --SOUT1/SOUT2
						end if;
						if (action1(31 downto 24) = x"E1") then
						        vR:=RHOUT2; --SOUT1/SOUT2
						end if;
						if (action1(31 downto 24) = x"E2") then
						        vR:=RHOUT3; --SOUT1/SOUT2
						end if;
						if (action1(31 downto 24) = x"E3") then
							vR:=RHOUT4; --SOUT1/SOUT2
						end if;
						if (action1(31 downto 24) = x"F1") then
							vR:=SOUTA1a+action1(19 downto 4); --SOUT1/SOUT2
						end if;
                        
                         if (action1(10 downto 4)= x"01") then
                              hout:=RHOUT1;
                         end if;
                         
                          
                         if (action1(10 downto 4)= x"02") then
                             hout:=RHOUT2;
                         end if;
                          
                              
                         if (action1(10 downto 4)= x"03") then
                             hout:=RHOUT3;
                         end if;
                          
                              
                         if (action1(10 downto 4)= x"04") then
                             hout:=RHOUT4;
                         end if;
                             
                         if (action1(14 downto 11)= x"0") then
                             reg:=SOUTA1a;
                         end if;
                         if (action1(14 downto 11)= x"1") then
                             reg:=SOUTA1b;
                         end if;
                         
                         if (action1(31 downto 24) = x"F2") then
                               vR:=reg+hout; --SOUT1/SOUT2
                         end if;
                         
                         if (action1(31 downto 24) = x"F3") then
                               vR:=hout+reg; --SOUT1/SOUT2
                         end if;
                         
                         if (action1(31 downto 24) = x"F4") then
                               vR:=reg-hout; --SOUT1/SOUT2
                         end if;
                         
                         if (action1(31 downto 24) = x"F5") then
                               vR:=hout-reg; --SOUT1/SOUT2
                         end if;
                         

						if (action1(3 downto 0) = x"0" ) then
							R0<= vR;
						end if;
						if (action1(3 downto 0) = x"1") then
							R8<= vR;
						end if;

					end if; --execute
				end if; --rst
		end if; --clk
end process;

--- Azioni per R0 & R8. Tramite action1(9 downto 0) seleziono il registro su cui operare
AR1eR9: process(S0_AXIS_ACLK, S0_AXIS_ARESETN)
		variable result: std_logic_vector(41 downto 0);
		variable result_1: std_logic_vector(41 downto 0);
		variable vR: std_logic_vector(31 downto 0);
		variable reg: std_logic_vector(31 downto 0);
		variable hout: std_logic_vector(31 downto 0);
		begin
			if (S0_AXIS_ACLK'event and S0_AXIS_ACLK = '1') then
				if (S0_AXIS_ARESETN = '0') then
					R1<=x"00000000";
					R9<=x"00000000";

				else
					vR:=x"00000000";
					reg:=x"00000000";
					hout:=x"00000000";
-- AZIONI SU R0
					if (curr_state=START ) then
						R1<=R1_in;
					end if;
					if (curr_state=START ) then
						R9<=R9_in;
					end if;
                    if (curr_state=reset_all) then
                        R1<=x"00000000";
                        R9<=x"00000000";
                    end if;
					if (execute='1') then
						if ( action2(31 downto 24) = x"11") then
							vR:=R1_in;
						end if;
                
						if ( action2(31 downto 24) = x"1A") then 
						    vR:= x"000" & action2(23 downto 4);
						end if;
                
						if (action2(31 downto 24) = x"91" ) then
							vR:=SOUTA2a+1;
						end if;

						if (action2(31 downto 24) = x"A1") then
							vR:=SOUTA2a+SOUTA2b;
						end if;
						if (action2(31 downto 24) = x"B1" ) then
							if (SOUTA2a>SOUTA2b) then
                            vR:=SOUTA2a-SOUTA2b;
                            else
                            vR:=(others=>'0');
                            end if;
						end if;
                                                    
						if (action2(31 downto 24) = x"C1") then
							vR:=SOUTA2a(15 downto 0)*SOUTA2b(15 downto 0);
						end if;

						if (action2(31 downto 24) = x"D1" ) then
							result:=SOUTA2a*inv_tab(conv_integer(SOUTA2b(7 downto 0))); --SOUT1*(1024/SOUT2)
							vR:=result(41 downto 10); --SOUT1*(1024/SOUT2)*1/1024
						end if;
                       
						if (action2(31 downto 24) = x"E0") then
							vR:=RHOUT1; --SOUT1/SOUT2
						end if;
						if (action2(31 downto 24) = x"F0") then
							vR:=RHOUT1+1; --SOUT1/SOUT2
						end if;
						if (action2(31 downto 24) = x"E1") then
						        vR:=RHOUT2; --SOUT1/SOUT2
						end if;
						if (action2(31 downto 24) = x"E2") then
						        vR:=RHOUT3; --SOUT1/SOUT2
						end if;
						if (action2(31 downto 24) = x"E3") then
							vR:=RHOUT4; --SOUT1/SOUT2
						end if;
						if (action2(31 downto 24) = x"F1") then
							vR:=SOUTA2a+action2(19 downto 4); --SOUT1/SOUT2
						end if;
						
						if (action2(10 downto 4)= x"01") then
						     hout:=RHOUT1;
						end if;
						
						 
						if (action2(10 downto 4)= x"02") then
						    hout:=RHOUT2;
						end if;
						 
						     
						if (action2(10 downto 4)= x"03") then
						    hout:=RHOUT3;
						end if;
						 
						     
						if (action2(10 downto 4)= x"04") then
						    hout:=RHOUT4;
						end if;
						    
						if (action2(14 downto 11)= x"0") then
						    reg:=SOUTA2a;
						end if;
						if (action2(14 downto 11)= x"1") then
						    reg:=SOUTA2b;
						end if;
						
						if (action2(31 downto 24) = x"F2") then
						      vR:=reg+hout; --SOUT1/SOUT2
						end if;
						
						if (action2(31 downto 24) = x"F3") then
						      vR:=hout+reg; --SOUT1/SOUT2
						end if;
						
						if (action2(31 downto 24) = x"F4") then
						      vR:=reg-hout; --SOUT1/SOUT2
						end if;
						
						if (action2(31 downto 24) = x"F5") then
						      vR:=hout-reg; --SOUT1/SOUT2
						end if;

						if (action2(3 downto 0) = x"00" ) then
							R1<= vR;
						end if;
						if (action2(3 downto 0) = x"01") then
							R9<= vR;
						end if;

					end if; --execute
				end if; --rst
		end if; --clk
end process;


--- Azioni per R0 & R8. Tramite action1(9 downto 0) seleziono il registro su cui operare
AR2eR10: process(S0_AXIS_ACLK, S0_AXIS_ARESETN)
		variable result: std_logic_vector(41 downto 0);
		variable result_1: std_logic_vector(41 downto 0);
		variable vR: std_logic_vector(31 downto 0);
		variable reg: std_logic_vector(31 downto 0);
		variable hout: std_logic_vector(31 downto 0);
		begin
			if (S0_AXIS_ACLK'event and S0_AXIS_ACLK = '1') then
				if (S0_AXIS_ARESETN = '0') then
					R2<=x"00000000";
					R10<=x"00000000";

				else
					vR:=x"00000000";
					reg:=x"00000000";
					hout:=x"00000000";
-- AZIONI SU R0
					if (curr_state=START ) then
						R2<=R2_in;
					end if;
					if (curr_state=START ) then
						R10<=R10_in;
					end if;
                    if (curr_state=reset_all) then
                        R2<=x"00000000";
                        R10<=x"00000000";
                    end if;
					if (execute='1') then
						if ( action3(31 downto 24) = x"11") then
							vR:=R2_in;
						end if;
                
						if ( action3(31 downto 24) = x"1A") then 
						    vR:= x"000" & action3(23 downto 4);
						end if;
                

						if (action3(31 downto 24) = x"91" ) then
							vR:=SOUTA3a+1;
						end if;

						if (action3(31 downto 24) = x"A1") then
							vR:=SOUTA3a+SOUTA3b;
						end if;
						if (action3(31 downto 24) = x"B1" ) then
							if (SOUTA3a>SOUTA3b) then
                               vR:=SOUTA3a-SOUTA3b;
                             else
                               vR:=(others=>'0');
                             end if;
						end if;
                                                    
						if (action3(31 downto 24) = x"C1") then
							vR:=SOUTA3a(15 downto 0)*SOUTA3b(15 downto 0);
						end if;

						if (action3(31 downto 24) = x"D1" ) then
							result:=SOUTA3a*inv_tab(conv_integer(SOUTA3b(7 downto 0))); --SOUT1*(1024/SOUT2)
							vR:=result(41 downto 10); --SOUT1*(1024/SOUT2)*1/1024
						end if;
                       
						if (action3(31 downto 24) = x"E0") then
							vR:=RHOUT1; --SOUT1/SOUT2
						end if;
						if (action3(31 downto 24) = x"F0") then
							vR:=RHOUT1+1; --SOUT1/SOUT2
						end if;
						if (action3(31 downto 24) = x"E1") then
						        vR:=RHOUT2; --SOUT1/SOUT2
						end if;
						if (action3(31 downto 24) = x"E2") then
						        vR:=RHOUT3; --SOUT1/SOUT2
						end if;
						if (action3(31 downto 24) = x"E3") then
							vR:=RHOUT4; --SOUT1/SOUT2
						end if;
						if (action3(31 downto 24) = x"F1") then
							vR:=SOUTA3a+action3(19 downto 4); --SOUT1/SOUT2
						end if;
						if (action3(10 downto 4)= x"01") then
						     hout:=RHOUT1;
						end if;
						
						 
						if (action3(10 downto 4)= x"02") then
						    hout:=RHOUT2;
						end if;
						 
						     
						if (action3(10 downto 4)= x"03") then
						    hout:=RHOUT3;
						end if;
						 
						     
						if (action3(10 downto 4)= x"04") then
						    hout:=RHOUT4;
						end if;
						    
						if (action3(14 downto 11)= x"0") then
						    reg:=SOUTA3a;
						end if;
						if (action3(14 downto 11)= x"1") then
						    reg:=SOUTA3b;
						end if;
						
						if (action3(31 downto 24) = x"F2") then
						      vR:=reg+hout; --SOUT1/SOUT2
						end if;
						
						if (action3(31 downto 24) = x"F3") then
						      vR:=hout+reg; --SOUT1/SOUT2
						end if;
						
						if (action3(31 downto 24) = x"F4") then
						      vR:=reg-hout; --SOUT1/SOUT2
						end if;
						
						if (action3(31 downto 24) = x"F5") then
						      vR:=hout-reg; --SOUT1/SOUT2
						end if;

						if (action3(3 downto 0) = x"00" ) then
							R2<= vR;
						end if;
						if (action3(3 downto 0) = x"01") then
							R10<= vR;
						end if;

					end if; --execute
				end if; --rst
		end if; --clk
end process;
--- Azioni per R0 & R8. Tramite action1(9 downto 0) seleziono il registro su cui operare
AR3eR11: process(S0_AXIS_ACLK, S0_AXIS_ARESETN)
		variable result: std_logic_vector(41 downto 0);
		variable result_1: std_logic_vector(41 downto 0);
		variable vR: std_logic_vector(31 downto 0);
		variable reg: std_logic_vector(31 downto 0);
		variable hout: std_logic_vector(31 downto 0);
		begin
			if (S0_AXIS_ACLK'event and S0_AXIS_ACLK = '1') then
				if (S0_AXIS_ARESETN = '0') then
					R3<=x"00000000";
					R11<=x"00000000";

				else
					vR:=x"00000000";
					reg:=x"00000000";
					hout:=x"00000000";
-- AZIONI SU R0
					if (curr_state=START ) then
						R3<=R3_in;
					end if;
					if (curr_state=START ) then
						R11<=R11_in;
					end if;
                    if (curr_state=reset_all) then
                        R3<=x"00000000";
                        R11<=x"00000000";
                    end if;
					if (execute='1') then
						if ( action4(31 downto 24) = x"11") then
							vR:=R3_in;
						end if;
                
						if ( action4(31 downto 24) = x"1A") then 
						    vR:= x"000" & action4(23 downto 4);
						end if;
                

						if (action4(31 downto 24) = x"91" ) then
							vR:=SOUTA4a+1;
						end if;

						if (action4(31 downto 24) = x"A1") then
							vR:=SOUTA4a+SOUTA4b;
						end if;
						if (action4(31 downto 24) = x"B1" ) then
							if (SOUTA4a>SOUTA4b) then
                               vR:=SOUTA4a-SOUTA4b;
                            else
                               vR:=(others=>'0');
                            end if;
						end if;
                                                    
						if (action4(31 downto 24) = x"C1") then
							vR:=SOUTA4a(15 downto 0)*SOUTA4b(15 downto 0);
						end if;

						if (action4(31 downto 24) = x"D1" ) then
							result:=SOUTA4a*inv_tab(conv_integer(SOUTA4b(7 downto 0))); --SOUT1*(1024/SOUT2)
							vR:=result(41 downto 10); --SOUT1*(1024/SOUT2)*1/1024
						end if;
                       
						if (action4(31 downto 24) = x"E0") then
							vR:=RHOUT1; --SOUT1/SOUT2
						end if;
						if (action4(31 downto 24) = x"F0") then
							vR:=RHOUT1+1; --SOUT1/SOUT2
						end if;
						if (action4(31 downto 24) = x"E1") then
						        vR:=RHOUT2; --SOUT1/SOUT2
						end if;
						if (action4(31 downto 24) = x"E2") then
						        vR:=RHOUT3; --SOUT1/SOUT2
						end if;
						if (action4(31 downto 24) = x"E3") then
							vR:=RHOUT4; --SOUT1/SOUT2
						end if;
						if (action4(31 downto 24) = x"F1") then
							vR:=SOUTA4a+action4(19 downto 4); --SOUT1/SOUT2
						end if;
						if (action4(10 downto 4)= x"01") then
						     hout:=RHOUT1;
						end if;
						
						 
						if (action4(10 downto 4)= x"02") then
						    hout:=RHOUT2;
						end if;
						 
						     
						if (action4(10 downto 4)= x"03") then
						    hout:=RHOUT3;
						end if;
						 
						     
						if (action4(10 downto 4)= x"04") then
						    hout:=RHOUT4;
						end if;
						    
						if (action4(14 downto 11)= x"0") then
						    reg:=SOUTA4a;
						end if;
						if (action4(14 downto 11)= x"1") then
						    reg:=SOUTA4b;
						end if;
						
						if (action4(31 downto 24) = x"F2") then
						      vR:=reg+hout; --SOUT1/SOUT2
						end if;
						
						if (action4(31 downto 24) = x"F3") then
						      vR:=hout+reg; --SOUT1/SOUT2
						end if;
						
						if (action4(31 downto 24) = x"F4") then
						      vR:=reg-hout; --SOUT1/SOUT2
						end if;
						
						if (action4(31 downto 24) = x"F5") then
						      vR:=hout-reg; --SOUT1/SOUT2
						end if;
						if (action4(3 downto 0) = x"00" ) then
							R3<= vR;
						end if;
						if (action4(3 downto 0) = x"01") then
							R11<= vR;
						end if;

					end if; --execute
				end if; --rst
		end if; --clk
end process;
--- Azioni per R0 & R8. Tramite action1(9 downto 0) seleziono il registro su cui operare
AR4eR12: process(S0_AXIS_ACLK, S0_AXIS_ARESETN)
		variable result: std_logic_vector(41 downto 0);
		variable result_1: std_logic_vector(41 downto 0);
		variable vR: std_logic_vector(31 downto 0);
		variable reg: std_logic_vector(31 downto 0);
		variable hout: std_logic_vector(31 downto 0);
		begin
			if (S0_AXIS_ACLK'event and S0_AXIS_ACLK = '1') then
				if (S0_AXIS_ARESETN = '0') then
					R4<=x"00000000";
					R12<=x"00000000";

				else
					vR:=x"00000000";
					reg:=x"00000000";
					hout:=x"00000000";
-- AZIONI SU R0
					if (curr_state=START ) then
						R4<=R4_in;
					end if;
					if (curr_state=START ) then
						R12<=R12_in;
					end if;
                    if (curr_state=reset_all) then
                        R4<=x"00000000";
                        R12<=x"00000000";
                    end if;
					if (execute='1') then
						if ( action5(31 downto 24) = x"11") then
							vR:=R4_in;
						end if;
                
						if ( action5(31 downto 24) = x"1A") then 
						    vR:= x"000" & action5(23 downto 4);
						end if;
                

						if (action5(31 downto 24) = x"91" ) then
							vR:=SOUTA5a+1;
						end if;

						if (action5(31 downto 24) = x"A1") then
							vR:=SOUTA5a+SOUTA5b;
						end if;
						if (action5(31 downto 24) = x"B1" ) then
							if (SOUTA5a>SOUTA5b) then
                                vR:=SOUTA5a-SOUTA5b;
                            else
                                vR:=(others=>'0');
                            end if;
						end if;
                                                    
						if (action5(31 downto 24) = x"C1") then
							vR:=SOUTA5a(15 downto 0)*SOUTA5b(15 downto 0);
						end if;

						if (action5(31 downto 24) = x"D1" ) then
							result:=SOUTA5a*inv_tab(conv_integer(SOUTA5b(7 downto 0))); --SOUT1*(1024/SOUT2)
							vR:=result(41 downto 10); --SOUT1*(1024/SOUT2)*1/1024
						end if;
                       
						if (action5(31 downto 24) = x"E0") then
							vR:=RHOUT1; --SOUT1/SOUT2
						end if;
						if (action5(31 downto 24) = x"F0") then
							vR:=RHOUT1+1; --SOUT1/SOUT2
						end if;
						if (action5(31 downto 24) = x"E1") then
						        vR:=RHOUT2; --SOUT1/SOUT2
						end if;
						if (action5(31 downto 24) = x"E2") then
						        vR:=RHOUT3; --SOUT1/SOUT2
						end if;
						if (action5(31 downto 24) = x"E3") then
							vR:=RHOUT4; --SOUT1/SOUT2
						end if;
						if (action5(31 downto 24) = x"F1") then
							vR:=SOUTA5a+action5(19 downto 4); --SOUT1/SOUT2
						end if;
						if (action5(10 downto 4)= x"02") then
						    hout:=RHOUT2;
						end if;
						 
						     
						if (action5(10 downto 4)= x"03") then
						    hout:=RHOUT3;
						end if;
						 
						     
						if (action5(10 downto 4)= x"04") then
						    hout:=RHOUT4;
						end if;
						    
						if (action5(14 downto 11)= x"0") then
						    reg:=SOUTA5a;
						end if;
						if (action5(14 downto 11)= x"1") then
						    reg:=SOUTA5b;
						end if;
						
						if (action5(31 downto 24) = x"F2") then
						      vR:=reg+hout; --SOUT1/SOUT2
						end if;
						
						if (action5(31 downto 24) = x"F3") then
						      vR:=hout+reg; --SOUT1/SOUT2
						end if;
						
						if (action5(31 downto 24) = x"F4") then
						      vR:=reg-hout; --SOUT1/SOUT2
						end if;
						
						if (action5(31 downto 24) = x"F5") then
						      vR:=hout-reg; --SOUT1/SOUT2
						end if;
						if (action5(3 downto 0) = x"00" ) then
							R4<= vR;
						end if;
						if (action5(3 downto 0) = x"01") then
							R12<= vR;
						end if;

					end if; --execute
				end if; --rst
		end if; --clk
end process;
--- Azioni per R0 & R8. Tramite action1(9 downto 0) seleziono il registro su cui operare
AR5eR13: process(S0_AXIS_ACLK, S0_AXIS_ARESETN)
		variable result: std_logic_vector(41 downto 0);
		variable result_1: std_logic_vector(41 downto 0);
		variable vR: std_logic_vector(31 downto 0);
		variable reg: std_logic_vector(31 downto 0);
		variable hout: std_logic_vector(31 downto 0);
		begin
			if (S0_AXIS_ACLK'event and S0_AXIS_ACLK = '1') then
				if (S0_AXIS_ARESETN = '0') then
					R5<=x"00000000";
					R13<=x"00000000";

				else
					vR:=x"00000000";
					reg:=x"00000000";
					hout:=x"00000000";
-- AZIONI SU R0
					if (curr_state=START ) then
						R5<=R5_in;
					end if;
					if (curr_state=START ) then
						R13<=R13_in;
					end if;
					if (curr_state=reset_all) then
                        R5<=x"00000000";
                        R13<=x"00000000";
                    end if;
                    
					if (execute='1') then
						if ( action6(31 downto 24) = x"11") then
							vR:=R5_in;
						end if;
                
						if ( action6(31 downto 24) = x"1A") then 
						    vR:= x"000" & action6(23 downto 4);
						end if;
                

						if (action6(31 downto 24) = x"91" ) then
							vR:=SOUTA6a+1;
						end if;

						if (action6(31 downto 24) = x"A1") then
							vR:=SOUTA6a+SOUTA6b;
						end if;
						if (action6(31 downto 24) = x"B1" ) then
							if (SOUTA6a>SOUTA6b) then
                               vR:=SOUTA6a-SOUTA6b;
                            else
                               vR:=(others=>'0');
                            end if;
						end if;
                                                    
						if (action6(31 downto 24) = x"C1") then
							vR:=SOUTA6a(15 downto 0)*SOUTA6b(15 downto 0);
						end if;

						if (action6(31 downto 24) = x"D1" ) then
							result:=SOUTA6a*inv_tab(conv_integer(SOUTA6b(7 downto 0))); --SOUT1*(1024/SOUT2)
							vR:=result(41 downto 10); --SOUT1*(1024/SOUT2)*1/1024
						end if;
                       
						if (action6(31 downto 24) = x"E0") then
							vR:=RHOUT1; --SOUT1/SOUT2
						end if;
						if (action6(31 downto 24) = x"F0") then
							vR:=RHOUT1+1; --SOUT1/SOUT2
						end if;
						if (action6(31 downto 24) = x"E1") then
						        vR:=RHOUT2; --SOUT1/SOUT2
						end if;
						if (action6(31 downto 24) = x"E2") then
						        vR:=RHOUT3; --SOUT1/SOUT2
						end if;
						if (action6(31 downto 24) = x"E3") then
							vR:=RHOUT4; --SOUT1/SOUT2
						end if;
						if (action6(31 downto 24) = x"F1") then
							vR:=SOUTA6a+action6(19 downto 4); --SOUT1/SOUT2
						end if;
						if (action6(10 downto 4)= x"02") then
						    hout:=RHOUT2;
						end if;
						 
						     
						if (action6(10 downto 4)= x"03") then
						    hout:=RHOUT3;
						end if;
						 
						     
						if (action6(10 downto 4)= x"04") then
						    hout:=RHOUT4;
						end if;
						    
						if (action6(14 downto 11)= x"0") then
						    reg:=SOUTA6a;
						end if;
						if (action6(14 downto 11)= x"1") then
						    reg:=SOUTA6b;
						end if;
						
						if (action6(31 downto 24) = x"F2") then
						      vR:=reg+hout; --SOUT1/SOUT2
						end if;
						
						if (action6(31 downto 24) = x"F3") then
						      vR:=hout+reg; --SOUT1/SOUT2
						end if;
						
						if (action6(31 downto 24) = x"F4") then
						      vR:=reg-hout; --SOUT1/SOUT2
						end if;
						
						if (action6(31 downto 24) = x"F5") then
						      vR:=hout-reg; --SOUT1/SOUT2
						end if;
						if (action6(3 downto 0) = x"00" ) then
							R5<= vR;
						end if;
						if (action6(3 downto 0) = x"01") then
							R13<= vR;
						end if;

					end if; --execute
				end if; --rst
		end if; --clk
end process;
--- Azioni per R0 & R8. Tramite action1(9 downto 0) seleziono il registro su cui operare
AR6eR14: process(S0_AXIS_ACLK, S0_AXIS_ARESETN)
		variable result: std_logic_vector(41 downto 0);
		variable result_1: std_logic_vector(41 downto 0);
		variable vR: std_logic_vector(31 downto 0);
		variable reg: std_logic_vector(31 downto 0);
		variable hout: std_logic_vector(31 downto 0);
		begin
			if (S0_AXIS_ACLK'event and S0_AXIS_ACLK = '1') then
				if (S0_AXIS_ARESETN = '0') then
					R6<=x"00000000";
					R14<=x"00000000";

				else
					vR:=x"00000000";
					reg:=x"00000000";
					hout:=x"00000000";
-- AZIONI SU R0
					if (curr_state=START ) then
						R6<=R6_in;
					end if;
					if (curr_state=START ) then
						R14<=R14_in;
					end if;
					if (curr_state=reset_all) then
                        R6<=x"00000000";
                        R14<=x"00000000";
                    end if;

					if (execute='1') then
						if ( action7(31 downto 24) = x"11") then
							vR:=R6_in;
						end if;
                
						if ( action7(31 downto 24) = x"1A") then 
						    vR:= x"000" & action7(23 downto 4);
						end if;
                

						if (action7(31 downto 24) = x"91" ) then
							vR:=SOUTA7a+1;
						end if;

						if (action7(31 downto 24) = x"A1") then
							vR:=SOUTA7a+SOUTA7b;
						end if;
						if (action7(31 downto 24) = x"B1" ) then
							if (SOUTA7a>SOUTA7b) then
                                vR:=SOUTA7a-SOUTA7b;
                            else
                                vR:=(others=>'0');
                            end if;
						end if;
                                                    
						if (action7(31 downto 24) = x"C1") then
							vR:=SOUTA7a(15 downto 0)*SOUTA7b(15 downto 0);
						end if;

						if (action7(31 downto 24) = x"D1" ) then
							result:=SOUTA7a*inv_tab(conv_integer(SOUTA7b(7 downto 0))); --SOUT1*(1024/SOUT2)
							vR:=result(41 downto 10); --SOUT1*(1024/SOUT2)*1/1024
						end if;
                       
						if (action7(31 downto 24) = x"E0") then
							vR:=RHOUT1; --SOUT1/SOUT2
						end if;
						if (action7(31 downto 24) = x"F0") then
							vR:=RHOUT1+1; --SOUT1/SOUT2
						end if;
						if (action7(31 downto 24) = x"E1") then
						        vR:=RHOUT2; --SOUT1/SOUT2
						end if;
						if (action7(31 downto 24) = x"E2") then
						        vR:=RHOUT3; --SOUT1/SOUT2
						end if;
						if (action7(31 downto 24) = x"E3") then
							vR:=RHOUT4; --SOUT1/SOUT2
						end if;
						if (action7(31 downto 24) = x"F1") then
							vR:=SOUTA7a+action7(19 downto 4); --SOUT1/SOUT2
						end if;
						if (action7(10 downto 4)= x"02") then
						    hout:=RHOUT2;
						end if;
						 
						     
						if (action7(10 downto 4)= x"03") then
						    hout:=RHOUT3;
						end if;
						 
						     
						if (action7(10 downto 4)= x"04") then
						    hout:=RHOUT4;
						end if;
						    
						if (action7(14 downto 11)= x"0") then
						    reg:=SOUTA7a;
						end if;
						if (action7(14 downto 11)= x"1") then
						    reg:=SOUTA7b;
						end if;
						
						if (action7(31 downto 24) = x"F2") then
						      vR:=reg+hout; --SOUT1/SOUT2
						end if;
						
						if (action7(31 downto 24) = x"F3") then
						      vR:=hout+reg; --SOUT1/SOUT2
						end if;
						
						if (action7(31 downto 24) = x"F4") then
						      vR:=reg-hout; --SOUT1/SOUT2
						end if;
						
						if (action7(31 downto 24) = x"F5") then
						      vR:=hout-reg; --SOUT1/SOUT2
						end if;
						if (action7(3 downto 0) = x"00" ) then
							R6<= vR;
						end if;
						if (action7(3 downto 0) = x"01") then
							R14<= vR;
						end if;

					end if; --execute
				end if; --rst
		end if; --clk
end process;
--- Azioni per R0 & R8. Tramite action1(9 downto 0) seleziono il registro su cui operare
AR7eR15: process(S0_AXIS_ACLK, S0_AXIS_ARESETN)
		variable result: std_logic_vector(41 downto 0);
		variable result_1: std_logic_vector(41 downto 0);
		variable vR: std_logic_vector(31 downto 0);
		variable reg: std_logic_vector(31 downto 0);
		variable hout: std_logic_vector(31 downto 0);
		begin
			if (S0_AXIS_ACLK'event and S0_AXIS_ACLK = '1') then
				if (S0_AXIS_ARESETN = '0') then
					R7<=x"00000000";
					R15<=x"00000000";
					count_timestamp<=x"00000000";
					R15_timestamp<=x"00000000";

				else
					vR:=x"00000000";
					reg:=x"00000000";
					hout:=x"00000000";
-- AZIONI SU R0
					if (curr_state=START ) then
						R7<=R7_in;
					end if;
					if (curr_state=START ) then
						R15<=R15_in;
					end if;
					if (curr_state=reset_all) then
                        R7<=x"00000000";
                        R15<=x"00000000";
                        count_timestamp<=x"00000000";
                        R15_timestamp<=x"00000000";
                    end if;
                    count_timestamp<=count_timestamp+1;
                    if (count_timestamp=x"0000009c") then
                        R15_timestamp<=R15_timestamp+1;
                        count_timestamp<=x"00000000";
                    end if;
					if (execute='1') then
						if ( action8(31 downto 24) = x"11") then
							vR:=R7_in;
						end if;
                
						if ( action8(31 downto 24) = x"1A") then 
						    vR:= x"000" & action8(23 downto 4);
						end if;
                

						if (action8(31 downto 24) = x"91" ) then
							vR:=SOUTA8a+1;
						end if;

						if (action8(31 downto 24) = x"A1") then
							vR:=SOUTA8a+SOUTA8b;
						end if;
						if (action8(31 downto 24) = x"B1" ) then
							if (SOUTA8a>SOUTA8b) then
                                vR:=SOUTA8a-SOUTA8b;
                            else
                                vR:=(others=>'0');
                            end if;
						end if;
                                                    
						if (action8(31 downto 24) = x"C1") then
							vR:=SOUTA8a(15 downto 0)*SOUTA8b(15 downto 0);
						end if;

						if (action8(31 downto 24) = x"D1" ) then
							result:=SOUTA8a*inv_tab(conv_integer(SOUTA8b(7 downto 0))); --SOUT1*(1024/SOUT2)
							vR:=result(41 downto 10); --SOUT1*(1024/SOUT2)*1/1024
						end if;
                       
						if (action8(31 downto 24) = x"E0") then
							vR:=RHOUT1; --SOUT1/SOUT2
						end if;
						if (action8(31 downto 24) = x"F0") then
							vR:=RHOUT1+1; --SOUT1/SOUT2
						end if;
						if (action8(31 downto 24) = x"E1") then
						        vR:=RHOUT2; --SOUT1/SOUT2
						end if;
						if (action8(31 downto 24) = x"E2") then
						        vR:=RHOUT3; --SOUT1/SOUT2
						end if;
						if (action8(31 downto 24) = x"E3") then
							vR:=RHOUT4; --SOUT1/SOUT2
						end if;
						if (action8(31 downto 24) = x"F1") then
							vR:=SOUTA8a+action8(19 downto 4); --SOUT1/SOUT2
						end if;
						if (action8(10 downto 4)= x"02") then
						    hout:=RHOUT2;
						end if;
						 
						     
						if (action8(10 downto 4)= x"03") then
						    hout:=RHOUT3;
						end if;
						 
						     
						if (action8(10 downto 4)= x"04") then
						    hout:=RHOUT4;
						end if;
						    
						if (action8(14 downto 11)= x"0") then
						    reg:=SOUTA8a;
						end if;
						if (action8(14 downto 11)= x"1") then
						    reg:=SOUTA8b;
						end if;
						
						if (action8(31 downto 24) = x"F2") then
						      vR:=reg+hout; --SOUT1/SOUT2
						end if;
						
						if (action8(31 downto 24) = x"F3") then
						      vR:=hout+reg; --SOUT1/SOUT2
						end if;
						
						if (action8(31 downto 24) = x"F4") then
						      vR:=reg-hout; --SOUT1/SOUT2
						end if;
						
						if (action8(31 downto 24) = x"F5") then
						      vR:=hout-reg; --SOUT1/SOUT2
						end if;
						if (action8(3 downto 0) = x"00" ) then
							R7<= vR;
						end if;
--						if (action8(3 downto 0) = x"01") then
--							R15<= vR;
--					    else
--					        R15<=R15_timestamp;
						end if;
						if (action8(3 downto 0) = x"01" and execute='1' ) then
                            R15<= vR;
                        else
                            R15<=R15_timestamp;

					end if; --execute
				end if; --rst
		end if; --clk
end process;


Count_Pkts: process(S0_AXIS_ACLK, S0_AXIS_ARESETN)
	begin
		if (S0_AXIS_ACLK'event and S0_AXIS_ACLK = '1') then
			if (S0_AXIS_ARESETN = '0') then
				count_send_pkt <=x"00000000";
				count_timer<=x"00000000";
				count_pkt<=x"00000000";
			else
			    if (curr_state=reset_all) then
			        count_send_pkt <=x"00000000";
                    count_timer<=x"00000000";
                    count_pkt<=x"00000000";
                    end if;
				if (send='1') then
					count_send_pkt <= count_send_pkt+1;
				end if;
				if (timer_c='1') then
					count_timer <= count_timer+1;
					end if;
			    if (pkt='1') then
				   count_pkt <= count_pkt+1;
					
				end if;
			end if;
		end if;
end process;


-- from 0x8001_0000 to 0x8001_00FF
cam1_we <=  write_enable when (address(31 downto 8)=x"800100") else '0';
we_r0   <=  write_enable when (address(31 downto 8)=x"800200") else '0';



we_rdp_s0(0)  <=   write_enable when address(31 downto 8)=x"801000" else '0';
we_rdp_s0(1)  <=   write_enable when address(31 downto 8)=x"801100" else '0';
we_rdp_s0(2)  <=   write_enable when address(31 downto 8)=x"801200" else '0';
we_rdp_s0(3)  <=   write_enable when address(31 downto 8)=x"801300" else '0';
we_rdp_s0(4)  <=   write_enable when address(31 downto 8)=x"801400" else '0';
we_rdp_s0(5)  <=   write_enable when address(31 downto 8)=x"801500" else '0';
we_rdp_s0(6)  <=   write_enable when address(31 downto 8)=x"801600" else '0';
we_rdp_s0(7)  <=   write_enable when address(31 downto 8)=x"801700" else '0';
we_rdp_s0(8)  <=   write_enable when address(31 downto 8)=x"801800" else '0';
we_rdp_s1(0)  <=   write_enable when address(31 downto 8)=x"802000" else '0';
we_rdp_s1(1)  <=   write_enable when address(31 downto 8)=x"802100" else '0';
we_rdp_s1(2)  <=   write_enable when address(31 downto 8)=x"802200" else '0';
we_rdp_s1(3)  <=   write_enable when address(31 downto 8)=x"802300" else '0';
we_rdp_s1(4)  <=   write_enable when address(31 downto 8)=x"802400" else '0';
we_rdp_s1(5)  <=   write_enable when address(31 downto 8)=x"802500" else '0';
we_rdp_s1(6)  <=   write_enable when address(31 downto 8)=x"802600" else '0';
we_rdp_s1(7)  <=   write_enable when address(31 downto 8)=x"802700" else '0';
we_rdp_s1(8)  <=   write_enable when address(31 downto 8)=x"802800" else '0';
we_rdp_s2(0)  <=   write_enable when address(31 downto 8)=x"803000" else '0';
we_rdp_s2(1)  <=   write_enable when address(31 downto 8)=x"803100" else '0';
we_rdp_s2(2)  <=   write_enable when address(31 downto 8)=x"803200" else '0';
we_rdp_s2(3)  <=   write_enable when address(31 downto 8)=x"803300" else '0';
we_rdp_s2(4)  <=   write_enable when address(31 downto 8)=x"803400" else '0';
we_rdp_s2(5)  <=   write_enable when address(31 downto 8)=x"803500" else '0';
we_rdp_s2(6)  <=   write_enable when address(31 downto 8)=x"803600" else '0';
we_rdp_s2(7)  <=   write_enable when address(31 downto 8)=x"803700" else '0';
we_rdp_s2(8)  <=   write_enable when address(31 downto 8)=x"803800" else '0';









--REG_READ_PROCESS:
S_AXI_RDATA<= tcam_mask       when address= x"80009210" else
		start_register when address=  x"81000000" else
		reg_test0      when address = x"80000008" else
		R0             when address= x"80008004"  else
		R1             when address= x"80008008"  else
		R2             when address= x"8000800C"  else
		R3             when address= x"80008010"  else
		R4             when address= x"80008014"  else
		R5             when address= x"80008018"  else
		R6             when address= x"8000801C"  else
		R7             when address= x"80008020"  else
		R8             when address= x"80008024"  else
		R9             when address= x"80008028"  else
		R10            when address= x"8000802C"  else
		R11            when address= x"80008030"  else
		R12            when address= x"80008034"  else
		R13            when address= x"80008038"  else
		R14            when address= x"8000803C"  else
 		R15            when address= x"80008040"  else
		count_66       when address= x"800000b9" else
		count_1024     when address= x"800000c0" else
		present_state  when address= x"800000B3" else
		future_state  when address= x"800000B4" else
		count_send_pkt when address= x"800000B0"  else
		count_timer    when address= x"800000B1"  else
		count_pkt      when address= x"800000B2" else
		(x"000000")&match_addr_tcam_t   when address= x"800000B5" else
		end_count      when address=x"800000B6" else
		conditions_reg when address=x"800000B7" else
		conditions_reg_1 when address=x"800000B8" else
		read_from_ram0 when address(31 downto 8)=x"800200" else
		read_from_ram_s0(0) when address(31 downto 8)=x"801000" else
		read_from_ram_s0(1) when address(31 downto 8)=x"801100" else
		read_from_ram_s0(2) when address(31 downto 8)=x"801200" else
		read_from_ram_s0(3) when address(31 downto 8)=x"801300" else
		read_from_ram_s0(4) when address(31 downto 8)=x"801400" else
		read_from_ram_s0(5) when address(31 downto 8)=x"801500" else
		read_from_ram_s0(6) when address(31 downto 8)=x"801600" else
		read_from_ram_s0(7) when address(31 downto 8)=x"801700" else
		read_from_ram_s0(8) when address(31 downto 8)=x"801800" else
		read_from_ram_s1(0) when address(31 downto 8)=x"802000" else
		read_from_ram_s1(1) when address(31 downto 8)=x"802100" else
		read_from_ram_s1(2) when address(31 downto 8)=x"802200" else
		read_from_ram_s1(3) when address(31 downto 8)=x"802300" else
		read_from_ram_s1(4) when address(31 downto 8)=x"802400" else
		read_from_ram_s1(5) when address(31 downto 8)=x"802500" else
		read_from_ram_s1(6) when address(31 downto 8)=x"802600" else
		read_from_ram_s1(7) when address(31 downto 8)=x"802700" else
		read_from_ram_s1(8) when address(31 downto 8)=x"802800" else
		read_from_ram_s2(0) when address(31 downto 8)=x"803000" else
		read_from_ram_s2(1) when address(31 downto 8)=x"803100" else
		read_from_ram_s2(2) when address(31 downto 8)=x"803200" else
		read_from_ram_s2(3) when address(31 downto 8)=x"803300" else
		read_from_ram_s2(4) when address(31 downto 8)=x"803400" else
		read_from_ram_s2(5) when address(31 downto 8)=x"803500" else
		read_from_ram_s2(6) when address(31 downto 8)=x"803600" else
		read_from_ram_s2(7) when address(31 downto 8)=x"803700" else
		read_from_ram_s2(8) when address(31 downto 8)=x"803800" else
		CR_1 when address=x"81100000" else
		CR_2 when address=x"81200000" else
		CR_3 when address=x"81300000" else
		CR_4 when address=x"81400000" else
		CR_5 when address=x"81500000" else
		CR_6 when address=x"81600000" else
		CR_7 when address=x"81700000" else
		CR_8 when address=x"81800000" else
		CR_9 when address=x"81900000" else
		CR_10 when address=x"81a00000" else
		CR_11 when address=x"81b00000" else
		x"deadbeef";


REG_WRITE_PROCESS: process(S_AXI_ACLK)
	begin
		if (S_AXI_ACLK'event and S_AXI_ACLK = '1') then
			if (S_AXI_ARESETN = '0') then
				R0_in <= x"00000000";
				R1_in <= x"00000000";
				R2_in <= x"00000000";
				R3_in <= x"00000000";

				R4_in <= x"00000000";
				R5_in <= x"00000000";
				R6_in <= x"00000000";
				R7_in <= x"00000000";
				R8_in <= x"00000000";
				R9_in <= x"00000000";
				R10_in <= x"00000000";
				R11_in <= x"00000000";
				R12_in <= x"00000000";
				R13_in <= x"00000000";
				R14_in <= x"00000000";
				R15_in <= x"00000000";
				ConfH1_offset <=x"00000000";
				ConfH1_size <= x"00000000";
				ConfH2_offset <=x"00000000";
				ConfH2_size <= x"00000000";
				start_register <= (others =>'0');
				CR_1      <= (others =>'0');
				CR_2      <= (others =>'0');
				CR_3      <= (others =>'0');
				CR_4      <= (others =>'0');
				CR_5      <= (others =>'0');
				CR_6      <= (others =>'0');
				CR_7      <= (others =>'0');
				CR_8      <= (others =>'0');
				CR_9      <= (others =>'0');
				CR_10      <= (others =>'0');
				CR_11     <= (others =>'0');
--if(IN_SYNTHESIS) then
--    reg_test0      <= (others =>'0');
--else
--    reg_test0      <= x"00000001";
--end if;
			elsif(curr_state=reset_all) then
			   CR_1<=X"00000000";
			   CR_2<=X"00000000";
			   CR_3<=X"00000000";
			   CR_4<=X"00000000";
			   CR_5<=X"00000000";
			   CR_6<=X"00000000";
			   CR_7<=X"00000000";
			   CR_8<=X"00000000";
			   CR_9<=X"00000000";
			   CR_10<=X"00000000";
			   CR_11<=X"00000000";
			
--			   end if;
			
			
			
			elsif (write_enable='1') then
				if (address=x"81000000") then
					start_register <= S_AXI_WDATA;
				end if;
				if (address=x"80008000") then
					R0_in <= S_AXI_WDATA;
				end if;
				if (address=x"80008004") then
					R1_in <= S_AXI_WDATA;
				end if;
				if (address=x"80008008") then
					R2_in <= S_AXI_WDATA;
				end if;
				if (address=x"8000800C") then
					R3_in <= S_AXI_WDATA;
				end if;

				if (address=x"80008010") then
					R4_in <= S_AXI_WDATA;
				end if;
				if (address=x"80008014") then
					R5_in <= S_AXI_WDATA;
				end if;
				if (address=x"80008018") then
					R6_in <= S_AXI_WDATA;
				end if;
				if (address=x"8000801C") then
					R7_in <= S_AXI_WDATA;
				end if;
				if (address=x"80008020") then
					R8_in <= S_AXI_WDATA;
				end if;
				if (address=x"80008024") then
					R9_in <= S_AXI_WDATA;
				end if;
				if (address=x"80008028") then
					R10_in <= S_AXI_WDATA;
				end if;
				if (address=x"8000802C") then
					R11_in <= S_AXI_WDATA;
				end if;
				if (address=x"80008030") then
					R12_in <= S_AXI_WDATA;
				end if;
				if (address=x"80008034") then
					R13_in <= S_AXI_WDATA;
				end if;
				if (address=x"80008038") then
					R14_in <= S_AXI_WDATA;
				end if;
				if (address=x"8000803C") then
					R15_in <= S_AXI_WDATA;
				end if;
				if (address=x"80009000") then
					ConfM1 <= S_AXI_WDATA;
				end if;
				if (address=x"80009004") then
					ConfM2 <= S_AXI_WDATA;
				end if;
				if (address=x"80009008") then
					ConfM3 <= S_AXI_WDATA;
				end if;
				if (address=x"8000900C") then
					ConfM4 <= S_AXI_WDATA;
				end if;
				if (address=x"80009100") then
					ConfH1_offset <= S_AXI_WDATA;
				end if;
				if (address=x"80009104") then
					ConfH2_offset <= S_AXI_WDATA;
				end if;
				if (address=x"80009108") then
					ConfH3_offset <= S_AXI_WDATA;
				end if;
				if (address=x"8000910C") then
					ConfH4_offset <= S_AXI_WDATA;
				end if;
				if (address=x"80009200") then
					ConfH1_size <= S_AXI_WDATA;
				end if;
				if (address=x"80009204") then
					ConfH2_size <= S_AXI_WDATA;
				end if;
				if (address=x"80009208") then
					ConfH3_size <= S_AXI_WDATA;
				end if;
				if (address=x"8000920C") then
					ConfH4_size <= S_AXI_WDATA;
				end if;
				if (address=x"80009210") then
					tcam_mask <= S_AXI_WDATA;
				end if;
				if (address=x"81100000") then
					CR_1 <= S_AXI_WDATA;
				end if;
				if (address=x"81200000") then
					CR_2 <= S_AXI_WDATA;
				end if;
				if (address=x"81300000") then
					CR_3 <= S_AXI_WDATA;
				end if;
				if (address=x"81400000") then
					CR_4 <= S_AXI_WDATA;
				end if;
				if (address=x"81500000") then
					CR_5 <= S_AXI_WDATA;
				end if;
				if (address=x"81600000") then
					CR_6 <= S_AXI_WDATA;
				end if;
				if (address=x"81700000") then
					CR_7 <= S_AXI_WDATA;
				end if;
				if (address=x"81800000") then
					CR_8 <= S_AXI_WDATA;
				end if;
				if (address=x"81900000") then
					CR_9 <= S_AXI_WDATA;
				end if;
				if (address=x"81a00000") then
					CR_10 <= S_AXI_WDATA;
				end if;
				if (address=x"81b00000") then
					CR_11 <= S_AXI_WDATA;
				end if;
			end if;
		end if;
end process;


-- unused signals
S_AXI_BRESP <= "00";
S_AXI_RRESP <= "00";

-- axi-lite slave state machine
AXI_SLAVE_FSM: process (S_AXI_ACLK)
	begin
		if rising_edge(S_AXI_ACLK) then
			if S_AXI_ARESETN='0' then -- slave reset state
				S_AXI_RVALID <= '0';
				int_S_AXI_BVALID <= '0';
				S_AXI_ARREADY <= '0';
				S_AXI_WREADY <= '0';
				S_AXI_AWREADY <= '0';
				--axi_state <= addr_wait;
				axi_state <= "000";
				address <= (others=>'0');
				write_enable <= '0';
			else
				case axi_state is
--when addr_wait =>
					when "000" =>
						S_AXI_AWREADY <= '1';
						S_AXI_ARREADY <= '1';
						S_AXI_WREADY <= '0';
						S_AXI_RVALID <= '0';
						int_S_AXI_BVALID <= '0';
						read_enable <= '0';
						write_enable <= '0';
						-- wait for a read or write address and latch it in
							if S_AXI_ARVALID = '1' then -- read
								--axi_state <= read_state;
								axi_state <= "001";   -- TODO: only when curr_state=IDLE. Also put pause=1
								address <= S_AXI_ARADDR - (MY_BASEADDR -x"80000000");
								read_enable <= '1';
							elsif (S_AXI_AWVALID = '1' and S_AXI_WVALID = '1') then -- write
								--axi_state <= write_state;
								axi_state <= "100";
								address <= S_AXI_AWADDR - (MY_BASEADDR -x"80000000");
							else
								--axi_state <= addr_wait;
								axi_state <= "000";
							end if;

				--when read_state (wait1) =>
					when "001" =>
						read_enable <= '1';
						S_AXI_AWREADY <= '0';
						S_AXI_ARREADY <= '0';
						-- place correct data on bus and generate valid pulse
						int_S_AXI_BVALID <= '0';
						S_AXI_RVALID <= '0';
						--axi_state <= read_wait2;
						axi_state <= "010";

				--when read_state (wait2) =>
					when "010" =>
						read_enable <= '1';
						S_AXI_AWREADY <= '0';
						S_AXI_ARREADY <= '0';
						-- place correct data on bus and generate valid pulse
						int_S_AXI_BVALID <= '0';
						S_AXI_RVALID <= '0';
						--axi_state <= response_state;
						axi_state <= "011";

					--when read_state =>
					when "011" =>
						read_enable <= '1';
						S_AXI_AWREADY <= '0';
						S_AXI_ARREADY <= '0';
						-- place correct data on bus and generate valid pulse
						int_S_AXI_BVALID <= '0';
						S_AXI_RVALID <= '1';
						--axi_state <= response_state;
						axi_state <= "111";

					--when write_state =>
					when "100" =>
						-- generate a write pulse
						write_enable <= '1';
						S_AXI_AWREADY <= '0';
						S_AXI_ARREADY <= '0';
						S_AXI_WREADY <= '1';
						int_S_AXI_BVALID <= '1';
						--axi_state <= response_state;
						axi_state <= "111";

					--when response_state =>
					when "111" =>
						read_enable <= '0';
						write_enable <= '0';
						S_AXI_AWREADY <= '0';
						S_AXI_ARREADY <= '0';
						S_AXI_WREADY <= '0';
						-- wait for response from master
						if (int_S_AXI_BVALID = '0' and S_AXI_RREADY = '1') or (int_S_AXI_BVALID = '1' and S_AXI_BREADY = '1') then
							S_AXI_RVALID <= '0';
							int_S_AXI_BVALID <= '0';
								--axi_state <= addr_wait;
							axi_state <= "000";
						else
						--axi_state <= response_state;
							axi_state <= "111";
						end if;
					when others =>
						null;
				end case;
			end if;
		end if;
end process;
S_AXI_BVALID <= int_S_AXI_BVALID;


end architecture full;

--------------------
-- Address space ---
--------------------
--    cam1_we x"800100"';
--    we_r0   x"800200";
--    we_r1   x"800300";
--    we_r2   x"800400";
--    we_r3   x"800500";
--    we_r4   x"800600";
--    we_r5   x"800700";
--    we_r6   x"800800";

--     reg_test0   x"80000008"
--     read_from_ram0 x"800200"
--     read_from_ram1 x"800300"
--     read_from_ram2 x"800400"
--     read_from_ram3 x"800500"
--     read_from_ram4 x"800600"
--     read_from_ram5 x"800700"
--     read_from_ram6 x"800800"

--     start_register      x"81000000"
--     R0_in               x"80008000"
--     R1_in               x"80008004"
--     R2_in               x"80008008"
--     R3_in               x"8000800C"
--     R4_in               x"80008010"
--     R5_in               x"80008014"
--     R6_in               x"80008018"
--     R7_in               x"8000801C"
--     R8_in               x"80008020"
--     R9_in               x"80008024"
--     R10_in              x"80008028"
--     R11_in              x"8000802C"
--     R12_in              x"80008030" 
--     R13_in              x"80008034"
--     R14_in              x"80008038"
--     R15_in              x"8000803C"
--     ConfM1              x"80009000"
--     ConfM2              x"80009004"
--     ConfM3              x"80009008"
--     ConfM4              x"8000900C"
--     ConfH1_offset       x"80009100"
--     ConfH2_offset       x"80009104"
--     ConfH3_offset       x"80009108"
--     ConfH4_offset       x"8000910C"
--     ConfH1_size         x"80009200"
--     ConfH2_size         x"80009204"
--     ConfH3_size         x"80009208"
--     ConfH4_size         x"8000920C"
--     tcam_mask           x"80009210"
