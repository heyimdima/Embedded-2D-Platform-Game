LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY bcd_7segment IS
PORT (
    bcd_in : IN STD_LOGIC_VECTOR(3 DOWNTO 0);
    seven_segment_out : OUT STD_LOGIC_VECTOR(6 DOWNTO 0)
);
END bcd_7segment;

ARCHITECTURE behavior OF bcd_7segment IS
BEGIN
    PROCESS(bcd_in)
    BEGIN
        CASE bcd_in IS
            -- Active low outputs for 7-segment display
            WHEN "0000" => seven_segment_out <= "1000000"; -- 0
            WHEN "0001" => seven_segment_out <= "1111001"; -- 1
            WHEN "0010" => seven_segment_out <= "0100100"; -- 2
            WHEN "0011" => seven_segment_out <= "0110000"; -- 3
            WHEN "0100" => seven_segment_out <= "0011001"; -- 4
            WHEN "0101" => seven_segment_out <= "0010010"; -- 5
            WHEN "0110" => seven_segment_out <= "0000010"; -- 6
            WHEN "0111" => seven_segment_out <= "1111000"; -- 7
            WHEN "1000" => seven_segment_out <= "0000000"; -- 8
            WHEN "1001" => seven_segment_out <= "0010000"; -- 9
            WHEN OTHERS => seven_segment_out <= "1111111"; -- Blank display for invalid inputs
        END CASE;
    END PROCESS;
END behavior;
