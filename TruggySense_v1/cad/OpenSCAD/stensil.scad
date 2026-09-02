difference(){
    cube([100,80,0.2]);
    translate([-100,140,-0.05]) linear_extrude(.3) import("Teensy_board_07_adc_buffer-B_Paste.dxf");
}