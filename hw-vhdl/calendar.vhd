----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 12/15/2017 12:43:43 PM
-- Design Name: 
-- Module Name: calendar - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use ieee.std_logic_1164.all;
use IEEE.STD_LOGIC_unsigned.ALL;
use ieee.std_logic_arith.all;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity calendar is
    Port ( 
           
           S0_AXIS_ACLK    : in std_logic;
           S0_AXIS_ARESETN : in std_logic;
           --clock : in STD_LOGIC;
           --RESETN : in STD_LOGIC;
           
           --MI interface
           --we: in std_logic;
           --rd: in std_logic;
           --input_din: in std_logic_vector(73 downto 0);
           --addr :in std_logic_vector(11 downto 0);
           --data_out :out std_logic_vector(73 downto 0);         
                    
           ---
           out_count_item: out std_logic_vector(31 downto 0); 
           
           S0_AXIS_TVALID  : in std_logic;
           S0_AXIS_TDATA   : in std_logic_vector(255 downto 0);
           S0_AXIS_TKEEP   : in std_logic_vector(31 downto 0);
           S0_AXIS_TUSER   : in std_logic_vector(127 downto 0);
           S0_AXIS_TLAST   : in std_logic;
           S0_AXIS_TREADY  : out std_logic;

           
           M0_AXIS_TVALID   : out std_logic;
           M0_AXIS_TDATA    : out std_logic_vector(255 downto 0);
           M0_AXIS_TKEEP    : out std_logic_vector(31 downto 0);
           M0_AXIS_TUSER    : out std_logic_vector(127 downto 0);
           M0_AXIS_TLAST    : out std_logic;
           M0_AXIS_TREADY   : in std_logic
           
           );
    attribute core_generation_info : string;
    attribute core_generation_info of calendar : entity is "calendar{x_ipProduct=Vivado 2016.4,x_ipVendor=cnit.it}";
end calendar;

architecture Behavioral of calendar is

type state_type is (WAIT_STATE,INSERTING,DOUBLEINSERTING);  --type of state machine.
signal state: state_type;
signal collision,search,insert_ht:std_logic;
signal timer,timer2: STD_LOGIC_VECTOR (39 downto 0);
signal present_timer: STD_LOGIC_VECTOR (31 downto 0);
signal nanotimer: std_logic_vector(7 downto 0);
signal value,value2: STD_LOGIC_VECTOR (39 downto 0);


--AXIS to calendar

signal num_insert  : STD_LOGIC_VECTOR (1 downto 0);
signal delta1   : STD_LOGIC_VECTOR (31 downto 0);
signal delta2   : STD_LOGIC_VECTOR (31 downto 0);
signal value_in1   : STD_LOGIC_VECTOR (39 downto 0);
signal value_in2   : STD_LOGIC_VECTOR (39 downto 0);
signal insert_done : STD_LOGIC;

signal expired,delayed_expired     : STD_LOGIC;
signal value_out   : STD_LOGIC_VECTOR (39 downto 0);
           

--MI interface
signal we       : std_logic;
signal rd       : std_logic;
signal input_din: std_logic_vector(81 downto 0);
signal addr     : std_logic_vector(11 downto 0);
signal data_out : std_logic_vector(81 downto 0);         
          

--constant TIME_RESOLUTION:  std_logic_vector(7 downto 0):=x"27" -- 250 ns resolution
constant TIME_RESOLUTION:  std_logic_vector(7 downto 0):=x"9C";  -- 1us resolution 
 

attribute X_INTERFACE_INFO: string;
attribute X_INTERFACE_INFO of S0_AXIS_ACLK: signal is "xilinx.com:signal:clock:1.0 S0_AXIS_ACLK CLK";
attribute X_INTERFACE_PARAMETER: string;
attribute X_INTERFACE_PARAMETER of S0_AXIS_ACLK: signal is "ASSOCIATED_BUSIF M0_AXIS:S0_AXIS";                     


begin

num_insert     <= S0_AXIS_TUSER(33 downto 32) when S0_AXIS_TVALID='1' else "00";
S0_AXIS_TREADY <= insert_done;

delta1 <= S0_AXIS_TDATA(31 downto 0);
delta2 <= S0_AXIS_TDATA(63 downto 32);
value_in1 <= S0_AXIS_TDATA(103 downto 64);           
value_in2 <= S0_AXIS_TDATA(143 downto 104);    



process(S0_AXIS_ACLK)
    begin
	if(S0_AXIS_ACLK'event and S0_AXIS_ACLK = '1') then
	    if (S0_AXIS_ARESETN = '0') then
            delayed_expired <= '0';
	    else
            delayed_expired <= expired;
	    end if;
	end if;
end process;


M0_AXIS_TVALID<=expired or delayed_expired;
M0_AXIS_TUSER(127 downto 0)<= x"00000000000000000000000000FF0020" when expired='1' else (others =>'0');
M0_AXIS_TDATA(39 downto 0)<= value_out when expired='1' else (others =>'0');
M0_AXIS_TDATA(255 downto 40)<= (others =>'0');
M0_AXIS_TKEEP<= (others =>'1');
M0_AXIS_TLAST<=delayed_expired;
       


we<='0';
rd<='0';
input_din<= (others =>'0');
addr<= (others =>'0');

HT1: entity work.ht128dp_calendar
    generic map (
                    key_len =>40, --32 + 8 per avere 256 possibilita'
                    value_len => 40 -- 32 + 8 
                )
    port map(
                clock => S0_AXIS_ACLK,
                RESETN => S0_AXIS_ARESETN,
                
                --AXI interface
                we => we,
                rd => rd,
                input_din => input_din,
                addr  => addr,
                data_out => data_out,
                out_count_item => out_count_item,
                 
                -- if find remove!
                remove => '1', 
                search_key=> (present_timer & nanotimer),
                match  => expired,
                output => value_out,
                            
                insert => insert_ht,
                key    => (timer(39 downto 2) & "00"),
                value  => value,
                clear_count_collision => '0',
                collision => collision
            );


process(S0_AXIS_ACLK)
    begin
	if(S0_AXIS_ACLK'event and S0_AXIS_ACLK = '1') then
	    if (S0_AXIS_ARESETN = '0') then
		present_timer <= (others=>'0');
		nanotimer<= (others => '0');
	    else
		nanotimer <= nanotimer+1;
		if (nanotimer=TIME_RESOLUTION) then
		    nanotimer <= x"00";
		    present_timer <= present_timer+1;
		end if;
	    end if;
	end if;
end process;


-- se collision=0 stop 
-- se collision=1 insert in timer_in+1 


process(S0_AXIS_ACLK)
    begin
      if(S0_AXIS_ACLK'event and S0_AXIS_ACLK = '1') then
		if (S0_AXIS_ARESETN = '0') then
		  state<=WAIT_STATE;
		  insert_done<='1';
		  insert_ht <='0';
		else
		case state is
            when WAIT_STATE =>        --when current state is "s0"
                insert_done<='1';
                if(num_insert ="01") then
                    state<=INSERTING;
                    timer<=(present_timer+delta1) & nanotimer;
                    value<=value_in1;
                    insert_ht<='1';
                    insert_done<='0';
                end if; 
                if(num_insert ="10") then
                    state<=DOUBLEINSERTING;
                    timer<=(present_timer+delta1) & nanotimer;
                    value<=value_in1;
                    timer2<=(present_timer+delta2) & nanotimer;
                    value2<=value_in2;
                    insert_ht<='1';
                    insert_done<='0';
                end if;  
            when DOUBLEINSERTING =>        --when current state is "s1"
                if(collision ='0') then
                    state<=INSERTING;
                    timer<=timer2;
                    value<=value2;
                    insert_ht<='1';
                else
                    insert_ht<='1';
                    timer<=timer+1;
		    if (timer(7 downto 0)=TIME_RESOLUTION) then 
			timer(7 downto 0) <= x"00";
			timer(39 downto 8) <= timer(39 downto 8) +1;
		    end if;
                end if;
            when INSERTING =>        --when current state is "s1"
                if(collision ='0') then
                    insert_ht<='0';
                    state<=WAIT_STATE;
                else
                    insert_ht<='1';
                    timer<=timer+1;
		    if (timer(7 downto 0)=TIME_RESOLUTION) then 
			timer(7 downto 0) <= x"00";
			timer(39 downto 8) <= timer(39 downto 8) +1;
		    end if;
                end if;
	    end case;
	  end if;  
	end if;
end process;
		
end Behavioral;
