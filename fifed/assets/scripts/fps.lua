local script = {
	fps = 0
}

local timer = 0
local frames = 0

function script:init()
	log("fps.lua loaded")
end

function script:update(dt)
	frames = frames + 1;
	timer = timer + dt
	if timer >= 1 then
		script.fps = frames
		timer = 0
		frames = 0
	end
end

return script
