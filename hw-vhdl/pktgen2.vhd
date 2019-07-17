------------------------------
---                        ---
---       PKT GEN 2        ---
---                        ---
------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.std_logic_signed.all;
use ieee.numeric_std.all;

library work;
use work.salutil.all;

entity pktgen2 is
  Generic (-- ETH header, 112 bits
           eth_pckt_type   : std_logic_vector(15 downto 0) := x"0800"; -- IPv4
           -- IPv4 header
           ip_ver_hdr_len  : std_logic_vector(7 downto 0)  := x"45";
           ip_diff_serv    : std_logic_vector(7 downto 0)  := x"00"; -- differentiated services
           ip_tot_len      : std_logic_vector(15 downto 0) := x"0434"; -- packet total len in bytes
           ip_identif      : std_logic_vector(15 downto 0) := x"9f1b";
           ip_flags        : std_logic_vector(7 downto 0)  := x"40"; -- don't fragment
           ip_frag_offset  : std_logic_vector(7 downto 0)  := x"00";
           ip_ttl          : std_logic_vector(7 downto 0)  := x"34"; -- time to live
           ip_proto        : std_logic_vector(7 downto 0)  := x"06"; -- TCP protocol
           ip_checksum     : std_logic_vector(15 downto 0) :=  x"709f";
           ip_src          : std_logic_vector(31 downto 0) := x"4f062e26";
           ip_dst          : std_logic_vector(31 downto 0) := x"0a637ca9";
           -- void 32 bits for sequence number
           tcp_ack_num     : std_logic_vector(31 downto 0) := x"00000001";
           -- void 16 bits for length + tcp_flags
           tcp_win_size    : std_logic_vector(15 downto 0) := x"faf0";
           tcp_checksum    : std_logic_vector(15 downto 0) := x"7e83";
           tcp_urgent_ptr  : std_logic_vector(15 downto 0) := x"0000";
           -- 20 bytes of data
           packet_len      : std_logic_vector(15 downto 0) := x"0442" -- total packet len in bytes
           );
           
      Port (clock          : in std_logic;
            resetn         : in std_logic;
            S0_AXIS_TVALID  : in std_logic;
            S0_AXIS_TDATA   : in std_logic_vector(511 downto 0);
            S0_AXIS_TKEEP   : in std_logic_vector(63 downto 0);
            S0_AXIS_TUSER   : in std_logic_vector(255 downto 0);
            S0_AXIS_TLAST   : in std_logic;
            S0_AXIS_TREADY  : out std_logic;
            -- NF datapath AXI-S
            m_axis_tvalid  : out std_logic;
            m_axis_tdata   : out std_logic_vector(255 downto 0);
            m_axis_tkeep   : out std_logic_vector(31 downto 0);
            m_axis_tready  : in std_logic;
            m_axis_tlast   : out std_logic;
            m_axis_tuser   : out std_logic_vector(127 downto 0);
            -- RAM
            we : in std_logic;
            axi_addr : in std_logic_vector (31 downto 0);
            data_scritto : in std_logic_vector (31 downto 0);      
            data_letto : out std_logic_vector(31 downto 0)
            );
            end pktgen2;

architecture Behavioral of pktgen2 is
  type State_type is ( reset_state, waiting, sending_1, sending_2, sending_3, sending_4,sending_5);
                      -- Define the states
  signal state : State_Type;
  signal frame_sent : std_logic_vector (4 downto 0) := (others => '0');

  -- Control signals from OPP
  signal     header           : std_logic_vector(511 downto 0);
  signal     send             : std_logic;
  signal     ready            : std_logic;

  -- Control signals from RAMs
  signal addr_seq : std_logic_vector(31 downto 0);
  signal axi_clock : std_logic;
  signal axi_data_in : std_logic_vector(31 downto 0);
  signal axi_data_out :std_logic_vector(31 downto 0);
  signal we1, we2, we3, we4, we5, we6, we7, we8: std_logic := '0';
  -- Contro signal size file in RAMs
  signal buff_dim : std_logic_vector(31 downto 0):=x"00040000";   --x"00022257";
  -- AXIS interface
  signal addr  : std_logic_vector(31 downto 0);
  signal data_out_1 :std_logic_vector (31 downto 0);
  signal data_out_2 :std_logic_vector (31 downto 0);
  signal data_out_3 :std_logic_vector (31 downto 0);
  signal data_out_4 :std_logic_vector (31 downto 0);
  signal data_out_5 :std_logic_vector (31 downto 0);
  signal data_out_6 :std_logic_vector (31 downto 0);
  signal data_out_7 :std_logic_vector (31 downto 0);
  signal data_out_8 :std_logic_vector (31 downto 0);
  signal data_out_8_temp :std_logic_vector (15 downto 0);

  signal data_letto_1 :  std_logic_vector (31 downto 0);
  signal data_letto_2 :  std_logic_vector (31 downto 0);
  signal data_letto_3 :  std_logic_vector (31 downto 0);
  signal data_letto_4 :  std_logic_vector (31 downto 0);
  signal data_letto_5 :  std_logic_vector (31 downto 0);
  signal data_letto_6 :  std_logic_vector (31 downto 0);
  signal data_letto_7 :  std_logic_vector (31 downto 0);
  signal data_letto_8 :  std_logic_vector (31 downto 0);



begin
send <= S0_AXIS_TVALID;
S0_AXIS_TREADY <= ready;

SEND_PCKT_FSM: process(clock)
begin
if rising_edge (clock) then
                       if (resetn = '0') then
                       state <= reset_state;
                       frame_sent <= (others => '0');
		       header <=  (others => '0');
		       addr_seq <=  (others => '0');
                       else
                           case state is
                           when reset_state => state <= waiting;
                           when waiting => frame_sent <= (others => '0');
                                           if (send = '1') then
                                                header <= S0_AXIS_TDATA;
						addr_seq <= S0_AXIS_TUSER(31 downto 0);
                                                state <= sending_1;
                                           end if;
                            when sending_1 => if (m_axis_tready = '1') then
                                              state <= sending_2;
                                              end if;
                            when sending_2 => if (m_axis_tready = '1') then
					      addr_seq <= addr_seq+32;
                                              state <= sending_3;
                                              end if;
                            when sending_3 => if (m_axis_tready = '1') then
					      addr_seq <= addr_seq+32;
                                              state <= sending_4;
                                              end if;
                            when sending_4 => if (frame_sent /= "11110") and (m_axis_tready = '1') then
					      addr_seq <= addr_seq+32;
                                              frame_sent <= frame_sent + 1;
                                              elsif (frame_sent = "11110") and (m_axis_tready = '1') then
                                              state <= sending_5;
                                              end if;
                            when sending_5 => if (m_axis_tready = '1') then
                                              state <= waiting;
                                              end if;
                            when others => state <= reset_state;
                            end case;
                     end if;
end if;
end process;



SEND_PCKT: process(buff_dim,data_out_1,data_out_2,data_out_3,data_out_4,data_out_5,data_out_6,data_out_7,data_out_8_temp, data_out_8,state)
           begin
       ready <= '0';
       m_axis_tvalid <= '0';
       m_axis_tdata <= (others => '0');
       m_axis_tlast <= '0';
       m_axis_tuser <= (others => '0');
       m_axis_tkeep <= (others => '0');

       case state is
       when reset_state => ready <= '1';
                           m_axis_tvalid <= '0';
                           m_axis_tdata <= (others => '0');
                           m_axis_tlast <= '0';
                           m_axis_tuser <= (others => '0');
                           m_axis_tkeep <= (others => '0');
                           --buff_dim <= x"00040000";
       when waiting => ready <= '1';
                       m_axis_tvalid <= '0';
                       m_axis_tdata <= (others => '0');
                       m_axis_tlast <= '0';
                       m_axis_tuser <= (others => '0');
                       m_axis_tkeep <= (others => '0');
       when sending_1 => ready <= '0';
                         m_axis_tvalid <= '1';
                         m_axis_tdata <= header (255 downto 0);
                         m_axis_tkeep <= (others => '1');
                         m_axis_tlast <= '0';
                         m_axis_tuser <= x"000000000000000000000000" & x"00" & x"00" & x"0442";
        when sending_2 => ready <= '0';
                         m_axis_tvalid <= '1';
                         m_axis_tdata <= header(511 downto 256);
                         m_axis_tkeep <= (others => '1');
                         m_axis_tlast <= '0';
                         m_axis_tuser <= (others => '0');
        when sending_3 => ready <= '0';
                         m_axis_tvalid <= '1';
                         m_axis_tdata <= data_out_8(15 downto 0) & data_out_7 & data_out_6 & data_out_5 & data_out_4 & data_out_3 & data_out_2 & data_out_1 & x"0101"; 
                         m_axis_tkeep <= (others => '1');
                         m_axis_tlast <= '0';
                         m_axis_tuser <= (others => '0');
        when sending_4 => ready <= '0';
                         m_axis_tvalid <= '1';
                         m_axis_tdata <= data_out_8(15 downto 0)  & data_out_7 & data_out_6 & data_out_5 & data_out_4 & data_out_3& data_out_2 & data_out_1 & data_out_8_temp ;
                         m_axis_tkeep <= (others => '1');
                         m_axis_tlast <= '0';
                         m_axis_tuser <= (others => '0');
        when sending_5 => ready <= '0';
                         m_axis_tvalid <= '1';
                         m_axis_tdata(255 downto 240) <= data_out_8_temp;
                         m_axis_tdata(239 downto 0) <= (others=>'0');
                         m_axis_tuser <= (others => '0');
                         m_axis_tkeep<=x"00000003";
                         m_axis_tlast <= '1';
        when others => ready <= '0';
                       m_axis_tvalid <= '0';
                       m_axis_tdata <= (others => '0');
                       m_axis_tkeep <= (others => '1');
                       m_axis_tlast <= '0';
                       m_axis_tuser <= (others => '0');
        end case;
end process;
      
      
MAC_ADDR_AND_BUFF_DIM: process (axi_addr,buff_dim,data_letto_1,data_letto_2,data_letto_3,
                                data_letto_4,data_letto_5,data_letto_6,data_letto_7,data_letto_8,data_scritto, we, we1, we2, we3,
                                we4, we5, we6, we7, we8,data_scritto)
 begin
 if (we='0')then 
    case axi_addr is
    when x"91fffffb" => data_letto <= buff_dim;
    when others =>  case axi_addr(2 downto 0) is
                    when "000" => data_letto <= data_letto_1;
                    when "001" => data_letto <= data_letto_2;
                    when "010" => data_letto <= data_letto_3;
                    when "011" => data_letto <= data_letto_4;
                    when "100" => data_letto <= data_letto_5;
                    when "101" => data_letto <= data_letto_6;
                    when "110" => data_letto <= data_letto_7;
                    when others => data_letto <= data_letto_8;
                    end case;
    end case; 
 else -- write_enable = 1
 
    case axi_addr is
    when others => case axi_addr (2 downto 0) is
                   when "000" => we1 <= we;
                   when "001" => we2 <= we;
                   when "010" => we3 <= we;
                   when "011" => we4 <= we;
                   when "100" => we5 <= we;
                   when "101" => we6 <= we;
                   when "110" => we7 <= we;
                   when others => we8 <= we;
                   end case;
    end case;

end if;
end process;


ram1: entity work.ram4096x32_1
      generic map (init_file => "/home/aniello/Documenti/opp-tcp_copia/scripts/python/outputfile_1.mif")
      port map (--AXI interface
                   axi_clock => clock,
                   we1 => we1,
                   --en1 => en1,
                   axi_addr => axi_addr(15 downto 3),
                   axi_data_in => data_scritto,
                   axi_data_out => data_letto_1,
                   -- AXIS interface
                   clock => clock,
                   addr => "00000" & addr_seq(31 downto 5) ,
                   data_out => data_out_1         
                );
ram2: entity work.ram4096x32_1
      generic map (init_file => "/home/aniello/Documenti/opp-tcp_copia/scripts/python/outputfile_2.mif")
      port map (--AXI interface
                   axi_clock => clock,
                   we1 => we2,
                   --en1 => en2,
                   axi_addr => axi_addr(15 downto 3),
                   axi_data_in => data_scritto,
                   axi_data_out => data_letto_2,
                   -- AXIS interface
                    clock => clock,
                    addr => "00000" & addr_seq(31 downto 5) ,
                    data_out => data_out_2         
                );
ram3: entity work.ram4096x32_1
      generic map (init_file => "/home/aniello/Documenti/opp-tcp_copia/scripts/python/outputfile_3.mif")
      port map (--AXI interface
             axi_clock => clock,
             we1 => we3,
             --en1 => en2,
             axi_addr => axi_addr(15 downto 3),
             axi_data_in => data_scritto,
             axi_data_out => data_letto_3, 
             -- AXIS interface
             clock => clock,
             addr => "00000" & addr_seq(31 downto 5),
             data_out => data_out_3         
              );
ram4: entity work.ram4096x32_1
      generic map (init_file => "/home/aniello/Documenti/opp-tcp_copia/scripts/python/outputfile_4.mif")
      port map (--AXI interface
                   axi_clock => clock,
                   we1 => we4,
                  -- en4 => en4,
                   axi_addr => axi_addr(15 downto 3),
                   axi_data_in => data_scritto,      
                   axi_data_out => data_letto_4,       
                   -- AXIS interface
                    clock => clock,
                    addr => "00000" & addr_seq(31 downto 5) ,
                    data_out => data_out_4         
                );
ram5: entity work.ram4096x32_1
      generic map (init_file => "/home/aniello/Documenti/opp-tcp_copia/scripts/python/outputfile_5.mif")
      port map (--AXI interface
             axi_clock => clock,
             we1 => we5,
             --en1 => en2,
             axi_addr => axi_addr(15 downto 3),
             axi_data_in => data_scritto,      
             axi_data_out => data_letto_5,       
             -- AXIS interface
              clock => clock,
              addr => "00000" & addr_seq(31 downto 5) ,
              data_out => data_out_5         
          );
ram6: entity work.ram4096x32_1
      generic map (init_file => "/home/aniello/Documenti/opp-tcp_copia/scripts/python/outputfile_6.mif")
      port map (--AXI interface
                   axi_clock => clock,
                   we1 => we6,
                   --en6 => en6,
                  axi_addr => axi_addr(15 downto 3),
                  axi_data_in => data_scritto,      
                  axi_data_out => data_letto_6,       
                   -- AXIS interface
                    clock => clock,
                    addr => "00000" & addr_seq(31 downto 5) ,
                    data_out => data_out_6         
                   );
ram7: entity work.ram4096x32_1
      generic map (init_file => "/home/aniello/Documenti/opp-tcp_copia/scripts/python/outputfile_7.mif")
      port map (--AXI interface
             axi_clock => clock,
             we1 => we7,
             --en1 => en2,
             axi_addr => axi_addr(15 downto 3),
             axi_data_in => data_scritto,      
             axi_data_out => data_letto_7,       
             -- AXIS interface
              clock => clock,
              addr => "00000" & addr_seq(31 downto 5) ,
              data_out => data_out_7         
          );
ram8: entity work.ram4096x32_1
      generic map (init_file => "/home/aniello/Documenti/opp-tcp_copia/scripts/python/outputfile_8.mif")
      port map (--AXI interface
                   axi_clock => clock,
                   we1 => we8,
                   --en8 => en8,
                   axi_addr => axi_addr(15 downto 3),
                   axi_data_in => data_scritto,      
                   axi_data_out => data_letto_8,       
                   -- AXIS interface
                    clock => clock,
                    addr => "00000" & addr_seq(31 downto 5) ,
                    data_out => data_out_8         
                );
end Behavioral;










