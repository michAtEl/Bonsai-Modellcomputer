anzahlKabel = 2;

// Breite pro Kabel
bKabel = 1;

// Höhe pro Kabel
hKabel = 1;

difference() {
    cube(size=[10 + anzahlKabel*bKabel, 8, 1+hKabel]);
    
    // Bohrloch
    translate([4, 4, -1])
        cylinder(10, 3.5/2, 3.5/2, $fn=40);
    
    // Aussparung für Kabel
    translate([8, -5, -1]){
        cube(size=[anzahlKabel*bKabel+0.3, 20, 1+hKabel]);
    }
}