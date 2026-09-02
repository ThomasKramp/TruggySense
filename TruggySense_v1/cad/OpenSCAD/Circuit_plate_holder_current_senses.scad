$fn = 60;

e_m = 0.1;      //3D-printing error margin

//circuit board
cb_w = 21;    //circuit board width
cb_d = 23.2;    //circuit board depth
cb_h = 1.6;     //circuit board height
cb_h_t = 13;    //circuit board total height including components

cb_h_dia = 2.4;   //Hole size
cb_h_w = 15.30; //Hole width dimention
cb_h_d = 13.3;  //hole depth dimention

//circuit board placement
//cb_p_h = 4;   //height
cb_p_s_d = 2;   //sloth depth
cb_b_s = 0;     // back slot yes(1) or no(0)
//circuit board height needs to be changed by the circuit board height


//casing
c_t = 7;        //Case thick
c_w_t = 1;
c_w = 68.7;//c_t*2+e_m*2+cb_w;       //case width
c_d = 48.6;//c_t*2+e_m*2+cb_d;       //case depth

//Case height needs to be changed by the circuit board height

standoff_depth = 3;

difference(){
    cube([c_w, c_d, c_t]);
    union(){
        translate([c_w_t, c_w_t, 4]) board();
        translate([c_w_t*2 + cb_w, c_w_t, 4]) board();
        translate([c_w_t*3 + cb_w*2, c_w_t, 4]) board();
        
        translate([c_w_t, 1.3 + cb_d, 4]) board();
        translate([c_w_t*2 + cb_w, 1.3+cb_d, 4]) board();
        translate([c_w_t*3 + cb_w*2, 1.3+cb_d, 4]) board();
    }
}

module board(){
    difference(){
        translate([0,0,-3]) cube([cb_w, cb_d, cb_h_t]);
        union(){
            translate([2.5,2.5,-3]) cylinder(cb_h_t, d=cb_h_dia);
            translate([2.5 + 16,2.5,-3]) cylinder(cb_h_t, d=cb_h_dia);
            translate([2.5,2.5 + 17.5,-3]) cylinder(cb_h_t, d=cb_h_dia);
            translate([2.5 + 16,2.5 + 17.5 ,-3]) cylinder(cb_h_t, d=cb_h_dia);
            
            translate([2.5,2.5,-3]) cylinder(standoff_depth, d=cb_h_dia + 2);
            translate([2.5 + 16,2.5,-3]) cylinder(standoff_depth, d=cb_h_dia + 2);
            translate([2.5,2.5 + 17.5,-3]) cylinder(standoff_depth, d=cb_h_dia + 2);
            translate([2.5 + 16,2.5 + 17.5 ,-3]) cylinder(standoff_depth, d=cb_h_dia + 2);
        }
    }
}