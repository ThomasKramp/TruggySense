include <Circuit_plate_holder.scad>

//casing holder dimentions
ch_t = 1.6;       //thickness walls
ch_s_w = 10;     // width sides
ch_s_h = 50;      // height sides

//mounting holes
mh_dia = 3.2; //Diameter
mh_w = 8;
mh_d = 8;

translate([1/3*(c_w+ch_t*2+e_m*2)-mh_w/2,-mh_d, 0]) mount_hole();
translate([5/6*(c_w+ch_t*2+e_m*2)-mh_w/2,-mh_d, 0]) mount_hole();


translate([1/6*(c_w+ch_t*2+e_m*2)-mh_w/2,c_d+ch_t*2+e_m*2, 0]) mount_hole();
translate([2/3*(c_w+ch_t*2+e_m*2)-mh_w/2,c_d+ch_t*2+e_m*2, 0]) mount_hole();

union(){
    //base
    cube([c_w+ch_t*2+e_m*6, c_d+ch_t*2+e_m*6, ch_t]);
    //sides
    side();
    translate([c_w+ch_t*2+e_m*6, 0, 0])rotate([0,0,90])side();
    translate([c_w+ch_t*2+e_m*6, c_d+ch_t*2+e_m*6, 0])rotate([0,0,180])side();
    translate([0, c_d+ch_t*2+e_m*6, 0])rotate([0,0,270])side();
}
//color("green")translate([ch_t,ch_t,ch_t])circuit_holder();

module side(){
    union(){
        cube([ch_t, ch_s_w, ch_s_h]);
        cube([ch_s_w, ch_t , ch_s_h]);
    }
}

module mount_hole(){
    difference(){
        cube([mh_w, mh_d, ch_t]);
        translate([mh_w/2, mh_d/2])cylinder(h=ch_s_h, d=mh_dia);
    }
}