library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
use work.salutil.all;
use std.textio.all;
use ieee.std_logic_textio.all;

entity ram4096x32dp is -- 32x256
          generic (init_file: string:="";
      -- Parameters of AxiStream Slave Bus Interface S00_AXIS
                      C_S00_AXIS_DATA_WIDTH  : integer   := 256;
                      C_S00_AXIS_TUSER_WIDTH : integer   := 128;
      -- Parameters of AxiStream Master Bus Interface M00_AXIS
                      C_M00_AXIS_DATA_WIDTH  : integer   := 256;
                      C_M00_AXIS_TUSER_WIDTH : integer   := 128;
      -- Parameters of Axi Slave Bus Interface S00_AXIS
                      C_S00_AXI_DATA_WIDTH  : integer   := 32;
                      C_S00_AXI_ADDR_WIDTH  : integer   := 32;
                      C_BASEADDR  : std_logic_vector(31 downto 0)   := x"90000000"
      --                C_HIGHADDR  : std_logic_vector(31 downto 0)   := x"9FFFFFFF"
               );
      port (
             --AXI interface
                         axi_clock : in std_logic;
                         we: in std_logic;
                         re: in std_logic;
             axi_addr : in std_logic_vector(31 downto 0);
             axi_data_in : in std_logic_vector(31 downto 0);
             axi_data_out : out std_logic_vector(31 downto 0);

             -- AXIS interface
             clock : in std_logic;
                         addr  : in std_logic_vector(7 downto 0);
                         data_out : out std_logic_vector(31 downto 0)
                );
end ram4096x32dp;

architecture behavioral of ram4096x32dp is

type mem_type_32_32 is array (0 to 65535) of std_logic_vector(31 downto 0);

impure function InitRamFromFile (RamFileName : in string) return mem_type_32_32 is
    FILE ramfile : text is in RamFileName;
    variable RamFileLine : line;
    variable ram : mem_type_32_32;
        begin
            for i in mem_type_32_32'range loop
                readline(ramfile, RamFileLine);
                hread(RamFileLine, ram(i));
            end loop;
    return ram;
end function;

signal axi_data_out_temp : std_logic_vector (31 downto 0);
signal mem1: mem_type_32_32:=InitRamFromFile(init_file);
signal MY_BASEADDR  : std_logic_vector(31 downto 0)   := x"90000000";

begin

process(axi_clock)
    begin
        if axi_clock'event and axi_clock = '1' then
                        if (we = '1') then
                            mem1(conv_integer(axi_addr(15 downto 0))) <= axi_data_in;
                        else
                            axi_data_out <= mem1(conv_integer(axi_addr(15 downto 0)));
            end if;
        end if;                 
end process;


process(clock)
    begin
        if clock'event and clock = '1' then
                        data_out <= mem1(conv_integer(addr));
    end if;                     
 end process;

end behavioral;