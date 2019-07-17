library ieee;
use ieee.std_logic_1164.all;
use IEEE.STD_LOGIC_unsigned.ALL;
use ieee.std_logic_arith.all;

library work;
use work.salutil.all;
--use work.txt_util.all;



entity ht128dp_calendar is               
  generic (
              key_len:integer:=40;
              value_len:integer:=32
          );
  port ( 
         clock : in std_logic;
         resetn : in std_logic;
         
         --AXI interface
         we: in std_logic;
         rd: in std_logic;
         input_din: in std_logic_vector(1+key_len+value_len downto 0);
         addr :in std_logic_vector(11 downto 0);
         data_out :out std_logic_vector(1+key_len+value_len downto 0);

         --Hash Interface
         remove : in std_logic; -- 1 to remove
         insert : in std_logic; -- 1 to insert
         key    : in std_logic_vector(key_len-1 downto 0); -- insert key
         value  : in std_logic_vector(value_len-1 downto 0); -- insert value
         search_key : in std_logic_vector(key_len-1 downto 0); -- search key
         collision        : out std_logic; -- 1 if a key cannot be inserted
         
         out_count_collision :out std_logic_vector(31 downto 0);
         clear_count_collision: in std_logic;
         out_count_item :out std_logic_vector(31 downto 0);

         match  : out std_logic; -- 1 if hit on query
         output : out std_logic_vector(value_len-1 downto 0) -- value associated with the search key
       );
end ht128dp_calendar;

architecture behavioral of ht128dp_calendar is

  type ht_mem_type is array (1023 downto 0) of std_logic_vector(1+key_len+value_len downto 0);
  type stash_type is array  (7 downto 0) of std_logic_vector(1+key_len+value_len downto 0);
  signal dout_query1,dout_query2,dout_query3,dout_query4:std_logic_vector(1+key_len+value_len downto 0);
  signal dout_insert1,dout_insert2,dout_insert3,dout_insert4:std_logic_vector(1+key_len+value_len downto 0);
  
  signal write_enable_1,write_enable_2, write_enable_3, write_enable_4: std_logic;
  signal write_enable_q1,write_enable_q2, write_enable_q3, write_enable_q4: std_logic;
  signal memory_write_enable_1,memory_write_enable_2, memory_write_enable_3, memory_write_enable_4: std_logic;

  signal din_query1,din_insert1:  std_logic_vector(1+key_len+value_len downto 0);
  signal din_query2,din_insert2:  std_logic_vector(1+key_len+value_len downto 0);
  signal din_query3,din_insert3:  std_logic_vector(1+key_len+value_len downto 0);
  signal din_query4,din_insert4:  std_logic_vector(1+key_len+value_len downto 0);
  signal dinb1:  std_logic_vector(1+key_len+value_len downto 0);
  signal dinb2:  std_logic_vector(1+key_len+value_len downto 0);
  signal dinb3:  std_logic_vector(1+key_len+value_len downto 0);
  signal dinb4:  std_logic_vector(1+key_len+value_len downto 0);

  signal mem_addr: std_logic_vector(39 downto 0);
  signal dout1,dout2,dout3,dout4:  std_logic_vector(1+key_len+value_len downto 0);

  signal search_d,key_d    : std_logic_vector(key_len-1 downto 0);
  signal value_d  : std_logic_vector(value_len-1 downto 0);
  signal remove_d :std_logic:='0';
  signal insert_d :std_logic:='0';
  signal coll,deleted,inserted :std_logic;
  
  attribute max_fanout: integer;
  attribute max_fanout of output: signal is 1;


  signal addr_query1,addr_query2,addr_query3,addr_query4:std_logic_vector(9 downto 0);
  signal addrb_1,addrb_2,addrb_3,addrb_4,addr_insert1,addr_insert2,addr_insert3,addr_insert4:std_logic_vector(9 downto 0);

  signal ht_mem1: ht_mem_type := ((others=> (others=>'0'))); --(others => (x"0000000000000000000000000000000000000000" & "00"));
  signal ht_mem2: ht_mem_type := ((others=> (others=>'0'))); --(others => (x"0000000000000000000000000000000000000000" & "00"));
  signal ht_mem3: ht_mem_type := ((others=> (others=>'0'))); --(others => (x"0000000000000000000000000000000000000000" & "00"));
  signal ht_mem4: ht_mem_type := ((others=> (others=>'0'))); --(others => (x"0000000000000000000000000000000000000000" & "00"));
-- attribute ram_style:string;
--  attribute ram_style of ht_mem1:signal is "block";
--  attribute ram_style of ht_mem2:signal is "block";
--  attribute ram_style of ht_mem3:signal is "block";
--  attribute ram_style of ht_mem4:signal is "block";

          signal to_hash: std_logic_vector(key_len-1 downto 0);
          signal hashed_query,hashed_insert: std_logic_vector(63 downto 0);


          signal count_collision_next,count_collision: std_logic_vector(31 downto 0);
          signal count_item_next,count_item: std_logic_vector(31 downto 0);
  
          component  rams_tdp_rf_rf is
          PORT (
                --Inputs - Port A
              CLKA       : IN STD_LOGIC;
              WEA            : IN STD_LOGIC;
              ADDRA          : IN STD_LOGIC_VECTOR(9 DOWNTO 0);
              DIA           : IN STD_LOGIC_VECTOR(81 DOWNTO 0);
              DOA          : OUT STD_LOGIC_VECTOR(81 DOWNTO 0);
            
              --Inputs - Port B
              CLKB           : IN STD_LOGIC;
              WEB            : IN STD_LOGIC;
              ADDRB          : IN STD_LOGIC_VECTOR(9 DOWNTO 0);
              DIB           : IN STD_LOGIC_VECTOR(81 DOWNTO 0);
              DOB          : OUT STD_LOGIC_VECTOR(81 DOWNTO 0)
            );
          end component;
          
  
  
          
        begin

         out_count_collision  <= count_collision; 
         out_count_item <=count_item; 



          hashed_query(39 downto 0) <= search_key(39 downto 0 );
          hashed_query(63 downto 40) <= (others=>'0');
          
          addr_query1<=  hashed_query(17 downto 8)  xor hashed_query(9 downto 0); --xor hashed_query(49 downto 40);
          addr_query2<=  hashed_query(19 downto 10) xor hashed_query(29 downto 20);
          addr_query3<=  hashed_query(29 downto 20) xor hashed_query(23 downto 14);
          addr_query4<=  hashed_query(39 downto 30) xor hashed_query(24 downto 15);

          --hashed_insert <= key(63 downto 0 ) xor myror(key(127 downto 64),7);

          hashed_insert(39 downto 0) <= key_d when write_enable_1='1' else
                                        key_d when write_enable_2='1' else
                                        key_d when write_enable_3='1' else
                                        key_d when write_enable_4='1' else
                                        key;
                                        
          hashed_insert(63 downto 40) <= (others=>'0');


          addr_insert1<=  hashed_insert(17 downto 8)  xor hashed_insert(9 downto 0);-- xor hashed_insert(49 downto 40);
          addr_insert2<=  hashed_insert(19 downto 10) xor hashed_insert(29 downto 20);
          addr_insert3<=  hashed_insert(29 downto 20) xor hashed_insert(23 downto 14);
          addr_insert4<=  hashed_insert(39 downto 30) xor hashed_insert(24 downto 15);


          process(clock)
          begin
            if(clock'event and clock = '1') then
              search_d<=search_key;
              key_d<= key;
              value_d<= value;
              remove_d<=remove;
              insert_d<=insert;
            end if;
          end process;

--DUAL PORT RAM1
R1: rams_tdp_rf_rf port map(clock,write_enable_q1,addr_query1,din_query1,dout_query1,clock,write_enable_1,addrb_1,dinb1,dout_insert1);
R2: rams_tdp_rf_rf port map(clock,write_enable_q2,addr_query2,din_query2,dout_query2,clock,write_enable_2,addrb_2,dinb2,dout_insert2);
R3: rams_tdp_rf_rf port map(clock,write_enable_q3,addr_query3,din_query3,dout_query3,clock,write_enable_3,addrb_3,dinb3,dout_insert3);
R4: rams_tdp_rf_rf port map (clock,write_enable_q4,addr_query4,din_query4,dout_query4,clock,write_enable_4,addrb_4,dinb4,dout_insert4);


--          --AXI Read
          addrb_1<= addr(9 downto 0) when ((we='1') or (rd='1')) else addr_insert1;
          addrb_2<= addr(9 downto 0) when ((we='1') or (rd='1')) else addr_insert2;
          addrb_3<= addr(9 downto 0) when ((we='1') or (rd='1')) else addr_insert3;
          addrb_4<= addr(9 downto 0) when ((we='1') or (rd='1')) else addr_insert4;
          data_out <= dout_insert1 when addr(11 downto 10)="00" else 
                      dout_insert2 when addr(11 downto 10)="01" else
                      dout_insert3 when addr(11 downto 10)="10" else 
                      dout_insert4;


--          --AXI WRITE
--          --        (AXI)                     OPP
          dinb1 <= input_din when we='1' else din_insert1;
          dinb2 <= input_din when we='1' else din_insert2;
          dinb3 <= input_din when we='1' else din_insert3;
          dinb4 <= input_din when we='1' else din_insert4;

          write_enable_1 <= '1' when we='1' and addr(11 downto 10)="00" else memory_write_enable_1;
          write_enable_2 <= '1' when we='1' and addr(11 downto 10)="01" else memory_write_enable_2;
          write_enable_3 <= '1' when we='1' and addr(11 downto 10)="10" else memory_write_enable_3;
          write_enable_4 <= '1' when we='1' and addr(11 downto 10)="11" else memory_write_enable_4;




          
        process(clock)
        begin
            if(clock'event and clock = '1') then
		if (RESETN = '0') then
		    count_collision <= x"00000000";
		    count_item <=  x"00000000";
		else
		    if (clear_count_collision='1') then 
		      count_collision <= x"00000000";
		    else 
		      count_collision <= count_collision_next;
		    end if;  
		    count_item <= count_item-deleted+inserted;
		end if;
	    end if;
          end process;

          -- control logic
          process(count_collision,key_d,value_d,insert_d,dout_insert1,dout_insert2,dout_insert3,dout_insert4)

          begin
            collision  <='0';
            coll  <='0';
                        
            count_collision_next <= count_collision;
            din_insert1(key_len+value_len) <= '0'; 
            din_insert1(key_len+value_len+1) <= '0'; 
            din_insert1(key_len-1 downto 0) <= key_d;
            din_insert2(key_len+value_len) <= '0'; 
            din_insert2(key_len+value_len+1) <= '0'; 
            din_insert2(key_len-1 downto 0) <= key_d;
            din_insert3(key_len+value_len) <= '0'; 
            din_insert3(key_len+value_len+1) <= '0'; 
            din_insert3(key_len-1 downto 0) <= key_d;
            din_insert4(key_len+value_len) <= '0'; 
            din_insert4(key_len+value_len+1) <= '0'; 
            din_insert4(key_len-1 downto 0) <= key_d;

            din_insert1(key_len+value_len-1 downto key_len)   <= value_d;
            din_insert2(key_len+value_len-1 downto key_len)   <= value_d;
            din_insert3(key_len+value_len-1 downto key_len)   <= value_d;
            din_insert4(key_len+value_len-1 downto key_len)   <= value_d;


            memory_write_enable_1<='0';
            memory_write_enable_2<='0';
            memory_write_enable_3<='0';
            memory_write_enable_4<='0';

            -- TODO: check if item is already present and manage this as a collision
            inserted<='0';
            if (insert_d='1') then
              if dout_insert1(key_len+value_len)='0' then
                din_insert1(key_len+value_len) <= '1'; -- metto a zero con remove;
                din_insert1(key_len+value_len+1) <= '1'; -- update flag;
                memory_write_enable_1<='1';
                inserted<='1';
              elsif dout_insert2(key_len+value_len)='0' then
                din_insert2(key_len+value_len) <= '1'; -- metto a zero con remove;
                din_insert2(key_len+value_len+1) <= '1'; -- update flag;
                memory_write_enable_2<='1';
                inserted<='1';
              elsif dout_insert3(key_len+value_len)='0' then
                din_insert3(key_len+value_len) <= '1'; -- metto a zero con remove;
                din_insert3(key_len+value_len+1) <= '1'; -- update flag;
                memory_write_enable_3<='1';
                inserted<='1';
              elsif dout_insert4(key_len+value_len)='0' then
                din_insert4(key_len+value_len) <= '1'; -- metto a zero con remove;
                din_insert4(key_len+value_len+1) <= '1'; -- update flag;
                memory_write_enable_4<='1';
                inserted<='1';
              else
                inserted<='0';
                count_collision_next <= count_collision+1;
                collision  <='1';
                coll  <='1';
              end if;
            end if;
          --end if;

        end process;



process(clock)
          begin
            if(clock'event and clock = '1') then
              if write_enable_1='1' then
                  report "@0x" & hstr(search_key) & " inserted in RAM1 with expiration time: 0x" & hstr(key_d) & LF;
              end if;
              if write_enable_2='1' then
                   report "@0x" & hstr(search_key) & " inserted in RAM2 with expiration time: 0x" & hstr(key_d) & LF;
              end if;
              if write_enable_3='1' then
                report "@0x" & hstr(search_key) & " inserted in RAM3 with expiration time: 0x" & hstr(key_d) & LF;
              end if;
              if write_enable_4='1' then
                 report "@0x" & hstr(search_key) & " inserted in RAM4 with expiration time: 0x" & hstr(key_d) & LF;
              end if;                                          
              if coll='1' then
                report "@0x" & hstr(search_key) & " insertion failed with expiration time: 0x" & hstr(key_d) & LF;
              end if;
              if (deleted='1') then 
                report "@ 0x" & hstr(search_d) & " timer expired" & LF;
                report "@ " & integer'image(conv_integer(search_d(39 downto 8))) &  " us timer expired" & LF;
              end if;
          end if;
          end process;

-- control logic remove
          process(search_key,search_d,remove_d,dout_query1,dout_query2,dout_query3,dout_query4)

          begin
            din_query1(key_len+value_len+1 downto 0) <= (others=>'0');
            din_query2(key_len+value_len+1 downto 0) <= (others=>'0');
            din_query3(key_len+value_len+1 downto 0) <= (others=>'0');
            din_query4(key_len+value_len+1 downto 0) <= (others=>'0');

            write_enable_q1<='0';
            write_enable_q2<='0';
            write_enable_q3<='0';
            write_enable_q4<='0';
            match <='0';
            deleted<='0';
            output <=(others =>'0');
            if (remove_d='1') then
              if ((dout_query1(key_len-1 downto 0)=search_d) and dout_query1(key_len+value_len)='1') then 
                write_enable_q1<='1';
                output <= dout_query1(key_len+value_len-1 downto key_len);
                match <='1';
                deleted <='1';
              elsif ((dout_query2(key_len-1 downto 0)=search_d) and dout_query2(key_len+value_len)='1') then 
                write_enable_q2<='1';
                output <= dout_query2(key_len+value_len-1 downto key_len);
                match <='1';
                deleted <='1';
              elsif ((dout_query3(key_len-1 downto 0)=search_d) and dout_query3(key_len+value_len)='1') then 
                write_enable_q3<='1';
                output <= dout_query3(key_len+value_len-1 downto key_len);
                match <='1';
                deleted <='1';
              elsif ((dout_query4(key_len-1 downto 0)=search_d) and dout_query4(key_len+value_len)='1') then 
                write_enable_q4<='1';
                output <= dout_query4(key_len+value_len-1 downto key_len);
                match <='1';
                deleted <='1';
              else
                match <='0';
                deleted <='0';
              end if;
            end if;
         end process;
end behavioral;
