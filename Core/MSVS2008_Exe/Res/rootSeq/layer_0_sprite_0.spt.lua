--sprite...
name = "sprite_23" ;
image = "charkh.png" ;
x = 1876 ;
y = 338 ;
height = 550 ;
width = 551 ;
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
	rotation = rotation + 0.001;
	setRotation(rotation);
end
--End of Functions ...