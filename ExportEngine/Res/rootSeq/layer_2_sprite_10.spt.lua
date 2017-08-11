--sprite...
name = "sprite_1" ;
image = "player.png" ;
x = 467 ;
y = 374 ;
height = 154 ;
width = 129 ;
rotation = 0 ;
scale = 1 ;
alpha = 255 ;
isPhysical = true ;
isStatic = false ;
mass = 50 ;
inertia = -1 ;
friction = 0.5 ;
elastic = 0.5 ;
-- Animation Attributes ...
anim_timer = 50 ;
totalFrames = 27 ;
isRev = 1 ;
rows = 1 ;
cols = 27 ;
--Shapes...
    --LineShapes...
    lineShapesCount = 0 ;
    --End of LineShapes...
    --CircleShapes...
    circleShapesCount = 1 ;
        --circleShape_0...
        circleShape_0_x = 0 ;
        circleShape_0_y = 0 ;
        circleShape_0_r = 77 ;
    --End of CircleShapes...
    -- PolyGonShapes...
    polyGonShapesCount = 0 ;
    --End of PolyGonShapes...
--End of sprite...

--Functions ...
dofile("defines.lua");
function OnLoad()
	printConsol("I am Loaded !");
	gotoAndStop(0);
	playSound("s0.ogg");
end

function OnKeyDown(e)
	if(e == KEY_LEFT)then
		play();
		x = getX();
		x=x-8;
		setX(x);
		local xs = getXScale();
		if(xs>0)then
			setXScale(xs*-1);
		end
	elseif(e == KEY_RIGHT)then
		play();
		x = getX();
		x=x+8;
		setX(x);
		local xs = getXScale();
		if(xs<0)then
			setXScale(xs*-1);
		end
	elseif(e == KEY_UP)then
		if(getVY() == 0) then
			playSound("nuke_cover_open.ogg");
			applyImplus(0,-99000);
		end
	elseif(e == KEY_SPACE)then
		playSound("s0.ogg");
	end
end

function OnKeyUp(e)
	gotoAndStop(0);
	
end

--function OnEnterFrame()
	--printConsol(tostring(getVY()));
--end
--End of Functions ...