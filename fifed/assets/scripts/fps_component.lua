local frames = 0
local timer = 0

function Update(dt)
	frames = frames + 1;
	timer = timer + dt
	if timer >= 1 then
		log("FPS: " .. tostring(frames))
		timer = 0
		frames = 0
	end
end
