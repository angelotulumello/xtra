--        library IEEE;
--        use IEEE.STD_LOGIC_1164.ALL;
--        use ieee.std_logic_signed.all;
--        use ieee.numeric_std.all;
        
--        library work;
--        use work.salutil.all;
        
--        entity pktgen is
--          Generic (
--                       -- ETH header, 112 bits
--                       --eth_dst_mac     : std_logic_vector(47 downto 0) := x"3633159196B4";
--                       --eth_src_mac     : std_logic_vector(47 downto 0) := x"010c42363996";
--                       eth_pckt_type   : std_logic_vector(15 downto 0) := x"0800"; -- IPv4
        
--                       -- IPv4 header
--                       ip_ver_hdr_len  : std_logic_vector(7 downto 0)  := x"45";
--                       ip_diff_serv    : std_logic_vector(7 downto 0)  := x"00"; -- differentiated services
--                       ip_tot_len      : std_logic_vector(15 downto 0) := x"0434"; -- packet total len in bytes
--                       ip_identif      : std_logic_vector(15 downto 0) := x"9f1b";
--                       ip_flags        : std_logic_vector(7 downto 0)  := x"40"; -- don't fragment
--                       ip_frag_offset  : std_logic_vector(7 downto 0)  := x"00";
--                       ip_ttl          : std_logic_vector(7 downto 0)  := x"34"; -- time to live
--                       ip_proto        : std_logic_vector(7 downto 0)  := x"06"; -- TCP protocol
--                       ip_checksum     : std_logic_vector(15 downto 0) :=  x"709f";
--                       ip_src          : std_logic_vector(31 downto 0) := x"4f062e26";
--                       ip_dst          : std_logic_vector(31 downto 0) := x"0a637ca9";
        
--                       -- void 32 bits for sequence number
--                       tcp_ack_num     : std_logic_vector(31 downto 0) := x"00000001";
--                       -- void 16 bits for length + tcp_flags
--                       tcp_win_size    : std_logic_vector(15 downto 0) := x"faf0";
--                       tcp_checksum    : std_logic_vector(15 downto 0) := x"7e83";
--                       tcp_urgent_ptr  : std_logic_vector(15 downto 0) := x"0000";
--                       -- 20 bytes of data
        
--                       packet_len      : std_logic_vector(15 downto 0) := x"0442" -- total packet len in bytes
        
--                   );
        
--          Port (
        
--                    clock          : in std_logic;
--                    resetn         : in std_logic;
        
--                    --S0_AXIS_ACLK    : in std_logic;
--                    --S0_AXIS_ARESETN : in std_logic;
--                    S0_AXIS_TVALID  : in std_logic;
--                    S0_AXIS_TDATA   : in std_logic_vector(255 downto 0);
--                    S0_AXIS_TKEEP   : in std_logic_vector(31 downto 0);
--                    S0_AXIS_TUSER   : in std_logic_vector(127 downto 0);
--                    S0_AXIS_TLAST   : in std_logic;
--                    S0_AXIS_TREADY  : out std_logic;
        
--                    -- NF datapath AXI-S
--                    m_axis_tvalid  : out std_logic;
--                    m_axis_tdata   : out std_logic_vector(255 downto 0);
--                    m_axis_tkeep    : out std_logic_vector(31 downto 0);
--                    m_axis_tready  : in std_logic;
--                    m_axis_tlast   : out std_logic;
--                    m_axis_tuser   : out std_logic_vector(127 downto 0);
        
--        ----         -- RAM
--               --clock_ram : in std_logic;
--               we : in std_logic;

              
--               axi_addr : in std_logic_vector (31 downto 0);
--               data_scritto : in std_logic_vector (31 downto 0);      
--               data_letto : out std_logic_vector(31 downto 0);
        
               
--               eth_dst_mac_in : in std_logic_vector (47 downto 0);
--               eth_src_mac_in : in std_logic_vector (47 downto 0);
--               eth_dst_mac_out : out std_logic_vector (47 downto 0);
--               eth_src_mac_out : out std_logic_vector (47 downto 0);
--               buff_dim_in : in std_logic_vector (31 downto 0);
--               buff_dim_out : out std_logic_vector (31 downto 0)
--        );
        
--        end pktgen;
        
--        architecture Behavioral of pktgen is
        
--          type State_type is (end_file,reset_state, waiting, last_sending_1, last_sending_2, last_sending_3, sending_1, sending_2, sending_3, sending_4,sending_5,first_sending_1,first_sending_2,first_sending_3,first_sending_4,first_sending_5,first_sending_6 );  -- Define the states
--          signal state : State_Type;
--          signal m_axis_tuser_s : std_logic_vector(127 downto 0) := x"000000000000000000000000" & x"00" & x"00" & x"0442";
--          signal ack_num_t,seq_num_t   : std_logic_vector(31 downto 0) := (others => '0');
--          signal frame_sent : std_logic_vector (4 downto 0) := (others => '0');
        
--        --TCP Header
--        signal          tcp_src_port    : std_logic_vector(15 downto 0) := x"0231";
--        signal          tcp_dst_port    : std_logic_vector(15 downto 0) := x"c61c";
--        signal buff_dim : std_logic_vector(31 downto 0) := x"00040000";   --x"00022257";
--        signal eth_dst_mac : std_logic_vector (47 downto 0):= x"3633159196B4";
--        signal eth_src_mac : std_logic_vector (47 downto 0):= x"010c42363996";
        
        
--          -- Control signals from OPP
--          signal     seq_num          : std_logic_vector(31 downto 0);
--          signal     timestamp        : std_logic_vector(31 downto 0);
--          signal     send             : std_logic;
--          signal     ready            : std_logic;
--          signal     second_packet,first_packet     : std_logic;
        
--          signal addr_seq : std_logic_vector(31 downto 0);
--          signal seq_num_init: std_logic_vector(31 downto 0):=(others=>'0');
        
--          -- SEGNALI PER PROVARE LA RAM
--          signal  axi_clock : std_logic;
--         -- signal  we: std_logic;
--          --signal axi_addr :  std_logic_vector(7 downto 0);
--          signal axi_data_in : std_logic_vector(31 downto 0);
--          signal axi_data_out :std_logic_vector(31 downto 0);
        
--        signal we1, we2, we3, we4, we5, we6, we7, we8: std_logic;
        
        
--          -- AXIS interface
--           signal  addr  : std_logic_vector(31 downto 0);
--           signal data_out_1 :std_logic_vector (31 downto 0);
--           signal data_out_2 :std_logic_vector (31 downto 0);
--           signal data_out_3 :std_logic_vector (31 downto 0);
--           signal data_out_4 :std_logic_vector (31 downto 0);
--           signal data_out_5 :std_logic_vector (31 downto 0);
--           signal data_out_6 :std_logic_vector (31 downto 0);
--           signal data_out_7 :std_logic_vector (31 downto 0);
--           signal data_out_8 :std_logic_vector (31 downto 0);
--           signal data_out_8_temp :std_logic_vector (15 downto 0);
        
--           signal data_letto_1 :  std_logic_vector (31 downto 0);
--           signal data_letto_2 :  std_logic_vector (31 downto 0);
--           signal data_letto_3 :  std_logic_vector (31 downto 0);
--           signal data_letto_4 :  std_logic_vector (31 downto 0);
--           signal data_letto_5 :  std_logic_vector (31 downto 0);
--           signal data_letto_6 :  std_logic_vector (31 downto 0);
--           signal data_letto_7 :  std_logic_vector (31 downto 0);
--           signal data_letto_8 :  std_logic_vector (31 downto 0);
        
--          function ntoh32 (I: std_logic_vector(31 downto 0)) return std_logic_vector is
--          begin
--               return (I(7 downto 0) & I(15 downto 8) & I(23 downto 16) & I(31 downto 24));
--          end function ntoh32;
        
--          function ntoh16 (I: std_logic_vector(15 downto 0)) return std_logic_vector is
--          begin
--               return (I(7 downto 0) & I(15 downto 8));
--          end function ntoh16;
        
        
--          function compute_tcp(A: std_logic_vector(143 downto 0)) return  std_logic_vector is
--              variable temp1 : std_logic_vector(31 downto 0):=x"00000000";
--              variable temp2 : std_logic_vector(16 downto 0);
--              variable result : std_logic_vector(15 downto 0);
--              variable i: integer:=0;
--              begin
--                  --for i in (A'length/16-1) downto 0 loop
--                  for i in 8 downto 0 loop
--                      temp1:= temp1 + (x"0000" & A(16*i+15 downto 16*i));
--                  end loop;
--                  temp2  := ('0' & temp1(31 downto 16)) + ('0' & temp1(15 downto 0));
--                  if temp2(15 downto 0)=x"ffff" then
--                     temp2(15 downto 0):=x"0000";
--                  end if;
--                  result := temp2(16) + temp2(15 downto 0);
--                  if (result /=x"0000") then
--                    result := not(result);
--                  end if;
--                  return result;
--              end function compute_tcp;
        
        
--        --    attribute mark_debug: string;
--        --    attribute keep:string;
        
--        --    attribute mark_debug of state:signal is "TRUE";
--        --    attribute keep of state: signal is "TRUE";
        
--        --    attribute mark_debug of seq_num_t:signal is "TRUE";
--        --    attribute keep of seq_num_t: signal is "TRUE";
--        --    attribute mark_debug of timestamp:signal is "TRUE";
--        --    attribute keep of timestamp: signal is "TRUE";
        
--        --    attribute max_fanout:integer;
--        --    attribute max_fanout of new_tcp_checksum: signal is 8;
        
--        begin
        
        
--          -- RAM
        
--          --seq_num_t (7 downto 0) <= seq_num_ram;
        
--          send <= S0_AXIS_TVALID;
--          first_packet <=  S0_AXIS_TDATA(64) and S0_AXIS_TVALID;
--          second_packet <= S0_AXIS_TDATA(65) and S0_AXIS_TVALID;
--          S0_AXIS_TREADY <= ready;
        
--          --eth_dst_mac_t <= eth_dst_mac_in;
--          --eth_src_mac_t <= eth_src_mac_in;
        
--         eth_dst_mac_out <= eth_dst_mac;
--         eth_src_mac_out <= eth_src_mac;
        
        
--          SEND_PCKT_FSM: process(clock)
--          begin
--               if rising_edge (clock) then
--                   if (resetn = '0') then
--                       state <= reset_state;
--                       frame_sent <= (others => '0');
--                       seq_num_t  <= (others => '0');
--                       timestamp <= (others => '0');
--                   else
--                       case state is
--                           when reset_state =>     
--                                state <= waiting;
        
--                           when waiting =>                                                          
--                               frame_sent <= (others => '0');
--                               if (send = '1') then
--                                   --TCP Header
--                                   tcp_src_port<=S0_AXIS_TDATA(111 downto 96);
--                                   tcp_dst_port<=S0_AXIS_TDATA(95 downto 80);
                                           
--                                   if (first_packet='1') then
--                                           seq_num_init <= S0_AXIS_TDATA(31 downto 0);
--                                           state <= first_sending_1;
--                                   end if;
--                                   if (second_packet='1') then
--                                        ack_num_t <= 1+S0_AXIS_TDATA(31 downto 0);
--                                        state <= first_sending_4;
--                                    end if;
--                                    if (second_packet='0' and first_packet='0') then
--                                           state <= sending_1;
--                                           seq_num_t <= S0_AXIS_TDATA(31 downto 0);
--                                           timestamp <= S0_AXIS_TDATA(63 downto 32);
--                                     end if;
--                                     if (addr_seq > buff_dim) then 
--                                        state <= last_sending_1; 
--                                        seq_num_t <= S0_AXIS_TDATA(31 downto 0);
--                                        timestamp <= S0_AXIS_TDATA(63 downto 32);
--                                      end if;
--                               end if;
        
--                           when sending_1 =>
--                               if (m_axis_tready = '1') then
--                                   state <= sending_2;
--                               end if;
--                           when sending_2 =>
--                               if (m_axis_tready = '1') then
--                                   data_out_8_temp <= data_out_8(31 downto 16);
--                                   state <= sending_3;
--                                   seq_num_t <= seq_num_t + 32;
--                                   --seq_num_t <= seq_num_t + 8;
--                               end if;
--                           when sending_3 =>
--                               if (m_axis_tready = '1') then
--                                   data_out_8_temp <= data_out_8(31 downto 16);
--                                   state <= sending_4;
--                                   seq_num_t <= seq_num_t + 32;
--                                   --seq_num_t <= seq_num_t + 8;
--                               end if;
        
--                           when sending_4 =>
--                               if (frame_sent /= "11110") and (m_axis_tready = '1') then
--                                   data_out_8_temp <= data_out_8(31 downto 16);
--                                   seq_num_t <= seq_num_t + 32;
--                                   --seq_num_t <= seq_num_t + 8;
--                                   frame_sent <= frame_sent + 1;
--                               elsif (frame_sent = "11110") and (m_axis_tready = '1') then
--                                   state <= sending_5;
--                               end if;
--                           when sending_5 =>
--                               if (m_axis_tready = '1') then
--                                   state <= waiting;
--                               end if;
        
--                           when first_sending_1 =>
--                               if (m_axis_tready = '1') then
--                                   state <= first_sending_2;
--                               end if;   
--                           when first_sending_2 =>
--                               if (m_axis_tready = '1')  then
--                                   state <= first_sending_3;
--                               end if;   
--                           when first_sending_3 =>
--                               if (m_axis_tready = '1')  then
--                                   state <= waiting;
--                               end if;   
        
--                           when first_sending_4 =>
--                               if (m_axis_tready = '1') then
--                                   state <= first_sending_5;
--                               end if;   
--                           when first_sending_5 =>
--                               if (m_axis_tready = '1') then
--                                   state <= first_sending_6;
--                               end if;   
--                           when first_sending_6 =>
--                               if (m_axis_tready = '1') then
--                                   state <= waiting;
--                               end if;  
                               
                               
                               
--                    when last_sending_1 =>
--                             if (m_axis_tready = '1') then
--                                 state <= last_sending_2;
--                             end if;   
--                         when last_sending_2 =>
--                             if (m_axis_tready = '1') then
--                                 state <= last_sending_3;
--                             end if;   
--                         when last_sending_3 =>
--                             if (m_axis_tready = '1') then
--                                 state <= end_file;
--                             end if;    
--                     when end_file =>
--                             null;         
--                           when others =>
--                               state <= reset_state;
--                       end case;
--                   end if;
--               end if;
--          end process;
        
        
        
--          SEND_PCKT: process(data_out_1,data_out_2,data_out_3,data_out_4,data_out_5,data_out_6,data_out_7,data_out_8_temp,data_out_8,state,timestamp,seq_num_t,ack_num_t,tcp_src_port, tcp_dst_port)
--          begin
--               ready <= '0';
--               m_axis_tvalid <= '0';
--               m_axis_tdata <= (others => '0');
--               m_axis_tlast <= '0';
--               m_axis_tuser <= (others => '0');
--               m_axis_tkeep <= (others => '0');
        
--               case state is
--                   when reset_state =>     
--                       ready <= '1';
--                       m_axis_tvalid <= '0';
--                       m_axis_tdata <= (others => '0');
--                       m_axis_tlast <= '0';
--                       m_axis_tuser <= (others => '0');
--                       m_axis_tkeep <= (others => '0');
        
--                   when waiting =>
--                       ready <= '1';
--                       m_axis_tvalid <= '0';
--                       m_axis_tdata <= (others => '0');
--                       m_axis_tlast <= '0';
--                       m_axis_tuser <= (others => '0');
--                       m_axis_tkeep <= (others => '0');
        
--                   when sending_1 =>
        
--                       ready <= '0';                                --ip_checksum
--                       m_axis_tvalid <= '1';
--                       m_axis_tdata <= ntoh16(ip_dst(15  downto 0)) & ntoh32(ip_src) & ntoh16(x"9f70" )  & ip_proto & ip_ttl  & ip_frag_offset & ip_flags & ntoh16(ip_identif) & ntoh16(ip_tot_len) & ip_diff_serv & ip_ver_hdr_len & ntoh16(eth_pckt_type) & eth_src_mac & eth_dst_mac;--eth_src_mac--eth_dst_mac
--                       m_axis_tkeep <= (others => '1');
--                       m_axis_tlast <= '0';
--                       m_axis_tuser <= x"000000000000000000000000" & x"00" & x"00" & x"0442";
        
--                   when sending_2 =>
        
--                       ready <= '0';
--                       m_axis_tvalid <= '1';
--                       m_axis_tdata <=  x"00000000" & ntoh32(timestamp) & x"0A08" & ntoh16(tcp_urgent_ptr) & x"0000" & ntoh16(tcp_win_size) & x"1080" & ntoh32(ack_num_t) & ntoh32(seq_num_t) & ntoh16(tcp_dst_port) & ntoh16(tcp_src_port) & ntoh16(ip_dst(31 downto 16));
--                       m_axis_tkeep <= (others => '1');
--                       m_axis_tlast <= '0';
--                       m_axis_tuser <= (others => '0');
        
--                   when sending_3 =>
        
--                       ready <= '0';
--                       m_axis_tvalid <= '1';
--                       m_axis_tdata <= data_out_8(15 downto 0) & data_out_7 & data_out_6 & data_out_5 & data_out_4 & data_out_3 & data_out_2 & data_out_1 & x"0101"; --data_out_1 & data_out_2 & data_out_3 & data_out_4 & data_out_5 & data_out_6 & data_out_7 & data_out_8(31 downto 16) & x"0101";--(others=>'0');--data_out_1 & data_out_2 & data_out_3 & data_out_4 & data_out_5 & data_out_6 & data_out_7 & data_out_8;--& x"00000000" & x"00000000" & x"00000000" & x"00000000" & x"00000000" & x"00000000" & x"00000000";
--                       --m_axis_tdata <= seq_num_t & x"01234567" & seq_num_t & x"12345678" & x"00000000000000000000000000000000";
--                       m_axis_tkeep <= (others => '1');
--                       m_axis_tlast <= '0';
--                       m_axis_tuser <= (others => '0');
        
--                   when sending_4 =>
        
--                       ready <= '0';
--                       m_axis_tvalid <= '1';
--                       m_axis_tdata <=  data_out_8(15 downto 0)  & data_out_7 & data_out_6 & data_out_5 & data_out_4 & data_out_3 & data_out_2 & data_out_1 & data_out_8_temp ;--data_out_8_temp  & data_out_1 & data_out_2 & data_out_3 & data_out_4 & data_out_5 & data_out_6 & data_out_7 & data_out_8(31 downto 16); -- & x"00000000" & x"00000000" & x"00000000" & x"00000000" & x"00000000" & x"00000000" & x"00000000";
--                       m_axis_tkeep <= (others => '1');
--                       m_axis_tlast <= '0';
--                       m_axis_tuser <= (others => '0');
        
--                   when sending_5 =>
--                       ready <= '0';
--                       m_axis_tvalid <= '1';
--                       m_axis_tdata(255 downto 240) <= data_out_8_temp;
--                       m_axis_tdata(239 downto 0) <= (others=>'0');
--                       m_axis_tuser <= (others => '0');
--                       m_axis_tkeep<=x"00000003";
--                       m_axis_tlast <= '1';
        
--                   when first_sending_1 =>
        
--                       ready <= '0';
--                       m_axis_tvalid <= '1';
--                       m_axis_tdata <= ntoh16(ip_dst(15  downto 0)) & ntoh32(ip_src) & ntoh16(x"a370")  & ip_proto & ip_ttl & ip_frag_offset & ip_flags & ntoh16(ip_identif) & ntoh16(x"0034") & ip_diff_serv & ip_ver_hdr_len & ntoh16(eth_pckt_type) & eth_src_mac & eth_dst_mac;
--                       m_axis_tkeep <= (others => '1');
--                       m_axis_tlast <= '0';
--                       m_axis_tuser <= x"000000000000000000000000" & x"00" & x"00" & x"0042";
        
--                   when first_sending_2 =>
        
--                       ready <= '0';
--                       m_axis_tvalid <= '1';
--                       m_axis_tdata <=  x"00000000" & ntoh32(timestamp) & x"0A08" & ntoh16(tcp_urgent_ptr) & x"0000" & ntoh16(tcp_win_size) & x"0280" & x"00000000" & x"00000000" & ntoh16(tcp_dst_port) & ntoh16(tcp_src_port) & ntoh16(ip_dst(31 downto 16));
--                       m_axis_tkeep <= (others => '1');
--                       m_axis_tlast <= '0';
--                       m_axis_tuser <= (others => '0');
        
--                   when first_sending_3 =>
        
--                       ready <= '0';
--                       m_axis_tvalid <= '1';
--                       m_axis_tdata <= (others => '0');
--                       m_axis_tkeep <= x"00000003";
--                       m_axis_tlast <= '1';
--                       m_axis_tuser <= (others => '0');
        
--                  --second packet
--                   when first_sending_4 =>
        
--                       ready <= '0';
--                       m_axis_tvalid <= '1';
--                       m_axis_tdata <= ntoh16(ip_dst(15  downto 0)) & ntoh32(ip_src) & ntoh16(x"a370")  & ip_proto & ip_ttl & ip_frag_offset & ip_flags & ntoh16(ip_identif) & ntoh16(x"0034") & ip_diff_serv & ip_ver_hdr_len & ntoh16(eth_pckt_type) & eth_src_mac & eth_dst_mac;
--                       m_axis_tkeep <= (others => '1');
--                       m_axis_tlast <= '0';
--                       m_axis_tuser <= x"000000000000000000000000" & x"00" & x"00" & x"0042";
        
--                   when first_sending_5 =>
        
--                       ready <= '0';
--                       m_axis_tvalid <= '1';
--                       m_axis_tdata <=  x"00000000" & ntoh32(timestamp) & x"0A08" & ntoh16(tcp_urgent_ptr) & x"0000" & ntoh16(tcp_win_size) & x"1080" & ntoh32(ack_num_t) & x"01000000" & ntoh16(tcp_dst_port) & ntoh16(tcp_src_port) & ntoh16(ip_dst(31 downto 16));
--                       m_axis_tkeep <= (others => '1');
--                       m_axis_tlast <= '0';
--                       m_axis_tuser <= (others => '0');
        
--                   when first_sending_6 =>
        
--                       ready <= '0';
--                       m_axis_tvalid <= '1';
--                       m_axis_tdata <= (others => '0');
--                       m_axis_tkeep <= x"00000003";
--                       m_axis_tlast <= '1';
--                       m_axis_tuser <= (others => '0');
        
--                  --last packet
--                        when last_sending_1 =>
             
--                            ready <= '0';
--                            m_axis_tvalid <= '1';
--                            m_axis_tdata <= ntoh16(ip_dst(15  downto 0)) & ntoh32(ip_src) & ntoh16(x"a370")  & ip_proto & ip_ttl & ip_frag_offset & ip_flags & ntoh16(ip_identif) & ntoh16(x"0034") & ip_diff_serv & ip_ver_hdr_len & ntoh16(eth_pckt_type) & eth_src_mac & eth_dst_mac;
--                            m_axis_tkeep <= (others => '1');
--                            m_axis_tlast <= '0';
--                            m_axis_tuser <= x"000000000000000000000000" & x"00" & x"00" & x"0042";
             
--                        when last_sending_2 =>
             
--                            ready <= '0';
--                            m_axis_tvalid <= '1';
--                            m_axis_tdata <=  x"00000000" & ntoh32(timestamp) & x"0A08" & ntoh16(tcp_urgent_ptr) & x"0000" & ntoh16(tcp_win_size) & x"0180" & ntoh32(ack_num_t) & ntoh32(seq_num_t) & ntoh16(tcp_dst_port) & ntoh16(tcp_src_port) & ntoh16(ip_dst(31 downto 16));
--                            m_axis_tkeep <= (others => '1');
--                            m_axis_tlast <= '0';
--                            m_axis_tuser <= (others => '0');
             
--                        when last_sending_3 =>
             
--                            ready <= '0';
--                            m_axis_tvalid <= '1';
--                            m_axis_tdata <= (others => '0');
--                            m_axis_tkeep <= x"00000003";
--                            m_axis_tlast <= '1';
--                            m_axis_tuser <= (others => '0');
                            
--                         when others =>
--                       ready <= '0';
--                       m_axis_tvalid <= '0';
--                       m_axis_tdata <= (others => '0');
--                       m_axis_tkeep <= (others => '1');
--                       m_axis_tlast <= '0';
--                       m_axis_tuser <= (others => '0');
--               end case;
        
        
        
        
        
        
        
        
--          end process;
      
--        we1 <= we when axi_addr(2 downto 0) = "000" else '0'; 
--        we2 <= we when axi_addr(2 downto 0) = "001" else '0';
--        we3 <= we when axi_addr(2 downto 0) = "010" else '0'; 
--        we4 <= we when axi_addr(2 downto 0) = "011" else '0';
--        we5 <= we when axi_addr(2 downto 0) = "100" else '0'; 
--        we6 <= we when axi_addr(2 downto 0) = "101" else '0';
--        we7 <= we when axi_addr(2 downto 0) = "110" else '0'; 
--        we8 <= we when axi_addr(2 downto 0) = "111" else '0';
        
--        data_letto <= data_letto_1 when axi_addr(2 downto 0) = "000" else 
--                      data_letto_2 when axi_addr(2 downto 0) = "001" else
--                      data_letto_3 when axi_addr(2 downto 0) = "010" else
--                      data_letto_4 when axi_addr(2 downto 0) = "011" else
--                      data_letto_5 when axi_addr(2 downto 0) = "100" else
--                      data_letto_6 when axi_addr(2 downto 0) = "101" else
--                      data_letto_7 when axi_addr(2 downto 0) = "110" else
--                      data_letto_8 when axi_addr(2 downto 0) = "111" else 
--                      x"00000000";
                     
--        --ciclo_ram:
--        --   for i in 0 to 7 generate
--        --      begin
--        --ciclol_ram1: entity work.ram4096x32_1
--        --        generic map (init_file => "/home/aniellocamma/Documenti/opp-tcp/opp-tcp.srcs/sources/init_ram_1.mif")
--        --        port map (--AXI interface
--        --             axi_clock => clock,
--        --             we1 => we(i),
--        --             --en1 => en1,
--        --             axi_addr => axi_addr(15 downto 3),
--        --             axi_data_in => data_scritto,
--        --             axi_data_out => data_letto(i),
--        --             -- AXIS interface
--        --             clock => clock,
--        --              addr => seq_num_t,
--        --              data_out => data_out(i)         
--        --          );
--        --   end generate;
        
        
        
        
--        addr_seq <=seq_num_t-seq_num_init;
--        buff_dim_out <= buff_dim;
--        buff_dim <= buff_dim_in;
        
--        ram1: entity work.ram4096x32_1
--                generic map (init_file => "/home/aniello/Documenti/opp-tcp/scripts/python/outputfile_1.mif")
--                port map (--AXI interface
--                             axi_clock => clock,
--                             we1 => we1,
--                             --en1 => en1,
--                             axi_addr => axi_addr(15 downto 3),
--                             axi_data_in => data_scritto,
--                             axi_data_out => data_letto_1,
--                             -- AXIS interface
--                              clock => clock,
--                              addr => "00000" & addr_seq(31 downto 5) ,
--                              data_out => data_out_1         
--                          );
        
--        ram2: entity work.ram4096x32_1
--                generic map (init_file => "/home/aniello/Documenti/opp-tcp/scripts/python/outputfile_2.mif")
--                port map (--AXI interface
--                             axi_clock => clock,
--                             we1 => we2,
--                             --en1 => en2,
--                             axi_addr => axi_addr(15 downto 3),
--                             axi_data_in => data_scritto,
--                             axi_data_out => data_letto_2,
--                             -- AXIS interface
--                              clock => clock,
--                              addr => "00000" & addr_seq(31 downto 5) ,
--                              data_out => data_out_2         
--                          );
--        ram3: entity work.ram4096x32_1
--                        generic map (init_file => "/home/aniello/Documenti/opp-tcp/scripts/python/outputfile_3.mif")
--                         port map (--AXI interface
--                     axi_clock => clock,
--                     we1 => we3,
--                     --en1 => en2,
--                     axi_addr => axi_addr(15 downto 3),
--                     axi_data_in => data_scritto,
--                     axi_data_out => data_letto_3, 
--                     -- AXIS interface
--                      clock => clock,
--                      addr => "00000" & addr_seq(31 downto 5),
--                      data_out => data_out_3         
--                  );
--        ram4: entity work.ram4096x32_1
--               generic map (init_file => "/home/aniello/Documenti/opp-tcp/scripts/python/outputfile_4.mif")
--               port map (--AXI interface
--                            axi_clock => clock,
--                            we1 => we4,
--                           -- en4 => en4,
--                            axi_addr => axi_addr(15 downto 3),
--                            axi_data_in => data_scritto,      
--                            axi_data_out => data_letto_4,       
--                            -- AXIS interface
--                             clock => clock,
--                             addr => "00000" & addr_seq(31 downto 5) ,
--                             data_out => data_out_4         
--                         );
--        ram5: entity work.ram4096x32_1
--                generic map (init_file => "/home/aniello/Documenti/opp-tcp/scripts/python/outputfile_5.mif")
--                port map (--AXI interface
--                       axi_clock => clock,
--                       we1 => we5,
--                       --en1 => en2,
--                       axi_addr => axi_addr(15 downto 3),
--                       axi_data_in => data_scritto,      
--                       axi_data_out => data_letto_5,       
--                       -- AXIS interface
--                        clock => clock,
--                        addr => "00000" & addr_seq(31 downto 5) ,
--                        data_out => data_out_5         
--                    );
--        ram6: entity work.ram4096x32_1
--               generic map (init_file => "/home/aniello/Documenti/opp-tcp/scripts/python/outputfile_6.mif")
--               port map (--AXI interface
--                            axi_clock => clock,
--                            we1 => we6,
--                            --en6 => en6,
--                           axi_addr => axi_addr(15 downto 3),
--                           axi_data_in => data_scritto,      
--                           axi_data_out => data_letto_6,       
--                            -- AXIS interface
--                             clock => clock,
--                             addr => "00000" & addr_seq(31 downto 5) ,
--                             data_out => data_out_6         
--                                          );
--        ram7: entity work.ram4096x32_1
--                generic map (init_file => "/home/aniello/Documenti/opp-tcp/scripts/python/outputfile_7.mif")
--                port map (--AXI interface
--                       axi_clock => clock,
--                       we1 => we7,
--                       --en1 => en2,
--                       axi_addr => axi_addr(15 downto 3),
--                       axi_data_in => data_scritto,      
--                       axi_data_out => data_letto_7,       
--                       -- AXIS interface
--                        clock => clock,
--                        addr => "00000" & addr_seq(31 downto 5) ,
--                        data_out => data_out_7         
--                    );
--          ram8: entity work.ram4096x32_1
--                generic map (init_file => "/home/aniello/Documenti/opp-tcp/scripts/python/outputfile_8.mif")
--                port map (--AXI interface
--                             axi_clock => clock,
--                             we1 => we8,
--                             --en8 => en8,
--                             axi_addr => axi_addr(15 downto 3),
--                             axi_data_in => data_scritto,      
--                             axi_data_out => data_letto_8,       
--                             -- AXIS interface
--                              clock => clock,
--                              addr => "00000" & addr_seq(31 downto 5) ,
--                              data_out => data_out_8         
--                          );
                          
                
         
            
        
--        end Behavioral;










library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.std_logic_signed.all;
use ieee.numeric_std.all;

library work;
use work.salutil.all;

entity pktgen is
  Generic (-- ETH header, 112 bits
           --eth_dst_mac     : std_logic_vector(47 downto 0) := x"3633159196B4";
           --eth_src_mac     : std_logic_vector(47 downto 0) := x"010c42363996";
           eth_pckt_type   : std_logic_vector(15 downto 0) := x"0800"; -- IPv4
           -- IPv4 header
           ip_ver_hdr_len  : std_logic_vector(7 downto 0)  := x"45";
           ip_diff_serv    : std_logic_vector(7 downto 0)  := x"00"; -- differentiated services
           --ip_tot_len      : std_logic_vector(15 downto 0) := x"0434"; -- packet total len in bytes
           ip_identif      : std_logic_vector(15 downto 0) := x"9f1b";
           ip_flags        : std_logic_vector(7 downto 0)  := x"40"; -- don't fragment
           ip_frag_offset  : std_logic_vector(7 downto 0)  := x"00";
           ip_ttl          : std_logic_vector(7 downto 0)  := x"34"; -- time to live
           ip_proto        : std_logic_vector(7 downto 0)  := x"06"; -- TCP protocol
          -- ip_checksum     : std_logic_vector(15 downto 0) :=  x"709f";
           --ip_src          : std_logic_vector(31 downto 0) := x"4f062e26";
           --ip_dst          : std_logic_vector(31 downto 0) := x"0a637ca9";
           -- void 32 bits for sequence number
           tcp_ack_num     : std_logic_vector(31 downto 0) := x"00000001";
           -- void 16 bits for length + tcp_flags
           tcp_win_size    : std_logic_vector(15 downto 0) := x"faf0";
           tcp_checksum    : std_logic_vector(15 downto 0) := x"7e83";
           tcp_urgent_ptr  : std_logic_vector(15 downto 0) := x"0101";--x"0000";
           -- 20 bytes of data
           packet_len      : std_logic_vector(15 downto 0) := x"0442" -- total packet len in bytes
           );
           
      Port (clock          : in std_logic;
            resetn         : in std_logic;
            S0_AXIS_TVALID  : in std_logic;
            S0_AXIS_TDATA   : in std_logic_vector(255 downto 0);
            S0_AXIS_TKEEP   : in std_logic_vector(31 downto 0);
            S0_AXIS_TUSER   : in std_logic_vector(127 downto 0);
            S0_AXIS_TLAST   : in std_logic;
            S0_AXIS_TREADY  : out std_logic;
            -- NF datapath AXI-S
            m_axis_tvalid  : out std_logic;
            m_axis_tdata   : out std_logic_vector(255 downto 0);
            m_axis_tkeep    : out std_logic_vector(31 downto 0);
            m_axis_tready  : in std_logic;
            m_axis_tlast   : out std_logic;
            m_axis_tuser   : out std_logic_vector(127 downto 0);
            -- RAM
            we : in std_logic;
            axi_addr : in std_logic_vector (31 downto 0);
            data_scritto : in std_logic_vector (31 downto 0);      
            data_letto : out std_logic_vector(31 downto 0)
--            eth_dst_mac_in : in std_logic_vector (47 downto 0);
--            eth_src_mac_in : in std_logic_vector (47 downto 0);
--            eth_src_mac_out : out std_logic_vector (47 downto 0);
--            eth_dst_mac_out : out std_logic_vector (47 downto 0)
            );
            end pktgen;

architecture Behavioral of pktgen is
  type State_type is (end_file, reset_state, waiting, last_sending_1, last_sending_2, last_sending_3, sending_1,sending_1_plus,sending_2_plus, sending_2, sending_3,
                      sending_4,sending_5,first_sending_1,first_sending_2,first_sending_3,first_sending_4,first_sending_5,first_sending_6);
                      -- Define the states
  signal state : State_Type;
  signal m_axis_tuser_s : std_logic_vector(127 downto 0) := x"000000000000000000000000" & x"00" & x"00" & x"0442";
  signal ack_num_t,seq_num_t   : std_logic_vector(31 downto 0) := (others => '0');
  signal frame_sent : std_logic_vector (4 downto 0) := (others => '0');
  --TCP Header
  signal          tcp_src_port    : std_logic_vector(15 downto 0) := x"0231";
  signal          tcp_dst_port    : std_logic_vector(15 downto 0) := x"c61c";
  signal eth_dst_mac : std_logic_vector (47 downto 0):= x"3633159196B4";
  signal eth_src_mac : std_logic_vector (47 downto 0):= x"010c42363996";
  signal ip_src          : std_logic_vector(31 downto 0) := x"4f062e26";
  signal ip_dst          : std_logic_vector(31 downto 0) := x"0a637ca9";
  signal         ip_tot_len   : std_logic_vector(15 downto 0) := x"0434";
  signal         ip_chksum    : std_logic_vector(15 downto 0) := x"0000";

  -- Control signals from OPP
  signal     seq_num          : std_logic_vector(31 downto 0);
  signal     timestamp        : std_logic_vector(31 downto 0):=(others=>'0');
   signal     ts_echo       : std_logic_vector(31 downto 0):=(others=>'0');
  signal     send             : std_logic;
  signal     ready            : std_logic;
  signal     second_packet,first_packet     : std_logic;
  -- Control signals from RAMs
  signal addr_seq : std_logic_vector(31 downto 0);
  signal seq_num_init: std_logic_vector(31 downto 0):=(others=>'0');
  signal axi_clock : std_logic;
  signal axi_data_in : std_logic_vector(31 downto 0);
  signal axi_data_out :std_logic_vector(31 downto 0);
  signal we1, we2, we3, we4, we5, we6, we7, we8: std_logic := '0';
  -- Contro signal size file in RAMs
  signal buff_dim : std_logic_vector(31 downto 0):=x"00040000";   --x"00022257";
  -- AXIS interface
   signal count_p  : std_logic_vector(31 downto 0);
    signal count_p_1  : std_logic_vector(31 downto 0);
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
  
  signal flag_xtra: std_logic_vector(31 downto 0);
  signal flag_xtra_2: std_logic_vector(31 downto 0);
  signal flag_no_payload:std_Logic;
  signal ip_checksum     : std_logic_vector(15 downto 0) :=  x"709f";

  function ntoh32 (I: std_logic_vector(31 downto 0)) return std_logic_vector is
  begin
  return (I(7 downto 0) & I(15 downto 8) & I(23 downto 16) & I(31 downto 24));
  end function ntoh32;

  function ntoh16 (I: std_logic_vector(15 downto 0)) return std_logic_vector is
  begin
  return (I(7 downto 0) & I(15 downto 8));
  end function ntoh16;
  
  function compute_tcp(A: std_logic_vector(143 downto 0)) return  std_logic_vector is
  variable temp1 : std_logic_vector(31 downto 0):=x"00000000";
  variable temp2 : std_logic_vector(16 downto 0);
  variable result : std_logic_vector(15 downto 0);
  variable i: integer:=0;
  begin
  --for i in (A'length/16-1) downto 0 loop
  for i in 8 downto 0 loop
      temp1:= temp1 + (x"0000" & A(16*i+15 downto 16*i));
  end loop;
  temp2  := ('0' & temp1(31 downto 16)) + ('0' & temp1(15 downto 0));
  if temp2(15 downto 0)=x"ffff" then
     temp2(15 downto 0):=x"0000";
  end if;
  result := temp2(16) + temp2(15 downto 0);
  if (result /=x"0000") then
    result := not(result);
  end if;
  return result;
  end function compute_tcp;


--    attribute mark_debug: string;
--    attribute keep:string;

--    attribute mark_debug of state:signal is "TRUE";
--    attribute keep of state: signal is "TRUE";

--    attribute mark_debug of seq_num_t:signal is "TRUE";
--    attribute keep of seq_num_t: signal is "TRUE";
--    attribute mark_debug of timestamp:signal is "TRUE";
--    attribute keep of timestamp: signal is "TRUE";

--    attribute max_fanout:integer;
--    attribute max_fanout of new_tcp_checksum: signal is 8;

begin
send <= S0_AXIS_TVALID;
first_packet <=  S0_AXIS_TDATA(64) and S0_AXIS_TVALID;
second_packet <= S0_AXIS_TDATA(65) and S0_AXIS_TVALID;
S0_AXIS_TREADY <= ready;
flag_no_payload<=S0_AXIS_TDATA(66);

SEND_PCKT_FSM: process(clock)
begin
if rising_edge (clock) then
                       if (resetn = '0') then
                       state <= reset_state;
                       frame_sent <= (others => '0');
                       seq_num_t  <= (others => '0');
                       timestamp <= (others => '0');
                       flag_xtra<= (others => '0');
                       flag_xtra_2<= (others => '0');
                       ack_num_t<=(others=>'0');
                       count_p<=(others=>'0');
                       count_p_1<=(others=>'0');
                       --flag_no_payload<= '0';
                       else
                           case state is
                           when reset_state => state <= waiting;
                           when waiting => frame_sent <= (others => '0');
                                           if (send = '1') then
                                           --TCP Header
                                           --tcp_src_port<=S0_AXIS_TDATA(111 downto 96);
                                           --tcp_dst_port<=S0_AXIS_TDATA(95 downto 80);
                                                if (first_packet='1') then
                                                seq_num_init <= S0_AXIS_TDATA(31 downto 0);
                                                state <= first_sending_1;
                                                end if;
                                                if (second_packet='1') then
                                                ack_num_t <= 1+S0_AXIS_TDATA(31 downto 0);
                                                state <= first_sending_4;
                                                end if;
                                                if (second_packet='0' and first_packet='0') then
                                                state <= sending_1;
                                               flag_xtra<= S0_AXIS_TDATA(31 downto 0);
                                                 seq_num_t <= S0_AXIS_TDATA(63 downto 32);
                                                 flag_xtra_2<=(others=>'0');--S0_AXIS_TDATA(98 downto 67);
                                                 ack_num_t<=S0_AXIS_TDATA(130 downto 99);
                                                timestamp <= S0_AXIS_TDATA(98 downto 67);
                                                end if;
--                                                if (addr_seq > buff_dim) then 
--                                                state <= last_sending_1; 
--                                                flag_xtra  <= S0_AXIS_TDATA(31 downto 0);
--                                                seq_num_t <= S0_AXIS_TDATA(63 downto 32);
--                                                flag_xtra_2<=S0_AXIS_TDATA(98 downto 67);
--                                                ack_num_t<=S0_AXIS_TDATA(130 downto 99);
--                                                --flag_no_payload<=S0_AXIS_TDATA(66);
--                                                --timestamp <= S0_AXIS_TDATA(63 downto 32);
--                                                end if;
                                            end if;
                            when sending_1 => if (m_axis_tready = '1') then
                                              count_p<=count_p+1;
                                              if (flag_no_payload='0') then
                                              count_p<=count_p+1;
                                              state <= sending_2;
                                              else
                                              state <=sending_1_plus;
                                              count_p_1<=count_p_1+1;
                                              end if;
                                              end if;
                            when sending_1_plus => if (m_axis_tready = '1') then
                                                       state <= sending_2_plus;
                                                   end if;                 
                            when sending_2_plus => if (m_axis_tready = '1') then
                                                 state <= waiting;
                                                 seq_num_t <= seq_num_t + 32;
                                               end if;                  
                            when sending_2 => if (m_axis_tready = '1') then
                                             
                                              data_out_8_temp <= data_out_8(31 downto 16);
                                              state <= sending_3;
                                              seq_num_t <= seq_num_t + 32;
                                              
                                               
                                               
                                              end if;
                            when sending_3 => if (m_axis_tready = '1') then
                                              data_out_8_temp <= data_out_8(31 downto 16);
                                              state <= sending_4;
                                              seq_num_t <= seq_num_t + 32;
                                              end if;
                            when sending_4 => if (frame_sent /= "11110") and (m_axis_tready = '1') then
                                              data_out_8_temp <= data_out_8(31 downto 16);
                                              seq_num_t <= seq_num_t + 32;
                                              frame_sent <= frame_sent + 1;
                                              elsif (frame_sent = "11110") and (m_axis_tready = '1') then
                                              state <= sending_5;
                                              end if;
                            when sending_5 => if (m_axis_tready = '1') then
                                              state <= waiting;
                                              end if;
                            when first_sending_1 => if (m_axis_tready = '1') then
                                                    state <= first_sending_2;
                                                    end if;   
                            when first_sending_2 => if (m_axis_tready = '1')  then
                                                    state <= first_sending_3;
                                                    end if;   
                            when first_sending_3 => if (m_axis_tready = '1')  then
                                                    state <= waiting;
                                                    end if;
                            when first_sending_4 => if (m_axis_tready = '1') then
                                                    state <= first_sending_5;
                                                    end if;
                            when first_sending_5 => if (m_axis_tready = '1') then
                                                    state <= first_sending_6;
                                                    end if;   
                            when first_sending_6 => if (m_axis_tready    = '1') then
                                                    state <= waiting;
                                                    end if;
                            when last_sending_1 => if (m_axis_tready = '1') then
                                                   state <= last_sending_2;
                                                   end if;
                            when last_sending_2 => if (m_axis_tready = '1') then
                                                   state <= last_sending_3;
                                                   end if;   
                            when last_sending_3 => if (m_axis_tready = '1') then
                                                   state <= end_file;
                                                   end if;    
                            when end_file => state <= waiting;         
                            when others => state <= reset_state;
                            end case;
                     end if;
end if;
end process;


process(clock, resetn)
	begin
	if (clock'event and clock = '1') then
		if (resetn = '0') then
		ip_tot_len<=(others=>'0');
		else
		   if(flag_no_payload='1' ) then
		       ip_tot_len <=x"3400";
		       ip_chksum  <=x"70a3";
		   else
		       ip_tot_len <=x"3404";
		       ip_chksum  <=x"709f";
		   end if;
		end if;
	end if;
end process;
		




--ip_tot_len<= x"aaaa" when flag_no_payload='1' else   --0034
--             x"bbbb";     --0434

SEND_PCKT: process(buff_dim,eth_src_mac,eth_src_mac,data_out_1,data_out_2,data_out_3,data_out_4,data_out_5,data_out_6,data_out_7,data_out_8_temp,
         data_out_8,state,timestamp,ts_echo,seq_num_t,ack_num_t,tcp_src_port, tcp_dst_port, flag_xtra)
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
       when sending_1 => ready <= '0';--ip_checksum
                         m_axis_tvalid <= '1';
                         m_axis_tdata <= ntoh16(ip_dst(15  downto 0)) & ntoh32(ip_src) & ip_chksum &
                                         ip_proto & ip_ttl  & ip_frag_offset & ip_flags & ntoh16(ip_identif)&
                                         ip_tot_len & ip_diff_serv & ip_ver_hdr_len & ntoh16(eth_pckt_type)&
                                         eth_src_mac & eth_dst_mac;
                         m_axis_tkeep <= (others => '1');
                         m_axis_tuser <= x"000000000000000000000000" & x"00" & x"00" & x"0442";
       
       
        when sending_1_plus => ready <= '0';
                                  m_axis_tvalid <= '1';
                                                  --= x"00000000"                                 ntoh16(tcp_urgent_ptr)
                                  m_axis_tdata <= ntoh16(flag_xtra_2(15 downto 0)) & ntoh32(timestamp) & x"0A080101" & x"0000"               & x"0000" & ntoh16(tcp_win_size)&
                                                  flag_xtra(7 downto 0) & x"80"& ntoh32(ack_num_t) & ntoh32(seq_num_t) & ntoh16(tcp_dst_port) & ntoh16(tcp_src_port)&
                                                  ntoh16(ip_dst(31 downto 16));                                         
                                           
                                           --m_axis_tdata(223 downto 0) <= (others=>'0');
                                           m_axis_tuser <= (others => '0');
                                           m_axis_tkeep<=(others=>'1');
                                           m_axis_tlast <= '0';
         
         
         
         
         
         
         
         
         when sending_2_plus => ready <= '0';
                           m_axis_tvalid <= '1';
                           --m_axis_tdata(255 downto 224) <= flag_xtra_2;
                            m_axis_tdata <=  x"0000"&x"0000" &x"00000000" & x"0000" & x"0000"& x"1111" & x"0000"&
                                            x"00" & x"00"&x"00000000"& x"00000000" & x"0000"&x"0000"&
                                           ntoh16(flag_xtra_2(31 downto 16));
                           
                           
                           
                           --m_axis_tdata(223 downto 0) <= (others=>'0');
                           m_axis_tuser <= (others => '0');
                           m_axis_tkeep<=x"00000003";
                           m_axis_tlast <= '1';
        
        
        
        
        when sending_2 => ready <= '0';
                          m_axis_tvalid <= '1';
                                          --= x"00000000"                                 ntoh16(tcp_urgent_ptr)                       CHKSUM
                          m_axis_tdata <= ntoh16(flag_xtra_2(15 downto 0)) & ntoh32(timestamp) & x"0A080101" & x"0000" & x"0000" & ntoh16(tcp_win_size)&
                                          flag_xtra(7 downto 0) & x"80"& ntoh32(ack_num_t) & ntoh32(seq_num_t) & ntoh16(tcp_dst_port) & ntoh16(tcp_src_port)&
                                          ntoh16(ip_dst(31 downto 16));
                          m_axis_tkeep <= (others => '1');
                          m_axis_tlast <= '0';
                          m_axis_tuser <= (others => '0');
        when sending_3 => ready <= '0';
                          m_axis_tvalid <= '1';
                          m_axis_tdata <= data_out_8(15 downto 0) & data_out_7 & data_out_6 & data_out_5 & data_out_4 & data_out_3 & data_out_2&
                                          data_out_1 & x"0101";  --ntoh16(flag_xtra_2(31 downto 16)); --data_out_1 & data_out_2 & data_out_3 & data_out_4 & data_out_5 & data_out_6 & data_out_7 & data_out_8(31 downto 16) & x"0101";--(others=>'0');--data_out_1 & data_out_2 & data_out_3 & data_out_4 & data_out_5 & data_out_6 & data_out_7 & data_out_8;--& x"00000000" & x"00000000" & x"00000000" & x"00000000" & x"00000000" & x"00000000" & x"00000000";
               --m_axis_tdata <= seq_num_t & x"01234567" & seq_num_t & x"12345678" & x"00000000000000000000000000000000";
                          m_axis_tkeep <= (others => '1');
                          m_axis_tlast <= '0';
                          m_axis_tuser <= (others => '0');
        when sending_4 => ready <= '0';
                          m_axis_tvalid <= '1';
                          m_axis_tdata <= data_out_8(15 downto 0)  & data_out_7 & data_out_6 & data_out_5 & data_out_4 & data_out_3&
                                          data_out_2 & data_out_1 & data_out_8_temp ;--data_out_8_temp  & data_out_1 & data_out_2 & data_out_3 & data_out_4 & data_out_5 & data_out_6 & data_out_7 & data_out_8(31 downto 16); -- & x"00000000" & x"00000000" & x"00000000" & x"00000000" & x"00000000" & x"00000000" & x"00000000";
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
        when first_sending_1 => ready <= '0';
                                m_axis_tvalid <= '1';
                                m_axis_tdata <= ntoh16(ip_dst(15  downto 0)) & ntoh32(ip_src) & ntoh16(x"a370")  & ip_proto & ip_ttl&
                                ip_frag_offset & ip_flags & ntoh16(ip_identif) & ntoh16(x"0034") & ip_diff_serv & ip_ver_hdr_len&
                                ntoh16(eth_pckt_type) & eth_src_mac & eth_dst_mac;
                                m_axis_tkeep <= (others => '1');
                                m_axis_tlast <= '0';
                                m_axis_tuser <= x"000000000000000000000000" & x"00" & x"00" & x"0042";
        when first_sending_2 => ready <= '0';
                                m_axis_tvalid <= '1';
                                m_axis_tdata <= ntoh16(flag_xtra_2(15 downto 0)) & ntoh32(timestamp) & x"0A080101" & x"0000"               & x"0000" &
                                                ntoh16(tcp_win_size) & x"0280" & x"00000000" & x"00000000" & ntoh16(tcp_dst_port)&
                                                ntoh16(tcp_src_port) & ntoh16(ip_dst(31 downto 16));
                                m_axis_tkeep <= (others => '1');
                                m_axis_tlast <= '0';
                                m_axis_tuser <= (others => '0');
        when first_sending_3 => ready <= '0';
                                m_axis_tvalid <= '1';
                                m_axis_tdata <= (others => '0');
                                m_axis_tkeep <= x"00000003";
                                m_axis_tlast <= '1';
                                m_axis_tuser <= (others => '0');
        when first_sending_4 => ready <= '0';
                                m_axis_tvalid <= '1';
                                m_axis_tdata <= ntoh16(ip_dst(15  downto 0)) & ntoh32(ip_src) & ntoh16(x"a370")  & ip_proto & ip_ttl &
                                                ip_frag_offset& ip_flags & ntoh16(ip_identif) & ntoh16(x"0034") & ip_diff_serv & ip_ver_hdr_len&
                                                ntoh16(eth_pckt_type)&eth_src_mac & eth_dst_mac;
                                m_axis_tkeep <= (others => '1');
                                m_axis_tlast <= '0';
                                m_axis_tuser <= x"000000000000000000000000" & x"00" & x"00" & x"0042";
        when first_sending_5 => ready <= '0';
                                m_axis_tvalid <= '1';
                                m_axis_tdata <= ntoh16(flag_xtra_2(15 downto 0)) & ntoh32(timestamp) & x"0A080101" & x"0000"               & x"0000" &
                                                ntoh16(tcp_win_size)&x"1080" & ntoh32(ack_num_t) & x"01000000" & ntoh16(tcp_dst_port)&
                                                ntoh16(tcp_src_port) & ntoh16(ip_dst(31 downto 16));
                                m_axis_tkeep <= (others => '1');
                                m_axis_tlast <= '0';
                                m_axis_tuser <= (others => '0');
        when first_sending_6 => ready <= '0';
                                m_axis_tvalid <= '1';
                                m_axis_tdata <= (others => '0');
                                m_axis_tkeep <= x"00000003";
                                m_axis_tlast <= '1';
                                m_axis_tuser <= (others => '0');
        when last_sending_1 => ready <= '0';
                               m_axis_tvalid <= '1';
                               m_axis_tdata <= ntoh16(ip_dst(15  downto 0)) & ntoh32(ip_src) & ntoh16(x"a370")  & ip_proto & ip_ttl&
                                               ip_frag_offset & ip_flags & ntoh16(ip_identif) & ntoh16(x"0034") & ip_diff_serv & ip_ver_hdr_len&
                                               ntoh16(eth_pckt_type) & eth_src_mac & eth_dst_mac;
                               m_axis_tkeep <= (others => '1');
                               m_axis_tlast <= '0';
                               m_axis_tuser <= x"000000000000000000000000" & x"00" & x"00" & x"0042";
        when last_sending_2 => ready <= '0';
                               m_axis_tvalid <= '1';
                               m_axis_tdata <= ntoh16(flag_xtra_2(15 downto 0)) & ntoh32(timestamp) & x"0A080101" & x"0000"               & x"0000" & ntoh16(tcp_win_size)&
                                               x"0180" & ntoh32(ack_num_t) & ntoh32(seq_num_t) & ntoh16(tcp_dst_port) & ntoh16(tcp_src_port)&
                                               ntoh16(ip_dst(31 downto 16));
                               m_axis_tkeep <= (others => '1');
                               m_axis_tlast <= '0';
                               m_axis_tuser <= (others => '0');
        when last_sending_3 => ready <= '0';
                               m_axis_tvalid <= '1';
                               m_axis_tdata <= (others => '0');
                               m_axis_tkeep <= x"00000003";
                               m_axis_tlast <= '1';
                               m_axis_tuser <= (others => '0');
        when others => ready <= '0';
                       m_axis_tvalid <= '0';
                       m_axis_tdata <= (others => '0');
                       m_axis_tkeep <= (others => '1');
                       m_axis_tlast <= '0';
                       m_axis_tuser <= (others => '0');
        end case;
end process;
      
      
MAC_ADDR_AND_BUFF_DIM: process (axi_addr,eth_dst_mac,eth_src_mac,buff_dim,data_letto_1,data_letto_2,data_letto_3,
                                data_letto_4,data_letto_5,data_letto_6,data_letto_7,data_letto_8,data_scritto, we, we1, we2, we3,
                                we4, we5, we6, we7, we8,data_scritto)
 begin
 if (we='0')then 
    case axi_addr is
    when x"91fffffb" => data_letto <= buff_dim;
    when x"91fffffc" => data_letto <= eth_src_mac(47 downto 16);
    when x"91fffffd" => data_letto <= eth_src_mac(15 downto 0) & x"0000";
    when x"91fffffe" => data_letto <= eth_dst_mac(47 downto 16);
    when x"91ffffff" => data_letto <= eth_dst_mac(15 downto 0) & x"0000";
    when x"91fffff9" => data_letto<=  ip_src ;
    when x"91fffff8" => data_letto<=  ip_dst;
    when x"91fffff7" => data_letto<=  tcp_src_port(15 downto 0) & x"0000";
    when x"91fffff6" => data_letto<=  tcp_dst_port(15 downto 0) & x"0000";
    when x"91fffffa" => data_letto<=x"0000"&ip_checksum;
    when x"9aaaaaaa" => data_letto<= count_p;
     when x"9bbbbbbb" => data_letto<= count_p_1;
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
-- data_letto_1 <= x"00000000";
-- data_letto_2 <= x"00000000";
-- data_letto_3 <= x"00000000";
-- data_letto_4 <= x"00000000";
-- data_letto_5 <= x"00000000";
-- data_letto_6 <= x"00000000";
-- data_letto_7 <= x"00000000";
-- data_letto_8 <= x"00000000";
 
    case axi_addr is
    when x"91fffffb" => buff_dim <= data_scritto;            
    when x"91fffffc" => eth_src_mac(47 downto 16) <= data_scritto;    
    when x"91fffffd" => eth_src_mac (15 downto 0) <= data_scritto(31 downto 16);
    when x"91fffffe" => eth_dst_mac (47 downto 16) <= data_scritto;
    when x"91ffffff" => eth_dst_mac (15 downto 0) <= data_scritto(31 downto 16);
    when x"91fffff9" =>   ip_src <= data_scritto;
    when x"91fffff8" =>   ip_dst<=data_scritto;
    when x"91fffff7" =>  tcp_src_port<=data_scritto(31 downto 16)    ;
    when x"91fffff6" =>  tcp_dst_port<=data_scritto(31 downto 16);
    when x"91fffffa" => ip_checksum<= data_scritto(15 downto 0);
    when others => case axi_addr (2 downto 0) is
                   when "000" => we1 <= we;
                   when "001" => we2 <= we;
                   when "010" => we3 <= we;
                   when "011" => we4 <= we;
                   when "100" => we5 <= we;
                   when "101" => we6 <= we;
                   when "110" => we7 <= we;
                   --when "111" => we8 <= we;
                   when others => we8 <= we;
                   end case;
    end case;

end if;
end process;

addr_seq <=seq_num_t-seq_num_init;

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