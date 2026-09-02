$fn = 60;

e_m = 0.1;      //3D-printing error margin

//circuit board
cb_w = 21.2;    //circuit board width
cb_d = 23.4;    //circuit board depth
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
c_w_t = 0.8;
c_w = 68.7;//c_t*2+e_m*2+cb_w;       //case width
c_d = 48.6;//c_t*2+e_m*2+cb_d;       //case depth

//Case height needs to be changed by the circuit board height

standoff_depth = 3;

difference(){
    cube([c_w, c_d, c_t]);
    union(){
        translate([c_w_t,cb_w+c_w_t,4])rotate([0,0,-90]) board();
        translate([c_w_t*2 + cb_d,cb_w+c_w_t,4])rotate([0,0,-90]) board();
        translate([c_w_t*2+cb_d*2,cb_d+c_w_t+2,4])rotate([0,0,90]) board();
        translate([(c_w_t*3 + cb_w),cb_d+2+c_w_t,4])rotate([0,0,90]) board();
        translate([c_w_t*3 + cb_d*2, 2, 4])board_2();
    }
}



module board_2(){
    difference(){
        union(){
            translate([0,11,-2])cube([18.7, 34.2, 6]);
            translate([18.7/2-9/2 - 1, -20, -1.8-1-2])cube([11, 16.4*2, 10]);
        }
        union(){
            //translate([-18.7+(18.7-(3-18.7)),0+11,-4])cube([18.7-6, 34+2, 4]);
            //translate([-18.7+9.25,0+11,-4])cube([18.5-6, 34+2, 4]);
            //translate([3,0,-4])cube([18.5-6, 34, 4]);
        }
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
    translate([cb_w -7/2, cb_d/2 -7/2, cb_h])cube([7, 7, cb_h_t]);
}