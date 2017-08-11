--Particle...
name = "particle_6" ;
image = "particles_15.png" ;
x = 400 ;
y = 300 ;
height = 0 ;
width = 0 ;
rotation = 0 ;
scale = 1 ;
alpha = 0 ;
isPhysical = false ;
isStatic = false ;
mass = 0 ;
inertia = 0 ;
friction = 0 ;
elastic = 0 ;
--Particle Attributes...
count = 100 ;
startColor_R = 255 ;
startColor_G = 162 ;
startColor_B = 0 ;
endColor_R = 0 ;
endColor_G = 0 ;
endColor_B = 128 ;
startAlpha = 255 ;
endAlpha = 0 ;
minParticleLifeTime = 1 ;
maxParticleLifeTime = 100 ;
minStartSpeed = 0.01 ;
maxStartSpeed = 2 ;
acceleration_x = 0.96 ;
acceleration_y = 0.96 ;
gravity_x = 0 ;
gravity_y = -0.04 ;
minScale = 1 ;
maxScale = 4 ;
rpt = true ;
--end of Particle Attributes...

--End of Particle...
function OnMouseMove(x,y)
	setX(x);
	setY(y);
end

--Functions ...

--End of Functions ...