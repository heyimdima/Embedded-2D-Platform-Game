module led_pause_override(
    input  wire       switch,         // Switct that pauses the game
    input  wire [9:0] system_leds, // LED signals from the main system
    output wire [9:0] LEDR         // Final LED outputs to the board
);

  // When sw0 is high, force all LEDs on; otherwise, pass through system_leds.
  assign LEDR = switch ? 10'b1111111111 : system_leds;

endmodule
