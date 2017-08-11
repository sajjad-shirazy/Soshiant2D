--sprite...
name = "sprite_35" ;
image = "sun.png" ;
x = 3141 ;
y = 450 ;
height = 208 ;
width = 214 ;
rotation = 0 ;
scale = 1 ;
alpha = 255 ;
isPhysical = false ;
isStatic = false ;
mass = 0 ;
inertia = 0 ;
friction = 0 ;
elastic = 0 ;
-- Animation Attributes ...
anim_timer = 0 ;
totalFrames = 1 ;
isRev = 1 ;
rows = 1 ;
cols = 1 ;
--End of sprite...

--Functions ...

function OnEnterFrame()
	rotation = getRotation();
	rotation = rotation - 0.01;
	setRotation(rotation);
end

--End of Functions ...