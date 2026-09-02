$fn = 60;

e_m = 0.1;      //3D-printing error margin

//circuit board
cb_w = 26;    //circuit board width
cb_d = 22.8;    //circuit board depth
cb_h = 1.75;     //circuit board height

//circuit board placement
//cb_p_h = 4;   //height
cb_p_s_d = 0.7;   //sloth depth
cb_b_s = 0;     // back slot yes(1) or no(0)
//circuit board height needs to be changed by the circuit board height

//component opening
cb_p_h_t = 3.55;   //height top
cb_p_h_b = 1.5;   //height bottom

//casing
c_t = 1;        //Case thick
c_w = c_t*2+e_m*2+cb_w;       //case width
c_d = c_t*2+e_m*2+cb_d;       //case depth
c_h = c_t*2+e_m*2+cb_h+cb_p_h_b+cb_p_h_t;       //case height
echo(c_w); //68.7
echo(c_d); //48.6
echo(c_h); //22.8
//Case height needs to be changed by the circuit board height

circuit_holder();

module circuit_holder(){
    difference(){
        cube([c_w, c_d, c_h]);
        pcb_no_go_zone();
    }
}

module pcb_no_go_zone(){
    union(){
        union(){
            if ( cb_b_s == 1){
                translate([(c_w-cb_w)+c_t-e_m*2+e_m,c_t+cb_p_s_d,(cb_p_h_b+e_m + c_t)+cb_h+e_m*2])
                cube([(cb_w+e_m*2)-cb_p_s_d + c_t, (cb_d+e_m*2)-cb_p_s_d*2, cb_p_h_t]);
                translate([(c_w-cb_w)+c_t-e_m*2+e_m,c_t+cb_p_s_d,(cb_p_h_b+e_m + c_t) - cb_p_h_b])
                cube([(cb_w+e_m*2)-cb_p_s_d+ c_t, (cb_d+e_m*2)-cb_p_s_d*2, cb_p_h_b]);
            }
            else{
                translate([(c_w-cb_w)-c_t-e_m*2+e_m,c_t+cb_p_s_d,(cb_p_h_b+e_m+c_t)+cb_h+e_m*2])
                cube([(cb_w+e_m*2)+ c_t, (cb_d+e_m*2)-cb_p_s_d*2, cb_p_h_t]);
                translate([(c_w-cb_w)-c_t-e_m*2+e_m,c_t+cb_p_s_d,(cb_p_h_b+e_m+c_t) - cb_p_h_b])
                cube([(cb_w+e_m*2)+ c_t, (cb_d+e_m*2)-cb_p_s_d*2, cb_p_h_b]);
            }
        }
        translate([(c_w-cb_w)-c_t-e_m*2+e_m,c_t+e_m,cb_p_h_b+e_m+c_t])
        cube([cb_w+e_m*2+ c_t, cb_d+e_m*2, cb_h+e_m*2]);
    }
}
